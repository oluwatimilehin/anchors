#include "engine.h"

#include <sstream>
#include <stdexcept>

namespace {
template <typename T>
std::string toString(const T& value) {
    std::ostringstream ss;
    ss << value;
    return ss.str();
}
}  // namespace

namespace anchors {

Engine::Engine() : d_observedNodes(), d_recomputeHeap(), d_recomputeSet() {}

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

    for (auto& dep : current->getDependencies()) {
        observeNode(dep, visited);
        dep->addDependant(current);
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
    // Read from recompute heap to perform the calculations
    // When we implement set_updater, we also read from the adjust_heights heap
    /**
     *  - Increment stabilization number.
     * 	- It picks a node at a time from the recompute heap in a three step
     process: ○ Remove the node with the smallest height from the heap ○
     Recompute it
                    - If its value changed (perhaps according to the cut-off
     function), add the nodes that depend on it (parents) to the heap.
        -
        - Set the recompute ID of each calculated node to the current
     stabilization number
     *
     */
    if (d_recomputeHeap.empty()) {
        return;
    }

    d_stabilizationNumber++;

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
                if (dependant->isStale() &&
                    !d_recomputeSet.contains(dependant)) {
                    d_recomputeHeap.push(dependant);
                    // The parents should always have a higher
                    // height than a child, so this shouldn't
                    // cause any issues when we pop the heap
                }
            }
        }
    }

    d_recomputeSet.clear();
}

}  // namespace anchors