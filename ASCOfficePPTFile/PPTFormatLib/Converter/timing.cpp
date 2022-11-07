#include "timing.h"

#include "Animation/intermediate_anim.h"
#include "Animation/Timing_1995.h"


using namespace PPT::Converter;

Timing::Timing(const Intermediate::SlideAnimation& slideAnim) :
    slideAnim(slideAnim)
{}

PPTX::Logic::Timing Timing::Convert(CExMedia *pExMedia, CRelsGenerator *pRels)
{
//    this->pExMedia = pExMedia;
//    this->pRels = pRels;

    Timing_1995(slideAnim.arrAnim_1995).
            Convert(timing, pExMedia, pRels);
    return std::move(timing);
}
