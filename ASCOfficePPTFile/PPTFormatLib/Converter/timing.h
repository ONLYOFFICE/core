#pragma once

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Timing.h"
#include "../PPTXWriter/ImageManager.h"
#include "Animation/intermediate_anim.h"


namespace PPT {
namespace Converter {
class Timing
{
public:
    Timing(const Intermediate::SlideAnimation& slideAnim);
    PPTX::Logic::Timing Convert(CExMedia* pExMedia, CRelsGenerator *pRels);

    bool HasAnimation() const;

private:
    const Intermediate::SlideAnimation& slideAnim;
//    CExMedia* pExMedia;
//    CRelsGenerator* pRels;

    PPTX::Logic::Timing timing;
};
}
}
