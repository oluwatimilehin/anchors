//
// Created by Timi Adeniran on 23/03/2022.
//
#ifndef ANCHORS_ANCHORUTIL_H
#define ANCHORS_ANCHORUTIL_H

#include "anchor.h"

namespace anchors {

template <typename T>
using AnchorPtr = std::shared_ptr<Anchor<T>>;

class AnchorUtil {
   public:
    template <typename T>
    static AnchorPtr<T> create(const T& value);

    template <typename T>
    static AnchorPtr<T> map(const AnchorPtr<T>& anchor, const typename Anchor<T>::SingleInputUpdater& updater);

    template <typename T>
    static AnchorPtr<T> map2(const AnchorPtr<T>& anchor1, const AnchorPtr<T>& anchor2, const typename Anchor<T>::DualInputUpdater& updater);
    // Set the children of the node.
};

template <typename T>
AnchorPtr<T> AnchorUtil::create(const T& value) {
    AnchorPtr<T> newAnchor(std::make_shared<Anchor<T>>(value));

    return newAnchor;
}
template <typename T>
AnchorPtr<T> AnchorUtil::map(const AnchorPtr<T>& anchor, const typename Anchor<T>::SingleInputUpdater& updater) {
    AnchorPtr<T> newAnchor(std::make_shared<Anchor<T>>());

    newAnchor->d_singleInputUpdater = updater;
    newAnchor->d_children.push_back(anchor);

    newAnchor->d_height = anchor->getHeight() + 1;

    return newAnchor;
}

template <typename T>
AnchorPtr<T> AnchorUtil::map2(const AnchorPtr<T>& anchor1, const AnchorPtr<T>& anchor2, const typename Anchor<T>::DualInputUpdater& updater) {
    AnchorPtr<T> newAnchor(std::make_shared<Anchor<T>>());

    newAnchor->d_dualInputUpdater = updater;
    newAnchor->d_children.push_back(anchor1);
    newAnchor->d_children.push_back(anchor2);

    int height1 = anchor1->getHeight();
    int height2 = anchor2->getHeight();

    newAnchor->d_height = height1 > height2 ? height1 + 1 : height2 + 1;
    return newAnchor;
}

}  // namespace anchors
#endif  // ANCHORS_ANCHORUTIL_H
