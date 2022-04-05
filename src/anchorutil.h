// anchorutil.h
#ifndef ANCHORS_ANCHORS_H
#define ANCHORS_ANCHORS_H

#include "anchor.h"

namespace anchors {

template <typename T>
using AnchorPtr = std::shared_ptr<AnchorWrap<T>>;

/**
 * Utility class for creating a shared pointer to an Anchor.
 */
class Anchors {
   public:
    /**
     * Creates an Anchor containing the given value.
     *
     * @tparam T - Anchor type. `T` should overload the equality and output
     * operators if not already defined.
     * @param value - initial value of the Anchor.
     * @return a shared pointer to the created Anchor.
     */
    template <typename T>
    static AnchorPtr<T> create(const T& value);

    /**
     * Creates an Anchor from an input Anchor.
     *
     * @tparam T - type of the output Anchor. `T` should overload the
     * equality and output operators if not already defined.
     * @tparam InputType1 - optional type of the input Anchor. Only
     * specify this type if it is different from the output Anchor type.
     * @param anchor - input Anchor
     * @param updater - function that maps the input Anchor to the output.
     * Accepts an Lvalue reference.
     * @return a shared pointer to the created Anchor
     */
    template <typename T, typename InputType1 = T>
    static AnchorPtr<T> map(
        const AnchorPtr<InputType1>&                              anchor,
        const typename Anchor<T, InputType1>::SingleInputUpdater& updater);

    /**
     * Creates an Anchor from an input Anchor.
     *
     * @tparam T - type of the output Anchor. `T` should overload the
     * equality and output operators if not already defined.
     * @tparam InputType1 - optional type of the input Anchor. Only
     * specify this type if it is different from the output Anchor type.
     * @param anchor - input Anchor
     * @param updater - function that maps the input Anchor to the output.
     * Accepts an Rvalue reference.
     * @return a shared pointer to the created Anchor
     */
    template <typename T, typename InputType1 = T>
    static AnchorPtr<T> map(
        const AnchorPtr<InputType1>&                               anchor,
        const typename Anchor<T, InputType1>::SingleInputUpdater&& updater);

    /**
     * Creates an Anchor from two input Anchors.
     *
     * @tparam T - type of the output Anchor. `T` should overload the
     * equality and output operators if not already defined.
     * @tparam InputType1 - optional type of the first input Anchor. Only
     * specify this type if it is different from the output Anchor type.
     * @tparam InputType2 - optional type of the second input Anchor. Only
     * specify this type if it is different from the output Anchor type.
     * @param anchor1 - first input Anchor.
     * @param anchor2 - second input Anchor.
     * @param updater - function that maps the input Anchors to the output.
     * Accepts an Lvalue reference.
     * @return a shared pointer to the created Anchor.
     */
    template <typename T, typename InputType1 = T, typename InputType2 = T>
    static AnchorPtr<T> map2(
        const AnchorPtr<InputType1>& anchor1,
        const AnchorPtr<InputType2>& anchor2,
        const typename Anchor<T, InputType1, InputType2>::DualInputUpdater&
            updater);

    /**
     * Creates an Anchor from two input Anchors.
     *
     * @tparam T - type of the output Anchor. `T` should overload the
     * equality and output operators if not already defined.
     * @tparam InputType1 - optional type of the first input Anchor. Only
     * specify this type if it is different from the output Anchor type.
     * @tparam InputType2 - optional type of the second input Anchor. Only
     * specify this type if it is different from the output Anchor type.
     * @param anchor1 - first input Anchor.
     * @param anchor2 - second input Anchor.
     * @param updater - function that maps the input Anchors to the output.
     * Accepts an Rvalue reference.
     * @return a shared pointer to the created Anchor.
     */
    template <typename T, typename InputType1 = T, typename InputType2 = T>
    static AnchorPtr<T> map2(
        const AnchorPtr<InputType1>& anchor1,
        const AnchorPtr<InputType2>& anchor2,
        const typename Anchor<T, InputType1, InputType2>::DualInputUpdater&&
            updater);
};

template <typename T>
AnchorPtr<T> Anchors::create(const T& value) {
    AnchorPtr<T> newAnchor(std::make_shared<Anchor<T>>(value));

    return newAnchor;
}

template <typename T, typename InputType1>
AnchorPtr<T> Anchors::map(
    const AnchorPtr<InputType1>&                              anchor,
    const typename Anchor<T, InputType1>::SingleInputUpdater& updater) {
    AnchorPtr<T> newAnchor(
        std::make_shared<Anchor<T, InputType1>>(anchor, updater));

    return newAnchor;
}

template <typename T, typename InputType1>
AnchorPtr<T> Anchors::map(
    const AnchorPtr<InputType1>&                               anchor,
    const typename Anchor<T, InputType1>::SingleInputUpdater&& updater) {
    return map<T, InputType1>(anchor, updater);
}

template <typename T, typename InputType1, typename InputType2>
AnchorPtr<T> Anchors::map2(
    const AnchorPtr<InputType1>& anchor1,
    const AnchorPtr<InputType2>& anchor2,
    const typename Anchor<T, InputType1, InputType2>::DualInputUpdater&
        updater) {
    AnchorPtr<T> newAnchor(std::make_shared<Anchor<T, InputType1, InputType2>>(
        anchor1, anchor2, updater));

    return newAnchor;
}

template <typename T, typename InputType1, typename InputType2>
AnchorPtr<T> Anchors::map2(
    const AnchorPtr<InputType1>& anchor1,
    const AnchorPtr<InputType2>& anchor2,
    const typename Anchor<T, InputType1, InputType2>::DualInputUpdater&&
        updater) {
    return map2<T, InputType1, InputType2>(anchor1, anchor2, updater);
}

}  // namespace anchors
#endif  // ANCHORS_ANCHORS_H
