#ifndef ANCHOR_H
#define ANCHOR_H

#include <functional>
#include <memory>
#include <unordered_set>
#include <vector>

namespace anchors {

// TODO: I need to come up with a hash function for these things
class AnchorBase {
   public:
    virtual void compute(int stabilizationNumber) = 0;

    virtual int getHeight()      = 0;
    virtual int getRecomputeId() = 0;

    virtual int getChangeId() = 0;

    virtual int setChangeId(int) = 0;

    virtual void markNecessary() = 0;

    virtual void decrementNecessaryCount() = 0;

    virtual bool isNecessary() = 0;

    virtual const std::unordered_set<std::shared_ptr<AnchorBase>>& getParents() = 0;

    virtual const std::vector<std::shared_ptr<AnchorBase>>& getChildren() = 0;

    virtual void addParent(const std::shared_ptr<AnchorBase>& parent) = 0;
};

// TODO: I think some of these public functions will be made friend functions, so only the engine class can access them
template <typename T>
class Anchor : public AnchorBase {
   public:
    // TODO: These updaters currently take a function that returns a value.
    // Eventually, we want it to return an anchor
    using SingleInputUpdater = std::function<T(T)>;

    using DualInputUpdater = std::function<T(T, T)>;

    Anchor(T value);

    T get();
    // Always return the latest value.

    void compute(int stabilizationNumber) override;
    // Update the value based on the inputs. Set the recomputeID to stabilizationNumber, only set the changeId if the value changes.

    void markNecessary() override;

    bool isNecessary() override;

    void decrementNecessaryCount() override;
    // if this is zero, maybe log a message but it shouldn't ever be

    int getHeight() override;

    int getRecomputeId() override;

    int getChangeId() override;

    void setChangeId(int changeId) override;

    void setValue(T value);

    static Anchor<T> map(const std::shared_ptr<Anchor<T>>& anchor, const SingleInputUpdater& updater);

    static Anchor<T> map2(const std::shared_ptr<Anchor<T>>& anchor1, const std::shared_ptr<Anchor<T>>& anchor2, const DualInputUpdater& updater);
    // Set the children of the node.

    void addParent(const std::shared_ptr<AnchorBase>& parent) override;

    // TODO: I'm breaking encapsulation with the methods below, do I need to? I should probably just return a copy
    const std::unordered_set<std::shared_ptr<AnchorBase>>& getParents() override;

    const std::vector<std::shared_ptr<AnchorBase>>& getChildren() override;

   private:
    Anchor();

    // PRIVATE TYPES
    int d_height;
    T   d_value;

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
};

template <typename T>
Anchor<T>::Anchor(T value) {
    d_value = value;
}

template <typename T>
T Anchor<T>::get() {
    return d_value;
}

template <typename T>
void Anchor<T>::compute(int stabilizationNumber) {
    T newValue;
    d_recomputeId = stabilizationNumber;

    if (d_children.size() == 0) {
        return;
    } else if (d_children.size() == 1) {
        newValue = d_singleInputUpdater(d_children.at(0)->get());
    } else if (d_children.size() == 2) {
        newValue = d_dualInputUpdater(d_children.at(0)->get(), d_children.at(1)->get());
    } else {
        // TODO: This is an invalid situation
    }

    if (newValue != d_value) {
        d_changeId = stabilizationNumber;
        d_value    = newValue;
    }
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
void Anchor<T>::setValue(T value) {
    d_value = value;
}

template <typename T>
Anchor<T> Anchor<T>::map(const std::shared_ptr<Anchor<T>>& anchor, const SingleInputUpdater& updater) {
    Anchor<T> newAnchor;

    newAnchor.d_singleInputUpdater = updater;
    newAnchor.d_children.insert(anchor);

    return newAnchor;
}

template <typename T>
Anchor<T> Anchor<T>::map2(const std::shared_ptr<Anchor<T>>& anchor1, const std::shared_ptr<Anchor<T>>& anchor2, const DualInputUpdater& updater) {
    Anchor<T> newAnchor;

    newAnchor.d_dualInputUpdater = updater;
    newAnchor.d_children.insert(anchor1);
    newAnchor.d_children.insert(anchor2);

    return newAnchor;
}

template <typename T>
void Anchor<T>::addParent(const std::shared_ptr<AnchorBase>& parent) {
    d_parents.insert(parent);
}

template <typename T>
const std::unordered_set<std::shared_ptr<AnchorBase>>& Anchor<T>::getParents() {
    return d_parents;
}

template <typename T>
const std::vector<std::shared_ptr<AnchorBase>>& Anchor<T>::getChildren() {
    return d_children;
}

}  // namespace anchors

#endif