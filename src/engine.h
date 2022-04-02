#ifndef ANCHORS_ENGINE_H
#define ANCHORS_ENGINE_H

#include "anchor.h"

#include <memory>
#include <queue>
#include <unordered_set>

namespace anchors {
// The engine only knows about necessary nodes
class Engine {
   public:
    Engine();

    template <typename T>
    T get(const std::shared_ptr<AnchorWrap<T>>& anchor);
    // If the node is observed (or necessary), call stabilize
    // If the node is not observed or necessary, you might get a stale value

    template <typename T>
    void set(std::shared_ptr<AnchorWrap<T>>& anchor, T val);

    template <typename T>
    void observe(std::shared_ptr<AnchorWrap<T>>& anchor);
    // when you observe a node, mark all its children as necessary. Only
    // observed nodes will give the latest value

    template <typename T>
    void observe(std::vector<std::shared_ptr<AnchorWrap<T>>>& anchors);

    template <typename T>
    void unobserve(std::shared_ptr<AnchorWrap<T>>& anchor);

   private:
    void stabilize();

    void traverse(std::shared_ptr<AnchorBase>& current,
                  std::unordered_set<std::shared_ptr<AnchorBase>>&);

    std::unordered_set<std::shared_ptr<AnchorBase>> d_observedNodes;
    // - observed nodes // What do I use this observed nodes for? To stabilize,
    // I can just read from the recompute heap, right?

    std::priority_queue<std::shared_ptr<AnchorBase>> d_recomputeHeap;
    //  Only add something to the recompute heap if it is necessary and stale.

    std::unordered_set<std::shared_ptr<AnchorBase>> d_recomputeSet;

    //    std::priority_queue<std::shared_ptr<AnchorBase>> d_adjustHeightsHeap;
    //    // Update the adjust-heights heap when setUpdater is called. Will use
    //    later.

    int d_stabilizationNumber = 0;
};

template <typename T>
T Engine::get(const std::shared_ptr<AnchorWrap<T>>& anchor) {
    if (anchor->isNecessary()) {
        stabilize();
    }

    return anchor->get();
}

template <typename T>
void Engine::set(std::shared_ptr<AnchorWrap<T>>& anchor, T val) {
    T oldVal = anchor->get();

    if (oldVal == val) return;

    anchor->setChangeId(d_stabilizationNumber);
    anchor->set(val);

    if (anchor->isNecessary()) {
        for (const auto& parent : anchor->getDependents()) {
            if (parent->isNecessary() && !d_recomputeSet.contains(parent)) {
                d_recomputeHeap.push(parent);
                d_recomputeSet.insert(parent);
            }
        }
    }
}

template <typename T>
void Engine::observe(std::shared_ptr<AnchorWrap<T>>& anchor) {
    // Traverse the graph.
    // anchor -> find children, mark them as necessary and update the current
    // node as its parent. Repeat the same for each child
    if (d_observedNodes.contains(anchor)) {
        return;
    }

    d_observedNodes.insert(anchor);

    std::unordered_set<std::shared_ptr<AnchorBase>> visited;
    std::shared_ptr<AnchorBase>                     anchorBase = anchor;
    traverse(anchorBase, visited);
}

template <typename T>
void Engine::observe(std::vector<std::shared_ptr<AnchorWrap<T>>>& anchors) {
    // Traverse the graph.
    // anchor -> find children, mark them as necessary and update the current
    // node as its parent. Repeat the same for each child
    for (auto& anchor : anchors) {
        observe(anchor);
    }
}

template <typename T>
void Engine::unobserve(std::shared_ptr<AnchorWrap<T>>& anchor) {
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
