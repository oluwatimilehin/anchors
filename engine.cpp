#include "engine.h"

namespace anchors {

Engine::Engine()
    : d_observedNodes(),
      d_recomputeHeap(),
      d_adjustHeightsHeap() {
}

void Engine::stabilize() {
    // Read from recompute heap to perform the calculations
    // When we implement set_updater, we also read from the adjust_heights heap
    /**
     *  - Increment stabilization number.
     * 	- It picks a node at a time from the recompute heap in a three step process:
                    ○ Remove the node with the smallest height from the heap
                    ○ Recompute it
                    - If its value changed (perhaps according to the cut-off function), add the nodes that depend on it (parents) to the heap.
        -
        - Set the recompute ID of each calculated node to the current stabilization number
     *
     */
    if (d_recomputeHeap.empty()) {
        return;
    }

    d_stabilizationNumber++;

    while (!d_recomputeHeap.empty()) {
        std::shared_ptr<AnchorBase> top = d_recomputeHeap.top();
        d_recomputeHeap.pop();

        if (!top->isNecessary()) {
            continue;
        }

        top->compute(d_stabilizationNumber);

        if (top->getChangeId() == d_stabilizationNumber) {
            // Its value changed.
            for (const auto& parent : top->getParents()) {
                if (parent->isNecessary()) {
                    d_recomputeHeap.push(parent);  // The parents should always have a higher height than a child, so this shouldn't cause any issues when we pop the heap
                }
            }
        }
    }
}

}  // namespace anchors