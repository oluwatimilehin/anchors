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

template <>
struct std::less<std::shared_ptr<anchors::AnchorBase>> {
    bool operator()(const std::shared_ptr<anchors::AnchorBase>& a1,
                    const std::shared_ptr<anchors::AnchorBase>& a2) const {
        return a1->getHeight() > a2->getHeight();
    }
};

namespace anchors {

template <typename T>
class AnchorWrap : public AnchorBase {
   public:
    virtual ~AnchorWrap(){};

    virtual T get() const = 0;

    virtual void set(const T& value) = 0;
};

template <typename T, typename InputType1 = T, typename InputType2 = T>
class Anchor : public AnchorWrap<T> {
   public:
    using SingleInputUpdater = std::function<T(
        InputType1&)>;  // TODO: I think the inputs should be const

    using DualInputUpdater = std::function<T(InputType1&, InputType2&)>;

    // TODO: I want to make these constructors private, so you can only
    // create an anchor as a ptr.
    Anchor() = delete;

    // Always return the latest value.

    explicit Anchor(const T& value);

    explicit Anchor(const std::shared_ptr<AnchorWrap<InputType1>>& firstChild,
                    const SingleInputUpdater&                      updater);

    explicit Anchor(const std::shared_ptr<AnchorWrap<InputType1>>& firstChild,
                    const std::shared_ptr<AnchorWrap<InputType2>>& secondChild,
                    const DualInputUpdater&                        updater);

    Anchor(const Anchor& a) = delete;

    ~Anchor() override = default;

    T get() const override;

    friend class Engine;

    friend std::ostream& operator<<(std::ostream& out, const Anchor& anchor) {
        out << "[ value=" << anchor.get() << ", height=" << anchor.getHeight(),
            ", parents=" << anchor.getDependents()
                         << " children=" << anchor.getDependencies() << " ]";
    }

   private:
    void compute(int stabilizationNumber) override;
    // Update the value based on the inputs. Set the recomputeID to
    // stabilizationNumber, only set the changeId if the value changes.

    void markNecessary() override;

    bool isNecessary() const override;

    bool isStale() const override;

    void decrementNecessaryCount() override;
    // if this is zero, maybe log a message, but it shouldn't ever be

    AnchorBase::AnchorId getId() const override;

    int getHeight() const override;

    int getRecomputeId() const override;

    int getChangeId() const override;

    void setChangeId(int changeId) override;

    void set(const T& value) override;

    void addDependent(const std::shared_ptr<AnchorBase>& parent) override;

    std::unordered_set<std::shared_ptr<AnchorBase>> getDependents()
        const override;

    std::vector<std::shared_ptr<AnchorBase>> getDependencies() const override;

    // PRIVATE DATA
    boost::uuids::random_generator d_idGenerator;

    boost::uuids::uuid d_id;

    T d_value{};

    int d_height{};
    int d_necessary{};
    // For each time an anchor is necessary, increment it by 1. When we
    // unobserve an anchor, we decrement its necessary count. An anchor is
    // necessary if necessary > 0;

    int d_numChildren{};

    int d_recomputeId{};
    int d_changeId{};

    bool d_isStale;  // TODO: eventually, we will check if its recompute ID
                     // is less than one of its children

    const std::shared_ptr<AnchorWrap<InputType1>> d_firstChild;
    const std::shared_ptr<AnchorWrap<InputType2>> d_secondChild;

    std::unordered_set<std::shared_ptr<AnchorBase>> d_parents;
    // Anchors that depend on it.

    SingleInputUpdater d_singleInputUpdater;

    DualInputUpdater d_dualInputUpdater;

    //    template <T>
    //    struct MakeSharedEnabler;
};

template <typename T, typename InputType1, typename InputType2>
Anchor<T, InputType1, InputType2>::Anchor(const T& value)
    : d_id(d_idGenerator()), d_value(value), d_isStale(true), d_parents() {}

template <typename T, typename InputType1, typename InputType2>
Anchor<T, InputType1, InputType2>::Anchor(
    const std::shared_ptr<AnchorWrap<InputType1>>& firstChild,
    const SingleInputUpdater&                      updater)
    : d_id(d_idGenerator()),
      d_height(firstChild->getHeight() + 1),
      d_numChildren(1),
      d_isStale(true),
      d_firstChild(firstChild),
      d_parents(),
      d_singleInputUpdater(updater) {}

template <typename T, typename InputType1, typename InputType2>
Anchor<T, InputType1, InputType2>::Anchor(
    const std::shared_ptr<AnchorWrap<InputType1>>& firstChild,
    const std::shared_ptr<AnchorWrap<InputType2>>& secondChild,
    const DualInputUpdater&                        updater)
    : d_id(d_idGenerator()),
      d_height(std::max(firstChild->getHeight(), secondChild->getHeight()) + 1),
      d_numChildren(2),
      d_isStale(true),
      d_firstChild(firstChild),
      d_secondChild(secondChild),
      d_parents(),
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

    d_isStale = false;

    if (d_numChildren == 0) {
        return;
    }

    if (d_numChildren == 1) {
        InputType1 inputVal = d_firstChild->get();
        newValue            = d_singleInputUpdater(inputVal);
    } else {
        InputType1 inputVal  = d_firstChild->get();
        InputType2 inputVal2 = d_secondChild->get();

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
    return isNecessary() & d_isStale;
}

template <typename T, typename InputType1, typename InputType2>
void Anchor<T, InputType1, InputType2>::decrementNecessaryCount() {
    if (d_necessary <= 0) {
        // TODO:: LOG an error
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
void Anchor<T, InputType1, InputType2>::addDependent(
    const std::shared_ptr<AnchorBase>& parent) {
    d_parents.insert(parent);
}

template <typename T, typename InputType1, typename InputType2>
std::unordered_set<std::shared_ptr<AnchorBase>>
Anchor<T, InputType1, InputType2>::getDependents() const {
    std::unordered_set<std::shared_ptr<AnchorBase>> result;

    for (auto& parent : d_parents) {
        result.insert(parent);
    }

    return result;
}

template <typename T, typename InputType1, typename InputType2>
std::vector<std::shared_ptr<AnchorBase>>
Anchor<T, InputType1, InputType2>::getDependencies() const {
    std::vector<std::shared_ptr<AnchorBase>> result;

    if (d_numChildren >= 1) {
        result.push_back(d_firstChild);

        if (d_numChildren == 2) {
            result.push_back(d_secondChild);
        }
    }

    return result;
}

}  // namespace anchors

#endif