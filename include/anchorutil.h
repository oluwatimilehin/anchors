// anchorutil.h
#ifndef ANCHORS_ANCHORS_H
#define ANCHORS_ANCHORS_H

#include "anchor.h"

/**
 * Main library namespace
 */
namespace anchors {
/**
 * Alias representing a shared pointer to an Anchor of output type `T`.
 */
template <typename T>
using AnchorPtr = std::shared_ptr<AnchorWrap<T>>;

/**
 * Anchors is an utility class containing functions to simplify creating a
 * shared pointer to an Anchor, which the Engine class operates on.
 *
 * To create shared pointers with custom allocators, you can invoke the public
 * constructors in the `Anchor` class.
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
    static AnchorPtr<T> create(const T &value);

    /**
     * Creates an Anchor from an input Anchor.
     *
     * @tparam T - type of the output Anchor. `T` should overload the
     * equality and output operators if not already defined.
     * @tparam InputType1 - optional type of the input Anchor. Required only if
     * this type is different from the output Anchor Type T.
     * @param anchor - input Anchor
     * @param updater - function that maps the input Anchor to the output.
     * Accepts an Lvalue reference.
     * @return a shared pointer to the created Anchor
     */
    template <typename T, typename InputType1 = T>
    static AnchorPtr<T> map(
        const AnchorPtr<InputType1>                              &anchor,
        const typename Anchor<T, InputType1>::SingleInputUpdater &updater);

    /**
     * Creates an Anchor from an input Anchor.
     *
     * @tparam T - type of the output Anchor. `T` should overload the
     * equality and output operators if not already defined.
     * @tparam InputType1 - optional type of the input Anchor. Required only if
     * this type is different from the output Anchor Type T.
     * @param anchor - input Anchor
     * @param updater - function that maps the input Anchor to the output.
     * Accepts an Rvalue reference.
     * @return a shared pointer to the created Anchor
     */
    template <typename T, typename InputType1 = T>
    static AnchorPtr<T> map(
        const AnchorPtr<InputType1>                               &anchor,
        const typename Anchor<T, InputType1>::SingleInputUpdater &&updater);

    /**
     * Creates an Anchor from two input Anchors.
     *
     * @tparam T - type of the output Anchor. `T` should overload the
     * equality and output operators if not already defined.
     * @tparam InputType1 - optional type of the first input Anchor. Required
     * only if this type is different from the output Anchor Type T.
     * @tparam InputType2 - optional type of the second input Anchor. Required
     * only if this type is different from the output Anchor Type T.
     * @param anchor1 - first input Anchor.
     * @param anchor2 - second input Anchor.
     * @param updater - function that maps the input Anchors to the output.
     * Accepts an Lvalue reference.
     * @return a shared pointer to the created Anchor.
     */
    template <typename T, typename InputType1 = T, typename InputType2 = T>
    static AnchorPtr<T> map2(
        const AnchorPtr<InputType1> &anchor1,
        const AnchorPtr<InputType2> &anchor2,
        const typename Anchor<T, InputType1, InputType2>::DualInputUpdater
            &updater);

    /**
     * Creates an Anchor from two input Anchors.
     *
     * @tparam T - type of the output Anchor. `T` should overload the
     * equality and output operators if not already defined.
     * @tparam InputType1 - optional type of the first input Anchor. Required
     * only if this type is different from the output Anchor Type T.
     * @tparam InputType2 - optional type of the second input Anchor. Required
     * only if this type is different from the output Anchor Type T.
     * @param anchor1 - first input Anchor.
     * @param anchor2 - second input Anchor.
     * @param updater - function that maps the input Anchors to the output.
     * Accepts an Rvalue reference.
     * @return a shared pointer to the created Anchor.
     */
    template <typename T, typename InputType1 = T, typename InputType2 = T>
    static AnchorPtr<T> map2(
        const AnchorPtr<InputType1> &anchor1,
        const AnchorPtr<InputType2> &anchor2,
        const typename Anchor<T, InputType1, InputType2>::DualInputUpdater
            &&updater);

    /**
     *  Creates an Anchor from three input Anchors
     * @tparam T - type of the output Anchor. `T` should overload the
     * equality and output operators if not already defined.
     * @tparam InputType1 - optional type of the first input Anchor. Required
     * only if this type is different from the output Anchor Type T.
     * @tparam InputType2 - optional type of the second input Anchor. Required
     * only if this type is different from the output Anchor Type T.
     * @tparam InputType3 - optional type of the third input Anchor. Required
     * only if this type is different from the output Anchor Type T.
     * @param anchor1 - first input Anchor.
     * @param anchor2 - second input Anchor.
     * @param anchor3 - third input Anchor.
     * @param updater - function that maps the input Anchors to the output.
     * Accepts a Lvalue reference.
     * @return a shared pointer to the created Anchor
     */
    template <typename T,
              typename InputType1 = T,
              typename InputType2 = T,
              typename InputType3 = T>
    static AnchorPtr<T> map3(
        const AnchorPtr<InputType1> &anchor1,
        const AnchorPtr<InputType2> &anchor2,
        const AnchorPtr<InputType3> &anchor3,
        const std::function<T(InputType1 &, InputType2 &, InputType3 &)>
            &updater);

