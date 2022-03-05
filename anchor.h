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

    virtual int getHeight() = 0;

    virtual int setHeight(int) = 0;

    virtual int getRecomputeId() = 0;

    virtual int setRecomputeId(int) = 0;

    virtual int getChangeId() = 0;

    virtual int setChangeId(int) = 0;

    virtual void markNecessary() = 0;

    virtual void decrementNecessaryCount() = 0;

    virtual void isNecessary() = 0;

    virtual const std::vector<std::shared_ptr<AnchorBase>>& getParents() = 0;

    virtual const std::vector<std::shared_ptr<AnchorBase>>& getChildren() = 0;

    virtual void addParent(const std::shared_ptr<AnchorBase>& parent) = 0;
};

// TODO: I think some of these public functions will be made friend functions, so only the engine class can access them
template <typename T>
class Anchor : public AnchorBase {
   public:
    using SingleInputUpdater = std::function<Anchor<T>(T)>;

    using DualInputUpdater = std::function<Anchor<T>(T, T)>;

    Anchor(T value);

    T get();
    // Always return the latest value.

    void compute(int stabilizationNumber) override;
    // Update the value based on the inputs. Set the recomputeID to stabilizationNumber, only set the changeId if the value changes.

    void markNecessary() override;

    void isNecessary() override;

    void decrementNecessaryCount() override;

    int getHeight() override;

    void setHeight(int height) override;  // Do we need a function to set the height? I don't think so actually. should print an error if the height decreases

    int getRecomputeId() override;

    void setRecomputeId() override;

    int getChangeId() override;

    void setChangeId() override;

    void setValue(T value);

    static Anchor<T> map(const std::shared_ptr<Anchor<T>>& anchor, const SingleInputUpdater& updater);

    static Anchor<T> map2(const std::shared_ptr<Anchor<T>>& anchor1, const std::shared_ptr<Anchor<T>>& anchor2, const DualInputUpdater& updater);
    // Set the children of the node.

    void addParent(const std::shared_ptr<AnchorBase>& parent) override;

    const std::unordered_set<std::shared_ptr<AnchorBase>>& getParents() override;

    const std::unordered_set<std::shared_ptr<AnchorBase>>& getChildren() override;

   private:
    // PRIVATE TYPES
    int d_height;
    T   value;

    std::unordered_set<std::shared_ptr<Anchor<T>>> children;
    // Anchors it depends on... For now, there can only be two but eventually we'll have more

    std::unordered_set<std::shared_ptr<Anchor<T>>> parents;
    // Anchors that depend on it.

    int necessary = 0;
    // For each time an anchor is necessary, increment it by 1. When we unobserve an anchor, we decrement its necessary count.
    // An anchor is necessary if necessary > 0;

    int recomputeId = 0;
    int changeId    = 0;

    SingleInputUpdater d_singleInputUpdater;

    DualInputUpdater d_dualInputUpdater;
};
}  // namespace anchors

#endif