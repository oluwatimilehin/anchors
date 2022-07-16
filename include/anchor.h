// anchor.h
#ifndef ANCHORS_ANCHOR_H
#define ANCHORS_ANCHOR_H

#include "anchorbase.h"

#include <algorithm>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <functional>
#include <memory>
#include <unordered_set>
#include <vector>

template <>
struct std::hash<anchors::AnchorBase> {
    std::size_t operator()(const anchors::AnchorBase& a) const noexcept {
        return hash_value(a.getId());
    }
};

// Used by the priority queue in the Engine class to store Anchors in increasing
// order.
template <>
struct std::less<std::shared_ptr<anchors::AnchorBase>> {
    bool operator()(const std::shared_ptr<anchors::AnchorBase>& a1,
                    const std::shared_ptr<anchors::AnchorBase>& a2) const {
        return a1->getHeight() > a2->getHeight();
    }
};

namespace anchors {

/**
 * This class exists to simplify the `Anchors` API, so that we can pass an
 * Anchor around using only the type of its value, and excluding the type of its
 * input `Anchors`.
 * @tparam T - type of an Anchor's value.
 */
template <typename T>
class AnchorWrap : public AnchorBase {
   public:
    virtual ~AnchorWrap(){};
    virtual T get() const = 0;

   protected:
    virtual void set(const T& value) = 0;

    friend class Engine;
};

/**
 * A single node in the computation graph containing a value.
 * @tparam T - type of the Anchor's value
 * @tparam InputType1 - optional type of the first input Anchor, if applicable.
 * @tparam InputType2 - optional type of the second input Anchor, if applicable.
 */
template <typename T, typename InputType1 = T, typename InputType2 = T>
class Anchor : public AnchorWrap<T> {
   public:
    /**
     * Alias for function that accepts an input of type `InputType1` and returns
     * a value of type `T`.
     */
    using SingleInputUpdater = std::function<T(InputType1&)>;

    /**
     * Alias for function that accepts inputs of type `InputType1` and
     * `InputType2` and returns a value of type `T`.
     */
    using DualInputUpdater = std::function<T(InputType1&, InputType2&)>;

    Anchor() = delete;

    /**
     * Creates an Anchor. See Anchors::create(const T& value)
     * @param value - initial value of the Anchor
     */
    explicit Anchor(const T& value);

    /**
     * Creates an Anchor from an input Anchor. See Anchors::map()
     *
     * @param input - input Anchor.
     * @param updater - function that maps the input Anchor to the output.
     */
    explicit Anchor(const std::shared_ptr<AnchorWrap<InputType1>>& input,
                    const SingleInputUpdater&                      updater);

    /**
     * Creates an Anchor from two input Anchors. See Anchors::map2()
     *
     * @param firstInput - first input Anchor.
     * @param secondInput - second input Anchor.
     * @param updater - function that maps the input Anchors to the output.
     */
    explicit Anchor(const std::shared_ptr<AnchorWrap<InputType1>>& firstInput,
                    const std::shared_ptr<AnchorWrap<InputType2>>& secondInput,
                    const DualInputUpdater&                        updater);

    Anchor(const Anchor& a) = delete;

    ~Anchor() override = default;

    friend class Engine;

    friend std::ostream& operator<<(std::ostream& out, const Anchor& anchor) {
        out << "[ value=" << anchor.get() << ", height=" << anchor.getHeight(),
            ", firstDependency=" << anchor.d_firstDependency
                                 << ", secondDependency="
                                 << anchor.d_secondDependency << " ]";

        return out;
    }

   private:
    // PRIVATE MANIPULATORS
    T get() const override;
    // Returns the current value of an Anchor.

    void compute(int stabilizationNumber) override;
    // Computes the value of an Anchor based on its inputs and updater function.
    // When this function is called by the Engine, it is guaranteed that the
    // inputs are up-to-date.
    // This function also sets the recomputeID of the Anchor to the given
    // stabilizationNumber, and will update the changeId only if the Anchor
    // value changes after recomputing.

    void markNecessary() override;
    // Increments the `necessary count` of an Anchor. An Anchor is necessary if
    // it is a dependency of an observed Anchor, either directly or indirectly.