    /**
     *  Creates an Anchor from three input Anchors
     * @tparam T - type of the output Anchor. `T` should overload the
     * equality and output operators if not already defined.
     * @tparam InputType1 - optional type of the first input Anchor. Required
     * only if this type is different from the output Anchor Type T.
     * @tparam InputType2 - optional type of the second input Anchor. Required
     * only if this type is different from the output Anchor Type T.
     * @tparam InputType3 - optional type of the third input Anchor. Required
     * only if this type is different from the output Anchor Type T.
     * @param anchor1 - first input Anchor.
     * @param anchor2 - second input Anchor.
     * @param anchor3 - third input Anchor.
     * @param updater - function that maps the input Anchors to the output.
     * Accepts a Rvalue reference.
     * @return a shared pointer to the created Anchor
     */
    template <typename T,
              typename InputType1 = T,
              typename InputType2 = T,
              typename InputType3 = T>
    static AnchorPtr<T> map3(
        const AnchorPtr<InputType1> &anchor1,
        const AnchorPtr<InputType2> &anchor2,
        const AnchorPtr<InputType3> &anchor3,
        const std::function<T(InputType1 &, InputType2 &, InputType3 &)>
            &&updater);

    /**
     *  Creates an Anchor from four input Anchors
     * @tparam T - type of the output Anchor. `T` should overload the
     * equality and output operators if not already defined.
     * @tparam InputType1 - optional type of the first input Anchor. Required
     * only if this type is different from the output Anchor Type T.
     * @tparam InputType2 - optional type of the second input Anchor. Required
     * only if this type is different from the output Anchor Type T.
     * @tparam InputType3 - optional type of the third input Anchor. Required
     * only if this type is different from the output Anchor Type T.
     * @tparam InputType4 - optional type of the fourth input Anchor. Required
     * only if this type is different from the output Anchor Type T.
     * @param anchor1 - first input Anchor.
     * @param anchor2 - second input Anchor.
     * @param anchor3 - third input Anchor.
     * @param anchor4 - fourth input Anchor.
     * @param updater - function that maps the input Anchors to the output.
     * Accepts a Lvalue reference.
     * @return a shared pointer to the created Anchor
     */
    template <typename T,
              typename InputType1 = T,
              typename InputType2 = T,
              typename InputType3 = T,
              typename InputType4 = T>
    static AnchorPtr<T> map4(
        const AnchorPtr<InputType1> &anchor1,
        const AnchorPtr<InputType2> &anchor2,
        const AnchorPtr<InputType3> &anchor3,
        const AnchorPtr<InputType4> &anchor4,
        const std::function<
            T(InputType1 &, InputType2 &, InputType3 &, InputType4 &)>
            &updater);

    /**
     *  Creates an Anchor from four input Anchors
     * @tparam T - type of the output Anchor. `T` should overload the
     * equality and output operators if not already defined.
     * @tparam InputType1 - optional type of the first input Anchor. Required
     * only if this type is different from the output Anchor Type T.
     * @tparam InputType2 - optional type of the second input Anchor. Required
     * only if this type is different from the output Anchor Type T.
     * @tparam InputType3 - optional type of the third input Anchor. Required
     * only if this type is different from the output Anchor Type T.
     * @tparam InputType4 - optional type of the fourth input Anchor. Required
     * only if this type is different from the output Anchor Type T.
     * @param anchor1 - first input Anchor.
     * @param anchor2 - second input Anchor.
     * @param anchor3 - third input Anchor.
     * @param anchor4 - fourth input Anchor.
     * @param updater - function that maps the input Anchors to the output.
     * Accepts a Rvalue reference.
     * @return a shared pointer to the created Anchor
     */
    template <typename T,
              typename InputType1 = T,
              typename InputType2 = T,
              typename InputType3 = T,
              typename InputType4 = T>
    static AnchorPtr<T> map4(
        const AnchorPtr<InputType1> &anchor1,
        const AnchorPtr<InputType2> &anchor2,
        const AnchorPtr<InputType3> &anchor3,
        const AnchorPtr<InputType4> &anchor4,
        const std::function<
            T(InputType1 &, InputType2 &, InputType3 &, InputType4 &)>
            &&updater);
};

template <typename T>
AnchorPtr<T> Anchors::create(const T &value) {
    AnchorPtr<T> newAnchor(std::make_shared<Anchor<T>>(value));

    return newAnchor;
}

