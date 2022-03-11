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

        // TODO: Ideally, we would store the old value to determine whether or not we need to add the children but we can't access T from AnchorBase. Perhaps we can use the change Id?
        // Maybe pass the stabilization number to compute() to set it as its change ID and recompute ID and only update the change ID when the value actually changes?
        top->compute(d_stabilizationNumber);

        if (top->getChangeId() == d_stabilizationNumber) {
            // Its value changed. Maybe support a cut-off function later
            for (const auto& parent : top->getParents()) {
                d_recomputeHeap.push(parent);  // The parents should always have a higher height than a child, so this shouldn't cause any issues when we pop the heap
            }
        }

        d_recomputeHeap.pop();
    }
}

}  // namespace anchors