    bool isNecessary() const override;
    // Returns true if at least one observed Anchor depends on it, either
    // directly or indirectly.

    bool isStale() const override;
    // Returns true if the Anchor is necessary and has never been computed or
    // its recomputeId is less than the changeId of one of its children.

    void decrementNecessaryCount() override;
    // Decrements the `necessary count` of an Anchor after a dependant is marked
    // as unobserved.

    AnchorBase::AnchorId getId() const override;
    // Returns the generated UUID of the Anchor.

    int getHeight() const override;
    // Returns the height of an Anchor. An Anchor's height must always be
    // greater than the heights of its inputs.

    int getRecomputeId() const override;
    // Returns the ID at which the Anchor was last computed.

    int getChangeId() const override;
    // Returns the ID at which the value of an Anchor last changed.

    void setChangeId(int changeId) override;
    // Set the ID at which the value of an Anchor changed.

    void set(const T& value) override;
    // Set the value of the Anchor.

    void addDependant(const std::shared_ptr<AnchorBase>& dependant) override;
    // Adds the given Anchor as a dependant of this Anchor. When this Anchor's
    // value changes, we want to know update its dependants.

    void removeDependant(const std::shared_ptr<AnchorBase>& dependant) override;
    // Removes the given Anchor from the dependants of this Anchor.

    std::unordered_set<std::shared_ptr<AnchorBase>> getDependants()
        const override;
    // Returns the dependants of this Anchor.

    std::vector<std::shared_ptr<AnchorBase>> getDependencies() const override;
    // Returns the dependencies of this Anchor.

    // PRIVATE DATA
    boost::uuids::random_generator d_idGenerator;

    boost::uuids::uuid d_id;

    T d_value{};

    int d_height{};
    // The height of the Anchor. Its value is 0 if it has no dependencies.
    // Otherwise, its value = Max(Height of Inputs) + 1

    int d_necessary{};
    // Indicates how many Anchors this node is a dependency of either directly
    // or indirectly.

    int d_numDependencies{};
    // Number of dependencies this Anchor has.

    int d_recomputeId{};
    // The stabilization number at which this Anchor was last recomputed

    int d_changeId{};
    // The stabilization number at which the value of this Anchor last changed

    bool d_hasNeverBeenComputed;

    const std::shared_ptr<AnchorWrap<InputType1>> d_firstDependency;
    const std::shared_ptr<AnchorWrap<InputType2>> d_secondDependency;

    std::unordered_set<std::shared_ptr<AnchorBase>> d_dependants;
    // Anchors that depend on it.

    SingleInputUpdater d_singleInputUpdater;

