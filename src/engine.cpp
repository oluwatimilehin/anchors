#include "../include/engine.h"

namespace anchors {

Engine::Engine()
    : d_stabilizationNumber(0),
      d_observedNodes(),
      d_recomputeHeap(),
      d_recomputeSet() {}

void Engine::observeNode(
    std::shared_ptr<AnchorBase>&                     current,
    std::unordered_set<std::shared_ptr<AnchorBase>>& visited) {
    if (visited.contains(current)) {
        return;
    }

    visited.insert(current);
    current->markNecessary();

    if (current->isStale() && !d_recomputeSet.contains(current)) {
        d_recomputeHeap.push(current);
    }

    // Repeat the same for all its dependencies
    for (auto& dep : current->getDependencies()) {
        dep->addDependant(current);
        observeNode(dep, visited);
    }
}

void Engine::unobserveNode(std::shared_ptr<AnchorBase>& current) {
    current->decrementNecessaryCount();

    for (auto& dep : current->getDependencies()) {
        unobserveNode(dep);
        dep->removeDependant(current);
    }
}

void Engine::stabilize() {
    // In the future, we might first need to adjust_heights.
    if (d_recomputeHeap.empty()) {
        return;
    }

    d_stabilizationNumber++;
    // Stabilization is a three-step process:
    // - Remove the node with the smallest height from the recompute heap
    // - Recompute it.
    // - If its value changed, add the nodes that depend on it to the heap
    while (!d_recomputeHeap.empty()) {
        std::shared_ptr<AnchorBase> top = d_recomputeHeap.top();
        d_recomputeHeap.pop();

        if (!top->isStale()) {
            continue;
        }

        top->compute(d_stabilizationNumber);

        if (top->getChangeId() == d_stabilizationNumber) {
            // Its value changed.
            for (const auto& dependant : top->getDependants()) {
                if (!d_recomputeSet.contains(dependant)) {
                    d_recomputeHeap.push(dependant);
                }
            }
        }
    }

    d_recomputeSet.clear();
}

}  // namespace anchors
