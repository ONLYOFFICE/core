#pragma once

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Timing.h"

#include "../Records/SlideProgTagsContainer.h"
#include "../PPTXWriter/ImageManager.h"
#include "../Records/Animations/AnimationInfoContainer.h"
#include <unordered_set>


namespace PPT {
namespace Intermediate {
struct Animation
{
    _INT32 shapeId = -1;
    CRecordAnimationInfoContainer* pAnimIC = nullptr;   // old animation records
    CRecordExtTimeNodeContainer* pETNCIC = nullptr;     // new animation records
};

// todo not correct (for example: triggers)!!!
using ParallelTimeNodes = std::list<Animation>;
using SequenceTimeNodes = std::list<ParallelTimeNodes>;

struct SlideAnimation
{
    SequenceTimeNodes sequences;
    CRecordBuildListContainer* pBLC = nullptr;
    std::unordered_set<_INT32> realShapesIds;
};
}
}
