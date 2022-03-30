#ifndef ANCHORS_ANCHOR_H
#define ANCHORS_ANCHOR_H

#include "anchorbase.h"

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
class Anchor : public AnchorBase {
   public:
    using SingleInputUpdater = std::function<T(T&)>;

    using DualInputUpdater = std::function<T(T&, T&)>;

    // TODO: I want to make these constructors private, so you can only create
    // an anchor as a ptr.
    Anchor();

    explicit Anchor(const T& value);

    Anchor(const Anchor& a) = delete;

    ~Anchor() override = default;

    friend class Engine;

    friend class AnchorUtil;

    friend std::ostream& operator<<(std::ostream& out, const Anchor& anchor) {
        out << "[ value=" << anchor.get() << ", id=" << anchor.getId()
            << ", height=" << anchor.getHeight(),
            ", parents=" << anchor.getParents()
                         << " children=" << anchor.getChildren() << " ]";
    }

   private:
    T get() const;
    // Always return the latest value.

    void compute(int stabilizationNumber) override;
    // Update the value based on the inputs. Set the recomputeID to
    // stabilizationNumber, only set the changeId if the value changes.

    void markNecessary() override;

    bool isNecessary() const override;

    bool isStale() const override;

    void decrementNecessaryCount() override;
    // if this is zero, maybe log a message, but it shouldn't ever be

    AnchorId getId() const override;

    int getHeight() const override;

    int getRecomputeId() const override;

    int getChangeId() const override;

    void setChangeId(int changeId) override;

    void setValue(const T& value);

    void addParent(const std::shared_ptr<AnchorBase>& parent) override;

    std::unordered_set<std::shared_ptr<AnchorBase>> getParents() const override;

    std::vector<std::shared_ptr<AnchorBase>> getChildren() const override;

    // PRIVATE DATA
    boost::uuids::random_generator d_idGenerator;

    int d_height{};
    int d_necessary{};
    // For each time an anchor is necessary, increment it by 1. When we
    // unobserve an anchor, we decrement its necessary count. An anchor is
    // necessary if necessary > 0;

    int d_recomputeId{};
    int d_changeId{};

    boost::uuids::uuid d_id;

    T d_value{};

    bool d_isStale;  // TODO: eventually, we will check if its recompute ID is
                     // less than one of its children

    std::vector<std::shared_ptr<Anchor<T>>> d_children;
    // Anchors it depends on... For now, there can only be two but eventually
    // we'll have more

    std::unordered_set<std::shared_ptr<Anchor<T>>> d_parents;
    // Anchors that depend on it.

    SingleInputUpdater d_singleInputUpdater;

    DualInputUpdater d_dualInputUpdater;

    //    template <T>
    //    struct MakeSharedEnabler;
};

template <typename T>
Anchor<T>::Anchor(const T& value)
    : d_id(d_idGenerator()),
      d_value(value),
      d_isStale(true),
      d_children(),
      d_parents() {}

template <typename T>
Anchor<T>::Anchor()
    : d_id(d_idGenerator()), d_isStale(true), d_children(), d_parents() {}

template <typename T>
T Anchor<T>::get() const {
    return d_value;
}

template <typename T>
void Anchor<T>::compute(int stabilizationNumber) {
    if (d_recomputeId == stabilizationNumber) {
        // Don't compute a node more than once in the same cycle
        return;
    }

    T newValue;
    d_recomputeId = stabilizationNumber;

    if (d_children.empty()) {
        return;
    }

    if (d_children.size() == 1) {
        auto input = d_children.at(0)->get();
        newValue   = d_singleInputUpdater(input);

    } else {
        auto input1 = d_children.at(0)->get();
        auto input2 = d_children.at(1)->get();

        newValue = d_dualInputUpdater(input1, input2);
    }

    if (newValue != d_value) {
        d_changeId = stabilizationNumber;
        d_value    = newValue;
    }

    d_isStale = false;
}

template <typename T>
void Anchor<T>::markNecessary() {
    d_necessary++;
}

template <typename T>
bool Anchor<T>::isNecessary() const {
    return d_necessary > 0;
}

template <typename T>
bool Anchor<T>::isStale() const {
    return isNecessary() & d_isStale;
}

template <typename T>
void Anchor<T>::decrementNecessaryCount() {
    if (d_necessary <= 0) {
        // TODO:: LOG an error
        return;
    }

    d_necessary--;
}

template <typename T>
AnchorBase::AnchorId Anchor<T>::getId() const {
    return d_id;
}

template <typename T>
int Anchor<T>::getHeight() const {
    return d_height;
}

template <typename T>
int Anchor<T>::getRecomputeId() const {
    return d_recomputeId;
}

template <typename T>
int Anchor<T>::getChangeId() const {
    return d_changeId;
}

template <typename T>
void Anchor<T>::setChangeId(int changeId) {
    d_changeId = changeId;
}

template <typename T>
void Anchor<T>::setValue(const T& value) {
    d_value = value;
}

template <typename T>
void Anchor<T>::addParent(const std::shared_ptr<AnchorBase>& parent) {
    d_parents.insert(std::dynamic_pointer_cast<Anchor<T>>(parent));
}

template <typename T>
std::unordered_set<std::shared_ptr<AnchorBase>> Anchor<T>::getParents() const {
    std::unordered_set<std::shared_ptr<AnchorBase>> result;

    for (auto& parent : d_parents) {
        result.insert(parent);
    }

    return result;
}

template <typename T>
std::vector<std::shared_ptr<AnchorBase>> Anchor<T>::getChildren() const {
    std::vector<std::shared_ptr<AnchorBase>> result;

    for (auto& child : d_children) {
        result.push_back(child);
    }

    return result;
}

}  // namespace anchors

#endif