template <typename T, typename InputType1>
AnchorPtr<T> Anchors::map(
    const AnchorPtr<InputType1>                              &anchor,
    const typename Anchor<T, InputType1>::SingleInputUpdater &updater) {
    AnchorPtr<T> newAnchor(
        std::make_shared<Anchor<T, InputType1>>(anchor, updater));

    return newAnchor;
}

template <typename T, typename InputType1>
AnchorPtr<T> Anchors::map(
    const AnchorPtr<InputType1>                               &anchor,
    const typename Anchor<T, InputType1>::SingleInputUpdater &&updater) {
    return map<T, InputType1>(anchor, updater);
}

template <typename T, typename InputType1, typename InputType2>
AnchorPtr<T> Anchors::map2(
    const AnchorPtr<InputType1> &anchor1,
    const AnchorPtr<InputType2> &anchor2,
    const typename Anchor<T, InputType1, InputType2>::DualInputUpdater
        &updater) {
    AnchorPtr<T> newAnchor(std::make_shared<Anchor<T, InputType1, InputType2>>(
        anchor1, anchor2, updater));

    return newAnchor;
}

template <typename T, typename InputType1, typename InputType2>
AnchorPtr<T> Anchors::map2(
    const AnchorPtr<InputType1> &anchor1,
    const AnchorPtr<InputType2> &anchor2,
    const typename Anchor<T, InputType1, InputType2>::DualInputUpdater
        &&updater) {
    return map2<T, InputType1, InputType2>(anchor1, anchor2, updater);
}

template <typename T,
          typename InputType1,
          typename InputType2,
          typename InputType3>
AnchorPtr<T> Anchors::map3(
    const AnchorPtr<InputType1>                                      &anchor1,
    const AnchorPtr<InputType2>                                      &anchor2,
    const AnchorPtr<InputType3>                                      &anchor3,
    const std::function<T(InputType1 &, InputType2 &, InputType3 &)> &updater) {
    using PairType = std::pair<InputType1, InputType2>;

    const auto &anchorOfPair(map2<PairType, InputType1, InputType2>(
        anchor1, anchor2, [](InputType1 t1, InputType2 t2) {
            return std::make_pair(t1, t2);
        }));

    const auto &newUpdater = [updater](PairType &pair, InputType3 &anchor3) {
        return updater(pair.first, pair.second, anchor3);
    };

    return map2<T, PairType, InputType3>(anchorOfPair, anchor3, newUpdater);
}

template <typename T,
          typename InputType1,
          typename InputType2,
          typename InputType3>
AnchorPtr<T> Anchors::map3(
    const AnchorPtr<InputType1> &anchor1,
    const AnchorPtr<InputType2> &anchor2,
    const AnchorPtr<InputType3> &anchor3,
    const std::function<T(InputType1 &, InputType2 &, InputType3 &)>
        &&updater) {
    return map3<T, InputType1, InputType2, InputType3>(
        anchor1, anchor2, anchor3, updater);
}

template <typename T,
          typename InputType1,
          typename InputType2,
          typename InputType3,
          typename InputType4>
AnchorPtr<T> Anchors::map4(
    const AnchorPtr<InputType1> &anchor1,
    const AnchorPtr<InputType2> &anchor2,
    const AnchorPtr<InputType3> &anchor3,
    const AnchorPtr<InputType4> &anchor4,
    const std::function<
        T(InputType1 &, InputType2 &, InputType3 &, InputType4 &)> &updater) {
    using PairType1 = std::pair<InputType1, InputType2>;
    using PairType2 = std::pair<InputType3, InputType4>;

    const auto &anchorOfPair1(map2<PairType1, InputType1, InputType2>(
        anchor1, anchor2, [](InputType1 t1, InputType2 t2) {
            return std::make_pair(t1, t2);
        }));

    const auto &anchorOfPair2(map2<PairType2, InputType3, InputType4>(
        anchor3, anchor4, [](InputType1 t1, InputType2 t2) {
            return std::make_pair(t1, t2);
        }));

    const auto &newUpdater = [updater](PairType1 &firstPair,
                                       PairType2 &secondPair) {
        return updater(firstPair.first,
                       firstPair.second,
                       secondPair.first,
                       secondPair.second);
    };

    return map2<T, PairType1, PairType2>(
        anchorOfPair1, anchorOfPair2, newUpdater);
}

template <typename T,
          typename InputType1,
          typename InputType2,
          typename InputType3,
          typename InputType4>
AnchorPtr<T> Anchors::map4(
    const AnchorPtr<InputType1> &anchor1,
    const AnchorPtr<InputType2> &anchor2,
    const AnchorPtr<InputType3> &anchor3,
    const AnchorPtr<InputType4> &anchor4,
    const std::function<
        T(InputType1 &, InputType2 &, InputType3 &, InputType4 &)> &&updater) {
    return map4<T, InputType1, InputType2, InputType3, InputType4>(
        anchor1, anchor2, anchor3, anchor4, updater);
}

}  // namespace anchors
#endif  // ANCHORS_ANCHORS_H
