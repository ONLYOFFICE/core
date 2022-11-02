#pragma once

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Timing.h"

#include "../Records/SlideProgTagsContainer.h"
#include "../PPTXWriter/ImageManager.h"
#include "../Records/Animations/AnimationInfoContainer.h"
#include <unordered_set>
#include "TimeNode.h"


namespace PPT {
namespace Intermediate {
struct Animation
{
    _INT32 shapeId = -1;
    CRecordAnimationInfoContainer* pAnimIC = nullptr;   // old animation records
    CRecordExtTimeNodeContainer* pETNCIC = nullptr;     // new animation records
};


struct SimpleBuildNode
{
    _INT32 spid = -1;
    bool grpId = false;
    bool animBg = false;
};
using SimpleBuildList = std::vector<SimpleBuildNode>;


struct SlideAnimation
{
    std::unique_ptr<TimeNode> rootNode;
    SimpleBuildList oldBuildList;
    CRecordBuildListContainer* pBLC = nullptr;
    std::unordered_set<_INT32> realShapesIds;
};
}
}
