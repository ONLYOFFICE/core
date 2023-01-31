/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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

void AddElementMetadataToSlideAnim(SlideAnimation& anim, CElementPtr element)
{
    if (!element)
        return;

    auto id = element->m_lID;
    anim.realShapesIds.insert(id);

    auto type = element->m_etType;
    if (type == PPT::etPicture || type == PPT::etAudio || type == PPT::etVideo)
        anim.nonTextShapes.insert(id);
}

void ParseShapes(CSlide *pSlide, SlideAnimation& anim)
{
    CGroupElement *pGroupElement = !pSlide->m_arElements.empty() ? dynamic_cast<CGroupElement *>(pSlide->m_arElements[0].get()) : NULL;

    size_t start_index = 0;

    if (pGroupElement)
    {
        for (size_t i = 0; i < pGroupElement->m_pChildElements.size(); ++i)
            AddElementMetadataToSlideAnim(anim, pGroupElement->m_pChildElements[i]);

        start_index = 1;
    }

    for (size_t i = start_index; i < pSlide->m_arElements.size(); ++i)
        AddElementMetadataToSlideAnim(anim, pSlide->m_arElements[i]);

    return;
}
SlideAnimation ParseSlideAnimation(CRecordSlide *pSlideRecord, CSlide *pCSlide)
{
    SlideAnimation slideAnim;
    slideAnim.arrAnim_1995 = getOldSlideAnimation(pSlideRecord);
    slideAnim.pAnim_2010 = getPP10SlideBinaryTagExtension(pSlideRecord);
    ParseShapes(pCSlide, slideAnim);

    return slideAnim;
}
}
}
