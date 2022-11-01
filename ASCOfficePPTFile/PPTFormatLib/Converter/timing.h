#pragma once

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Timing.h"
#include "../PPTXWriter/ImageManager.h"
#include "Animation/intermediate_anim.h"


namespace PPT {
namespace Converter {
class Timing
{
public:
    Timing(const PPT::Intermediate::SlideAnimation& slideAnim, const CExMedia* pExMedia, const CRelsGenerator* pRels);
    PPTX::Logic::Timing Convert();

    bool HasAnimation() const;

private:
    const PPT::Intermediate::SlideAnimation& slideAnim;
    const CExMedia* pExMedia;
    const CRelsGenerator* pRels;

    bool isPPT10Broken = true;
    PPTX::Logic::Timing timing;
};
}
}
