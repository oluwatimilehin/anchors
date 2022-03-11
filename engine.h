#ifndef ANCHORS_ENGINE_H
#define ANCHORS_ENGINE_H

#include <anchor.h>

#include <memory>
#include <queue>
#include <unordered_set>

namespace std {
template <>
struct less<std::shared_ptr<anchors::AnchorBase>> {
    bool operator()(const std::shared_ptr<anchors::AnchorBase>& a1, const std::shared_ptr<anchors::AnchorBase>& a2) const {
        return a1->getHeight() > a2->getHeight();
    }
};
}  // namespace std

namespace anchors {
class Engine {
   public:
    Engine();

    template <typename T>
    T get(const std::shared_ptr<Anchor<T>>& anchor);
    // If the node is observed (or necessary), call stabilize
    // If the node is not observed or necessary, you might get a stale value

    template <typename T>
    void set(std::shared_ptr<Anchor<T>>& anchor, T val);

    template <typename T>
    void observe(std::shared_ptr<Anchor<T>>& anchor);
    // when you observe a node, mark all its children as necessary. Only observed nodes will give the latest value

    template <typename T>
    void unobserve(std::shared_ptr<Anchor<T>>& anchor);

   private:

    void stabilize();

    template <typename T>
    void traverse(std::shared_ptr<Anchor<T>>& current, std::unordered_set<std::shared_ptr<AnchorBase>>&);

    std::unordered_set<std::shared_ptr<AnchorBase>> d_observedNodes;
    // - observed nodes // What do I use this observed nodes for? To stabilize, I can just read from the recompute heap, right?

    std::priority_queue<std::shared_ptr<AnchorBase>> d_recomputeHeap;
    //  Only add something to the recompute heap if it is necessary and stale.

    std::priority_queue<std::shared_ptr<AnchorBase>> d_adjustHeightsHeap;
    // Update the adjust-heights heap when setUpdater is called. Will use later.

    int d_stabilizationNumber = 0;
};

template <typename T>
T Engine::get(const std::shared_ptr<Anchor<T>>& anchor) {
    if (anchor->isNecessary()) {
        stabilize();
    }

    return anchor->get();
}

template <typename T>
void Engine::set(std::shared_ptr<Anchor<T>>& anchor, T val) {
    int changeId = anchor->getChangeId();

    T oldVal = anchor->get();

    if (oldVal == val)
        return;

    anchor->setChangeId(d_stabilizationNumber);
    // anchor->setStab(d_stabilizationNumber);

    anchor->setValue(val);

    if (anchor->isNecessary()) {
        for (const auto& parent : anchor->getParents()) {
            if (parent->isNecessary()) {
                d_recomputeHeap.push(parent);
            }
        }
    }
}

template <typename T>
void Engine::observe(std::shared_ptr<Anchor<T>>& anchor) {
    // Traverse the graph.
    // anchor -> find children, mark them as necessary and update the current node as its parent. Repeat the same for each child
    if (d_observedNodes.contains(anchor)) {
        return;
    }

    d_observedNodes.insert(anchor);
    d_recomputeHeap.push(anchor);

    std::unordered_set<std::shared_ptr<AnchorBase>> visited;
    traverse(anchor, visited);
}

template <typename T>
void Engine::traverse(std::shared_ptr<Anchor<T>>& current, std::unordered_set<std::shared_ptr<AnchorBase>>& visited) {
    if (visited.contains(current)) {
        return;
    }

    visited.insert(current);
    current->markNecessary();

    for (auto& child : current->getChildren()) {
        auto castChild = std::dynamic_pointer_cast<Anchor<T>>(child);
        traverse(castChild, visited);
        child->addParent(current);
    }
}

template <typename T>
void Engine::unobserve(std::shared_ptr<Anchor<T>>& anchor) {
    if (!d_observedNodes.contains(anchor)) {
        return;
    }

    // TODO: On unobserve:
    //   - decrement the necessary count of this node and all its children.
    //   - remove it as a parent for each child. Do we actually need to do this?
    //

    d_observedNodes.erase(anchor);
}



}  // namespace anchors
#endif
