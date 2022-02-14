#include <functional>
#include <memory>
#include <vector>

namespace anchors {

class AnchorBase {
   public:
    virtual void compute() = 0;

    virtual int getHeight() = 0;

    virtual int getRecomputeId() = 0;

    virtual int getChangeId() = 0;
};

template <typename T>
class Anchor : public AnchorBase {
    // TODO: Attempt reading more rust code
   private:
    // PRIVATE TYPES
    int d_height;
    T   value;

    std::vector<std::shared_ptr<Anchor<T>>> children;
    // Anchors it depends on... For now, there can only be two but eventually we'll have more

    std::vector<std::shared_ptr<Anchor<T>>> parents;
    // Anchors that depend on it.

    int recomputeId;
    int changeId;

    // TODO: Store the input values, store the updater.

   public:
    using SingleInputUpdater = std::function<Anchor<T>(T)>;

    Anchor(T value);

    T get();
    // Always return the latest value.

    void compute() override;
    // Update the value based on the inputs.

    int getHeight() override;

    int getRecomputeId() override;

    int getChangeId() override;

    void setValue(T value);

    void setUpdater(const std::function<Anchor<T>(T)>& updater);

    static Anchor<T> map(const std::shared_ptr<Anchor<T>>& anchor, const SingleInputUpdater& updater);

    static Anchor<T> map2(const std::shared_ptr<Anchor<T>>& anchor1, const std::shared_ptr<Anchor<T>>& anchor2, const DualInputUpdater& updater);

    // TODO: How do we store the updater given that there are two types. Do we two different variables? I think so, and then maybe a flag to indicate which one is which.
    // By next week, implement Anchor and State classes
};
}  // namespace anchors