#ifndef ANCHORS_ANCHORBASE_H
#define ANCHORS_ANCHORBASE_H

#pragma comment(lib, "bcrypt.lib")  // TODO, will this fail on a linux machine?
                                    // It fixes a boost::uuid linker issue

#include <boost/uuid/uuid.hpp>
#include <memory>
#include <unordered_set>
#include <vector>

namespace anchors {
/***
 `AnchorBase` represents an Anchor without its type information, which allows us
 store Anchors of different types in a container.

 The functions in this class are implemented as private functions and only
 visible to the Engine class.
 ***/
class AnchorBase {
   public:
    using AnchorId = boost::uuids::uuid;

    virtual ~AnchorBase(){};

    virtual void compute(int stabilizationNumber) = 0;

    virtual AnchorId getId() const = 0;

    virtual int getHeight() const = 0;

    virtual int getRecomputeId() const = 0;

    virtual int getChangeId() const = 0;

    virtual void setChangeId(int changeId) = 0;

    virtual void markNecessary() = 0;

    virtual void decrementNecessaryCount() = 0;

    virtual bool isNecessary() const = 0;

    virtual bool isStale() const = 0;

    virtual std::unordered_set<std::shared_ptr<AnchorBase>> getDependants()
        const = 0;

    virtual std::vector<std::shared_ptr<AnchorBase>> getDependencies()
        const = 0;

    virtual void addDependant(const std::shared_ptr<AnchorBase>& parent) = 0;

    virtual void removeDependant(const std::shared_ptr<AnchorBase>& parent) = 0;
};
}  // namespace anchors

#endif  // ANCHORS_ANCHORBASE_H
