#ifndef ANCHORS_ANCHOR_H
#define ANCHORS_ANCHOR_H

#include <anchorbase.h>

#include <functional>
#include <memory>
#include <unordered_set>
#include <vector>

namespace anchors {

template <typename T>
class Anchor;

template <typename T>
using AnchorPtr = std::shared_ptr<Anchor<T>>;

// TODO: I need to come up with a hash function for these things
// TODO: I think some of these public functions will be made friend functions, so only the engine class can access them
// TODO:: What should the copy constructors look like? What requirements do we have for any type T?
template <typename T>
class Anchor : public AnchorBase {
   public:
    Anchor();

    explicit Anchor(const T& value);

    using SingleInputUpdater = std::function<T(T&)>;

    using DualInputUpdater = std::function<T(T&, T&)>;

    virtual ~Anchor() = default;

    static AnchorPtr<T> create(const T& value);

    static AnchorPtr<T> map(const AnchorPtr<T>& anchor, const SingleInputUpdater& updater);

    static AnchorPtr<T> map2(const AnchorPtr<T>& anchor1, const AnchorPtr<T>& anchor2, const DualInputUpdater& updater);
    // Set the children of the node.

    friend class Engine;

   private:
    T get();
    // Always return the latest value.

    void compute(int stabilizationNumber) override;
    // Update the value based on the inputs. Set the recomputeID to stabilizationNumber, only set the changeId if the value changes.

    void markNecessary() override;

    bool isNecessary() override;

    bool isStale() override;

    void decrementNecessaryCount() override;
    // if this is zero, maybe log a message but it shouldn't ever be

    int getHeight() override;

    int getRecomputeId() override;

    int getChangeId() override;

    void setChangeId(int changeId) override;

    void setValue(const T& value);

    void addParent(const std::shared_ptr<AnchorBase>& parent) override;

    std::unordered_set<std::shared_ptr<AnchorBase>> getParents() override;

    std::vector<std::shared_ptr<AnchorBase>> getChildren() override;

    // PRIVATE TYPES
    int d_height{};
    T   d_value{};

    bool d_isStale = true;  // TODO: eventually, we will check if its recompute ID is less than one of its children

    std::vector<std::shared_ptr<Anchor<T>>> d_children;
    // Anchors it depends on... For now, there can only be two but eventually we'll have more

    std::unordered_set<std::shared_ptr<Anchor<T>>> d_parents;
    // Anchors that depend on it.

    int d_necessary = 0;
    // For each time an anchor is necessary, increment it by 1. When we unobserve an anchor, we decrement its necessary count.
    // An anchor is necessary if necessary > 0;

    int d_recomputeId = 0;
    int d_changeId    = 0;

    SingleInputUpdater d_singleInputUpdater;

    DualInputUpdater d_dualInputUpdater;

    //    template <T>
    //    struct MakeSharedEnabler;
};

// template <typename T>
// struct Anchor<T>::MakeSharedEnabler<T> : public Anchor<T>{
//         MakeSharedEnabler(T val) : Anchor<T>(val){
//         }
//
//         MakeSharedEnabler() : Anchor<T>(){
//         }
// };

template <typename T>
Anchor<T>::Anchor(const T& value) : d_value(value),
                                    d_children(),
                                    d_parents() {
}

template <typename T>
Anchor<T>::Anchor() : d_children(),
                      d_parents() {
}

template <typename T>
T Anchor<T>::get() {
    return d_value;
}

template <typename T>
void Anchor<T>::compute(int stabilizationNumber) {
    if (d_recomputeId == stabilizationNumber) {
        return;  // TODO: This is to prevent computing a node more than once in the same cycle, but ideally, we should prevent this. Maybe have a separate set of nodes in the recompute heap
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
bool Anchor<T>::isNecessary() {
    return d_necessary > 0;
}

template <typename T>
bool Anchor<T>::isStale() {
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
int Anchor<T>::getHeight() {
    return d_height;
}

template <typename T>
int Anchor<T>::getRecomputeId() {
    return d_recomputeId;
}

template <typename T>
int Anchor<T>::getChangeId() {
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
AnchorPtr<T> Anchor<T>::create(const T& value) {
    AnchorPtr<T> newAnchor(std::make_shared<Anchor<T>>(value));

    return newAnchor;
}

template <typename T>
AnchorPtr<T> Anchor<T>::map(const AnchorPtr<T>& anchor, const SingleInputUpdater& updater) {
    AnchorPtr<T> newAnchor(std::make_shared<Anchor<T>>());
    ;

    newAnchor->d_singleInputUpdater = updater;
    newAnchor->d_children.push_back(anchor);

    newAnchor->d_height = anchor->getHeight() + 1;

    return newAnchor;
}

template <typename T>
AnchorPtr<T> Anchor<T>::map2(const AnchorPtr<T>& anchor1, const AnchorPtr<T>& anchor2, const DualInputUpdater& updater) {
    AnchorPtr<T> newAnchor(std::make_shared<Anchor<T>>());

    newAnchor->d_dualInputUpdater = updater;
    newAnchor->d_children.push_back(anchor1);
    newAnchor->d_children.push_back(anchor2);

    int height1 = anchor1->getHeight();
    int height2 = anchor2->getHeight();

    newAnchor->d_height = height1 > height2 ? height1 + 1 : height2 + 1;
    return newAnchor;
}

template <typename T>
void Anchor<T>::addParent(const std::shared_ptr<AnchorBase>& parent) {
    d_parents.insert(std::dynamic_pointer_cast<Anchor<T>>(parent));
    // TODO: can I avoid this cast?
}

template <typename T>
std::unordered_set<std::shared_ptr<AnchorBase>> Anchor<T>::getParents() {
    std::unordered_set<std::shared_ptr<AnchorBase>> result;

    for (auto& parent : d_parents) {
        result.insert(parent);
    }

    return result;
}

template <typename T>
std::vector<std::shared_ptr<AnchorBase>> Anchor<T>::getChildren() {
    std::vector<std::shared_ptr<AnchorBase>> result;

    for (auto& child : d_children) {
        result.push_back(child);
    }

    return result;
}

}  // namespace anchors

#endif