    DualInputUpdater d_dualInputUpdater;
};

template <typename T, typename InputType1, typename InputType2>
Anchor<T, InputType1, InputType2>::Anchor(const T& value)
    : d_id(d_idGenerator()),
      d_value(value),
      d_hasNeverBeenComputed(true),
      d_dependants() {}

template <typename T, typename InputType1, typename InputType2>
Anchor<T, InputType1, InputType2>::Anchor(
    const std::shared_ptr<AnchorWrap<InputType1>>& input,
    const SingleInputUpdater&                      updater)
    : d_id(d_idGenerator()),
      d_height(input->getHeight() + 1),
      d_numDependencies(1),
      d_hasNeverBeenComputed(true),
      d_firstDependency(input),
      d_dependants(),
      d_singleInputUpdater(updater) {}

template <typename T, typename InputType1, typename InputType2>
Anchor<T, InputType1, InputType2>::Anchor(
    const std::shared_ptr<AnchorWrap<InputType1>>& firstInput,
    const std::shared_ptr<AnchorWrap<InputType2>>& secondInput,
    const DualInputUpdater&                        updater)
    : d_id(d_idGenerator()),
      d_height(std::max(firstInput->getHeight(), secondInput->getHeight()) + 1),
      d_numDependencies(2),
      d_hasNeverBeenComputed(true),
      d_firstDependency(firstInput),
      d_secondDependency(secondInput),
      d_dependants(),
      d_dualInputUpdater(updater) {}

template <typename T, typename InputType1, typename InputType2>
T Anchor<T, InputType1, InputType2>::get() const {
    return d_value;
}

template <typename T, typename InputType1, typename InputType2>
void Anchor<T, InputType1, InputType2>::compute(int stabilizationNumber) {
    if (d_recomputeId == stabilizationNumber) {
        // Don't compute a node more than once in the same cycle
        return;
    }

    T newValue;
    d_recomputeId = stabilizationNumber;

    d_hasNeverBeenComputed = false;

    if (d_numDependencies == 0) {
        return;
    }

    if (d_numDependencies == 1) {
        InputType1 inputVal = d_firstDependency->get();
        newValue            = d_singleInputUpdater(inputVal);
    } else {
        InputType1 inputVal  = d_firstDependency->get();
        InputType2 inputVal2 = d_secondDependency->get();

        newValue = d_dualInputUpdater(inputVal, inputVal2);
    }

    if (newValue != d_value) {
        d_changeId = stabilizationNumber;
        d_value    = newValue;
    }
}

template <typename T, typename InputType1, typename InputType2>
void Anchor<T, InputType1, InputType2>::markNecessary() {
    d_necessary++;
}

template <typename T, typename InputType1, typename InputType2>
bool Anchor<T, InputType1, InputType2>::isNecessary() const {
    return d_necessary > 0;
}

template <typename T, typename InputType1, typename InputType2>
bool Anchor<T, InputType1, InputType2>::isStale() const {
    bool recomputeIdLessThanChildChangeId = false;

    if (d_numDependencies >= 1) {
        recomputeIdLessThanChildChangeId =
            d_recomputeId < d_firstDependency->getChangeId();

        if (!recomputeIdLessThanChildChangeId && d_numDependencies == 2) {
            recomputeIdLessThanChildChangeId |=
                d_recomputeId < d_secondDependency->getChangeId();
        }
    }

    return isNecessary() &
           (d_hasNeverBeenComputed || recomputeIdLessThanChildChangeId);
}

template <typename T, typename InputType1, typename InputType2>
void Anchor<T, InputType1, InputType2>::decrementNecessaryCount() {
    if (d_necessary <= 0) {
        return;
    }

    d_necessary--;
}

template <typename T, typename InputType1, typename InputType2>
AnchorBase::AnchorId Anchor<T, InputType1, InputType2>::getId() const {
    return d_id;
}

template <typename T, typename InputType1, typename InputType2>
int Anchor<T, InputType1, InputType2>::getHeight() const {
    return d_height;
}

template <typename T, typename InputType1, typename InputType2>
int Anchor<T, InputType1, InputType2>::getRecomputeId() const {
    return d_recomputeId;
}

template <typename T, typename InputType1, typename InputType2>
int Anchor<T, InputType1, InputType2>::getChangeId() const {
    return d_changeId;
}

template <typename T, typename InputType1, typename InputType2>
void Anchor<T, InputType1, InputType2>::setChangeId(int changeId) {
    d_changeId = changeId;
}

template <typename T, typename InputType1, typename InputType2>
void Anchor<T, InputType1, InputType2>::set(const T& value) {
    d_value = value;
}

template <typename T, typename InputType1, typename InputType2>
void Anchor<T, InputType1, InputType2>::addDependant(
    const std::shared_ptr<AnchorBase>& dependant) {
    d_dependants.insert(dependant);
}

template <typename T, typename InputType1, typename InputType2>
void Anchor<T, InputType1, InputType2>::removeDependant(
    const std::shared_ptr<AnchorBase>& dependant) {
    d_dependants.erase(dependant);
}

template <typename T, typename InputType1, typename InputType2>
std::unordered_set<std::shared_ptr<AnchorBase>>
Anchor<T, InputType1, InputType2>::getDependants() const {
    std::unordered_set<std::shared_ptr<AnchorBase>> result;

    for (auto& parent : d_dependants) {
        result.insert(parent);
    }

    return result;
}

template <typename T, typename InputType1, typename InputType2>
std::vector<std::shared_ptr<AnchorBase>>
Anchor<T, InputType1, InputType2>::getDependencies() const {
    std::vector<std::shared_ptr<AnchorBase>> result;

    if (d_numDependencies >= 1) {
        result.push_back(d_firstDependency);

        if (d_numDependencies == 2) {
            result.push_back(d_secondDependency);
        }
    }

    return result;
}

}  // namespace anchors

#endif
