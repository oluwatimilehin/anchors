#include <functional>
#include <memory>
#include <vector>

namespace anchors {
template <typename T>
class Anchor {
    // TODO: Attempt reading more rust code
   private:
    // PRIVATE TYPES
    int d_height;
    T   value;

    std::vector<std::shared_ptr<Anchor<T>>> children;
    // Anchors it depends on... For now, there can only be two but eventually we'll have more

    std::vector<std::shared_ptr<Anchor<T>>> parents;
    // Anchors that depend on it.

    // TODO: Store the parents and children, store the input values, store the updater.
    // TODO: Big question right now is how do we deal with Anchors of different types, i.e. storing them in a container

   public:
    using SingleInputUpdater = std::function<Anchor<T>(T)>;
    using DualInputUpdater   = std::function<Anchor<T>(T, T)>;

    Anchor(T value);

    T get();
    // Always return the latest value.

    void compute();
    // Update the value based on the inputs.

    int getHeight();

    void setValue(T value);

    void setUpdater(const std::function<Anchor<T>(T)>& updater);

    static Anchor<T> map(const std::shared_ptr<Anchor<T>>& anchor, const SingleInputUpdater& updater);

    static Anchor<T> map2(const std::shared_ptr<Anchor<T>>& anchor1, const std::shared_ptr<Anchor<T>>& anchor2, const DualInputUpdater& updater);

    // TODO: How do we store the updater given that there are two types. Do we two different variables? I think so, and then maybe a flag to indicate which one is which.
    // By next week, implement Anchor and State classes
};
}  // namespace anchors