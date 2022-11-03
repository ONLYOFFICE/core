#pragma once

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Timing.h"

#include "../Records/SlideProgTagsContainer.h"
#include "../PPTXWriter/ImageManager.h"
#include "../Records/Animations/AnimationInfoContainer.h"
#include <unordered_set>


namespace PPT {
namespace Intermediate {
struct SOldAnimation
{
    _INT32 shapeId;
    CRecordAnimationInfoContainer* anim;

    _UINT32 getAnimDur() const;
};

struct SlideAnimation
{
    CRecordPP10SlideBinaryTagExtension* pAnim_2010 = nullptr;
    std::vector<SOldAnimation> arrAnim_1995;
    std::unordered_set<_INT32> realShapesIds;
};
}
}
