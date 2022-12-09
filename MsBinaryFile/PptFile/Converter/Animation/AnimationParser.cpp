#include "AnimationParser.h"
#include "../../Records/Drawing/ShapeContainer.h"
#include "Timing_1995.h"

namespace PPT 
{
	namespace Intermediate 
	{
		CRecordPP10SlideBinaryTagExtension* getPP10SlideBinaryTagExtension(CRecordSlide *pSlide)
		{
			CRecordSlideProgTagsContainer* progTag = pSlide->m_pSlideProgTagsContainer;
			return progTag ? progTag->getPP10SlideBinaryTagExtension() : nullptr;
		}

		std::vector<SOldAnimation> getOldSlideAnimation(CRecordSlide *pSlide)
		{
			std::vector<CRecordShapeContainer*> arrShapeCont;
			pSlide->GetRecordsByType(&arrShapeCont, true);

			std::vector<SOldAnimation> listOfRawAnimIC;
			for (auto* pShapeCont : arrShapeCont)
			{
				std::vector<CRecordShape* > shape;
				pShapeCont->GetRecordsByType(&shape, true);
				std::vector<CRecordAnimationInfoContainer* > anim;
				pShapeCont->GetRecordsByType(&anim, true);
				SOldAnimation animIC;
				if (!anim.empty() && !shape.empty())
				{
					animIC.shapeId = shape[0]->m_nID;
					animIC.anim = anim[0];
					listOfRawAnimIC.push_back(animIC);
				}
			}

			return listOfRawAnimIC;
		}
		SlideAnimation ParseSlideAnimation(CRecordSlide *pSlide)
		{
			SlideAnimation slideAnim;
			slideAnim.arrAnim_1995 = getOldSlideAnimation(pSlide);
			slideAnim.pAnim_2010 = getPP10SlideBinaryTagExtension(pSlide);

			return slideAnim;
		}
	}
}
