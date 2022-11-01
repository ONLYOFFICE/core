#include "animationparser.h"
#include "../../Records/Drawing/ShapeContainer.h"

using namespace PPT::Intermediate;

static CRecordPP10SlideBinaryTagExtension* getPP10SlideBinaryTagExtension(CRecordSlide *pSlide);
static std::list<Animation>                getListOfRawAnimIC(CRecordSlide *pSlide);


SlideAnimation ParseSlideAnimation(CRecordSlide *pSlide)
{
    SlideAnimation slideAnim;
    auto listOfOldAnim = getListOfRawAnimIC(pSlide);
    auto pAnimExt = getPP10SlideBinaryTagExtension(pSlide);

    return slideAnim;
}

CRecordPP10SlideBinaryTagExtension* getPP10SlideBinaryTagExtension(CRecordSlide *pSlide)
{
    CRecordSlideProgTagsContainer* progTag = pSlide->m_pSlideProgTagsContainer;
    return progTag ? progTag->getPP10SlideBinaryTagExtension() : nullptr;
}

std::list<Animation> getListOfRawAnimIC(CRecordSlide *pSlide)
{
    std::vector<CRecordShapeContainer*> arrShapeCont;
    pSlide->GetRecordsByType(&arrShapeCont, true);

    std::list<Animation> listOfRawAnimIC;
    for (auto* pShapeCont : arrShapeCont)
    {
        std::vector<CRecordShape* > shape;
        pShapeCont->GetRecordsByType(&shape, true);
        std::vector<CRecordAnimationInfoContainer* > anim;
        pShapeCont->GetRecordsByType(&anim, true);
        Animation animIC;
        if (!anim.empty() && !shape.empty())
        {
            animIC.shapeId = shape[0]->m_nID;
            animIC.pAnimIC = anim[0];
            listOfRawAnimIC.push_back(animIC);
        }
    }

    return listOfRawAnimIC;
}
