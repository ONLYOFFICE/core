#include "timing.h"

#include "Animation/intermediate_anim.h"
#include "Animation/ppt10ext_animation.h"


using namespace PPT::Converter;

Timing::Timing(const PPT::Intermediate::SlideAnimation& slideAnim, const CExMedia* pExMedia, const CRelsGenerator* pRels) :
    slideAnim(slideAnim), pExMedia(pExMedia), pRels(pRels)
{

}

PPTX::Logic::Timing Timing::Convert()
{
    return std::move(timing);
}
