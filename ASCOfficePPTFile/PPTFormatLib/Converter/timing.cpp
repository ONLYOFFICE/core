#include "timing.h"

#include "Animation/intermediate_anim.h"
#include "Animation/Timing_1995.h"
#include "Animation/Timing_2010.h"


using namespace PPT::Converter;

Timing::Timing(const Intermediate::SlideAnimation& slideAnim, const std::unordered_set<int> &shapesID) :
    slideAnim(slideAnim),
    shapesID(shapesID)
{}

PPTX::Logic::Timing Timing::Convert(CExMedia *pExMedia, CRelsGenerator *pRels)
{
//    this->pExMedia = pExMedia;
//    this->pRels = pRels;

    Timing_1995(slideAnim.arrAnim_1995).
            Convert(timing, pExMedia, pRels);
    Timing_2010(slideAnim.pAnim_2010, shapesID).
            Convert(timing, pExMedia, pRels);

    return std::move(timing);
}
