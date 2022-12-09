#include "timing.h"

#include "Animation/intermediate_anim.h"
#include "Animation/Timing_1995.h"
#include "Animation/Timing_2010.h"
#include "Animation/TimingExeption.h"
#include "Animation/hashcode10.h"   // not work correct


using namespace PPT::Converter;

Timing::Timing(const PPT::Intermediate::SlideAnimation& slideAnim, const std::unordered_set<int> &shapesID) :
    slideAnim(slideAnim),
    shapesID(shapesID)
{}

PPTX::Logic::Timing Timing::Convert(PPT::CExMedia *pExMedia, CRelsGenerator *pRels)
{
    this->pExMedia = pExMedia;
    this->pRels = pRels;

    if (TryToConvertTiming2010() == false)
        if (TryToConvertTiming1995() == false)
            return {};

    if (timing.bldLst.IsInit() && timing.bldLst->list.empty())    // You can't leave an empty tag <p:bldLst/>
        timing.bldLst.reset();

    return std::move(timing);
}

bool Timing::HasAnimation() const
{
    if (slideAnim.arrAnim_1995.empty() && (slideAnim.pAnim_2010 == nullptr || slideAnim.pAnim_2010->m_haveExtTime == false))
        return false;
    else
        return true;
}

bool Timing::TryToConvertTiming2010()
{
    timing = PPTX::Logic::Timing();
    try {
        Timing_2010(slideAnim.pAnim_2010, shapesID).
                Convert(timing, pExMedia, pRels);
    } catch (const TimingExeption &ex) {
        return false;
    } catch (...) {
        return false;
    }
    return true;
}

bool Timing::TryToConvertTiming1995()
{
    timing = PPTX::Logic::Timing();
    try {
        Timing_1995(slideAnim.arrAnim_1995).
                Convert(timing, pExMedia, pRels);
    } catch (const TimingExeption &ex) {
        return false;
    } catch (...) {
        return false;
    }
    return true;
}

