#include <anchor.h>

#include <memory>
#include <queue>
#include <unordered_set>

namespace anchors {
class Engine {
   public:
    template <typename T>
    T get(const std::shared_ptr<Anchor<T>>& anchor);
    // If the node is observed (or necessary), call stabilize

    template <typename T>
    void set(const std::shared_ptr<Anchor<T>>& anchor, T val);
    // When we call set, if the node is necessary, get all its necessary children and add the stale ones to the recompute queue.

    template <typename T>
    void observe(const std::shared_ptr<Anchor<T>>& anchor);
    // when you observe a node, mark all its children as necessary

    template <typename T>
    void unobserve(const std::shared_ptr<Anchor<T>>& anchor);

   private:
    void stabilize();
    // Read from adjust heights heap to adjust all heights, then read from recompute heap to perform the calculations

    std::unordered_set<std::shared_ptr<AnchorBase>> observedNodes;
    // - observed nodes

    std::priority_queue<std::shared_ptr<AnchorBase>> recomputeHeap;
    //  Only add something to the recompute heap if it is necessary and stale.  A node is "stale" if it has never been computed or if its recompute id is less than the change id of one of its children.

    std::priority_queue<std::shared_ptr<AnchorBase>> adjustHeightsHeap;
    // Update the adjust-heights heap when setUpdater is called

    int stabilizationNumber;
    // - stabilization number
};
}  // namespace anchors
