//
// Created by Timi Adeniran on 10/03/2022.
//

#ifndef ANCHORS_ANCHORBASE_H
#define ANCHORS_ANCHORBASE_H

#include <memory>
#include <unordered_set>
#include <vector>

namespace anchors {
class AnchorBase {
   public:
    virtual ~AnchorBase(){};

    virtual void compute(int stabilizationNumber) = 0;

    virtual int getHeight()      = 0;
    virtual int getRecomputeId() = 0;

    virtual int getChangeId() = 0;

    virtual void setChangeId(int changeId) = 0;

    virtual void markNecessary() = 0;

    virtual void decrementNecessaryCount() = 0;

    virtual bool isNecessary() = 0;

    virtual bool isStale() = 0;

    virtual std::unordered_set<std::shared_ptr<AnchorBase>> getParents() = 0;

    virtual std::vector<std::shared_ptr<AnchorBase>> getChildren() = 0;

    virtual void addParent(const std::shared_ptr<AnchorBase>& parent) = 0;
};
}  // namespace anchors

#endif  // ANCHORS_ANCHORBASE_H
