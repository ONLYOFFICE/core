#include "AnimationParser.h"
#include "../../Records/Drawing/ShapeContainer.h"
#include "OldAnimationParser.h"

namespace PPT {
namespace Intermediate {

class AnimationParser
{
public:
    AnimationParser(CRecordSlide *pSlide) :
        pSlide(pSlide)
    {
        InitListOfRawAnimIC();
        InitPP10SlideBinaryTagExtension();
    }
    SlideAnimation Parse()
    {
        if(HasAnySlideAnimation() == false)
            return {};

        InitAnimationTree();
        return std::move(slideAnim);
    }

private:
    void InitPP10SlideBinaryTagExtension()
    {
        CRecordSlideProgTagsContainer* progTag = pSlide->m_pSlideProgTagsContainer;
        pAnimExt = progTag ? progTag->getPP10SlideBinaryTagExtension() : nullptr;
    }
    void InitListOfRawAnimIC()
    {
        std::vector<CRecordShapeContainer*> arrShapeCont;
        pSlide->GetRecordsByType(&arrShapeCont, true);

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
                listOfOldAnim.push_back(animIC);
            }
        }
    }
    bool HasAnySlideAnimation() const
    {
        return listOfOldAnim.size() || pAnimExt;
    }
    void InitAnimationTree()
    {
        InitOldAnimationTree();
        if (pAnimExt == nullptr)
            return;
        // todo modern animation
    }
    void InitOldAnimationTree()
    {
        OldAnimationParser parser(slideAnim, listOfOldAnim);
        // todo parser
        parser.Parse();
    };

private:
    CRecordSlide* pSlide;
    SlideAnimation slideAnim;
    std::list<Animation> listOfOldAnim;
    CRecordPP10SlideBinaryTagExtension* pAnimExt = nullptr;
};

SlideAnimation ParseSlideAnimation(CRecordSlide *pSlide)
{
    AnimationParser parser(pSlide);
    auto slideAnim= parser.Parse();
    return slideAnim;
}
}
}
