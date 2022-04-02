//
// Created by Timi Adeniran on 23/03/2022.
//
#ifndef ANCHORS_ANCHORS_H
#define ANCHORS_ANCHORS_H

#include "anchor.h"
// TODO: Maybe rename this file

namespace anchors {

template <typename T>
using AnchorPtr = std::shared_ptr<AnchorWrap<T>>;

class Anchors {
   public:
    template <typename T>
    static AnchorPtr<T> create(const T& value);

    template <typename T, typename InputType1 = T>
    static AnchorPtr<T> map(
        const AnchorPtr<InputType1>&                              anchor,
        const typename Anchor<T, InputType1>::SingleInputUpdater& updater);

    template <typename T, typename InputType1 = T>
    static AnchorPtr<T> map(
        const AnchorPtr<InputType1>&                               anchor,
        const typename Anchor<T, InputType1>::SingleInputUpdater&& updater);

    template <typename T, typename InputType1 = T, typename InputType2 = T>
    static AnchorPtr<T> map2(
        const AnchorPtr<InputType1>& anchor1,
        const AnchorPtr<InputType2>& anchor2,
        const typename Anchor<T, InputType1, InputType2>::DualInputUpdater&
            updater);

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
