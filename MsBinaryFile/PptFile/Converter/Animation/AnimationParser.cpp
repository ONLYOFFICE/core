/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
