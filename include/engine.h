// engine.h
#ifndef ANCHORS_ENGINE_H
#define ANCHORS_ENGINE_H

#include "anchor.h"
#include "anchorutil.h"

#include <memory>
#include <queue>
#include <unordered_set>

namespace anchors {

/**
 * Engine is the brain of %Anchors, containing the necessary functions and data to
 * retrieve the value of an `Anchor` object. Note that this class is not
 * thread-safe.
 */
class Engine {
   public:
    /**
     * Creates an instance of the Engine class.
     */
    Engine();

    /**
     * Returns the value of the given Anchor. This function is only guaranteed
     * to return the latest value of an Anchor marked observed using
     * `observe()`.
     *
     * For an unobserved Anchor, it may return a stale value—if the Anchor was
     * created with a value or has been computed before—or an undefined value if
     * the Anchor was created with a function e.g. using `Anchors::map`, and has
     * not been computed before.
     *
     * @tparam T - type of the Anchor value.
     * @param anchor - input Anchor.
     * @return the current value of the input Anchor.
     */
    template <typename T>
    T get(const AnchorPtr<T>& anchor);

    /**
     * Sets the value of the given Anchor. If the provided value is different
     * from the current value of the Anchor, any observed Anchors that depends
     * on the given Anchor will return an up-to-date value when its value is
     * retrieved using `get()`.
     *
     * @tparam T - type of the Anchor value.
     * @param anchor - input Anchor.
     * @param val - new value of the Anchor.
     */
    template <typename T>
    void set(AnchorPtr<T>& anchor, T val);

    /**
     * Marks an Anchor as observed. An observed Anchor is guaranteed to be up to
     * date when you retrieve its value.
     *
     * @tparam T - type of the Anchor value.
     * @param anchor - input Anchor.
     */
    template <typename T>
    void observe(AnchorPtr<T>& anchor);

    /**
     * Marks a vector of Anchors with the same type as observed.
     *
     * @tparam T - type of the Anchors.
     * @param anchors - input Anchors.
     */
    template <typename T>
    void observe(std::vector<AnchorPtr<T>>& anchors);

    /**
     * Marks an Anchor as unobserved.
     * @tparam T - type of the Anchor value.
     * @param anchor - input Anchor.
     */
    template <typename T>
    void unobserve(AnchorPtr<T>& anchor);

   private:
    // PRIVATE MANIPULATORS
    void stabilize();
    // Brings all observed Anchors up-to-date.

    void observeNode(std::shared_ptr<AnchorBase>& current,
                     std::unordered_set<std::shared_ptr<AnchorBase>>&);
    // Marks all the dependencies of the given Anchor as necessary and adds
    // stale Anchors to the recompute heap;

    void unobserveNode(std::shared_ptr<AnchorBase>& current);
    // Removes `current` from the set of observed Anchors.
    // Also decrements the 'necessary' count and removes `current` as a
    // dependant of each of its dependencies.

    // PRIVATE DATA
    int d_stabilizationNumber;
    // Current stabilization number of the engine. We use this number to
    // represent when an Anchor value was recomputed and/or changed.

    std::unordered_set<std::shared_ptr<AnchorBase>> d_observedNodes;
    // Set of observed Anchors.

    std::priority_queue<std::shared_ptr<AnchorBase>> d_recomputeHeap;
    // Priority queue containing Anchors that need to be recomputed, in
    // increasing order of their heights.

    std::unordered_set<std::shared_ptr<AnchorBase>> d_recomputeSet;
    // Set of Anchors present in the recompute queue.

    //    std::priority_queue<std::shared_ptr<AnchorBase>> d_adjustHeightsHeap;
    //    // Update the adjust-heights heap when setUpdater is called. Will use
    //    later.
};

template <typename T>
T Engine::get(const AnchorPtr<T>& anchor) {
    if (d_observedNodes.contains(anchor)) {
        stabilize();
    }

    return anchor->get();
}

template <typename T>
void Engine::set(AnchorPtr<T>& anchor, T val) {
    T oldVal = anchor->get();

    if (oldVal == val) return;
    d_stabilizationNumber++;

    anchor->setChangeId(d_stabilizationNumber);
    anchor->set(val);

    if (anchor->isNecessary()) {
        for (const auto& dependant : anchor->getDependants()) {
            if (dependant->isNecessary() && !d_recomputeSet.contains(dependant)) {
                d_recomputeHeap.push(dependant);
                d_recomputeSet.insert(dependant);
            }
        }
    }
}

template <typename T>
void Engine::observe(AnchorPtr<T>& anchor) {
    if (d_observedNodes.contains(anchor)) {
        return;
    }

    d_observedNodes.insert(anchor);

    std::unordered_set<std::shared_ptr<AnchorBase>> visited;
    std::shared_ptr<AnchorBase>                     anchorBase = anchor;
    observeNode(anchorBase, visited);
}

template <typename T>
void Engine::observe(std::vector<AnchorPtr<T>>& anchors) {
    for (auto& anchor : anchors) {
        observe(anchor);
    }
}

template <typename T>
void Engine::unobserve(AnchorPtr<T>& anchor) {
    if (!d_observedNodes.contains(anchor)) {
        return;
    }

    d_observedNodes.erase(anchor);

    std::shared_ptr<AnchorBase> anchorBase = anchor;
    unobserveNode(anchorBase);
}

}  // namespace anchors
#endif
