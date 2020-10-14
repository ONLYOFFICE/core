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
#pragma once

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/SchemeClr.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/SrgbClr.h"

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AnimClr.h"
#include "../../Records/Animations/ExtTimeNodeContainer.h"
#include "../../Records/Animations/TimeVariant4Behavior.h"

#include "CBhvr.h"


namespace PPT_FORMAT
{
// TODO
void FillAnimClr(
        CRecordExtTimeNodeContainer* pETNC,
        PPTX::Logic::AnimClr& oAnimClr)
{
    auto& clrAtom = pETNC->m_pTimeColorBehavior->m_oColorBehaviorAtom;

    FillCBhvr(pETNC, oAnimClr.cBhvr);

    // Write Attributes
    if (pETNC->m_pTimeColorBehavior->m_oBehavior.m_havePropertyList){
        for (auto pRec : pETNC->m_pTimeColorBehavior->m_oBehavior.m_pPropertyList->m_arRecords)
        {
            if (pRec->m_oHeader.RecInstance == TL_TBPID_ColorColorModel)
            {
                auto oTimeColorModel = dynamic_cast<CRecordTimeColorModel*>(pRec);
                oAnimClr.clrSpc = new PPTX::Limit::TLColorSpace;
                std::wstring clrSpc;
                if (!clrAtom.m_fColorSpacePropertyUsed) clrSpc = L"rgb";
                else clrSpc = oTimeColorModel->m_Value ? L"hsl" : L"rgb";
                oAnimClr.clrSpc = clrSpc;
            }
            else if (pRec->m_oHeader.RecInstance == TL_TBPID_ColorDirection)
            {
                auto oTimeColorDirection = dynamic_cast<CRecordTimeColorDirection*>(pRec);
                oAnimClr.dir = new PPTX::Limit::TLColorDirection;
                std::wstring dir;
                if (!clrAtom.m_fDirectionPropertyUsed) dir = L"cw";
                else dir = oTimeColorDirection->m_Value ? L"ccw" : L"cw";
                oAnimClr.dir = dir;
            }
        }
    }

    if (clrAtom.m_fByPropertyUsed)
    {
        if (clrAtom.m_sColorBy.model == 0) // RGB == 0
        {
            oAnimClr.byR = clrAtom.m_sColorBy.component0;
            oAnimClr.byG = clrAtom.m_sColorBy.component1;
            oAnimClr.byB = clrAtom.m_sColorBy.component2;
        }
        else if (clrAtom.m_sColorBy.model == 1) // HSL == 1
        {
            oAnimClr.byH = clrAtom.m_sColorBy.component0;
            oAnimClr.byS = clrAtom.m_sColorBy.component1;
            oAnimClr.byL = clrAtom.m_sColorBy.component2;
        }
    }

    if (clrAtom.m_fFromPropertyUsed)
    {
        oAnimClr.from = *new PPTX::Logic::UniColor;
        if (clrAtom.m_sColorFrom.model == 0)
        {
            auto pSrgb = new PPTX::Logic::SrgbClr;
            pSrgb->red = clrAtom.m_sColorFrom.component0;
            pSrgb->green = clrAtom.m_sColorFrom.component1;
            pSrgb->blue = clrAtom.m_sColorFrom.component2;
            oAnimClr.from.Color = pSrgb;
        }
        else
        {
            auto pScheme = new PPTX::Logic::SchemeClr;
            std::wstring strVal;
            UINT index = clrAtom.m_sColorFrom.component0;
            if (index >= 4 || index < 10)
            {
                strVal = L"accent" + std::to_wstring(index - 3);
            }
            pScheme->val = strVal;
            oAnimClr.from.Color = pScheme;
        }
    }

    if (clrAtom.m_fToPropertyUsed)
    {
        oAnimClr.to = *new PPTX::Logic::UniColor;
        if (clrAtom.m_sColorTo.model == 0)
        {
            auto pSrgb = new PPTX::Logic::SrgbClr;
            pSrgb->red = clrAtom.m_sColorTo.component0;
            pSrgb->green = clrAtom.m_sColorTo.component1;
            pSrgb->blue = clrAtom.m_sColorTo.component2;
            oAnimClr.to.Color = pSrgb;
        }
        else
        {
            auto pScheme = new PPTX::Logic::SchemeClr;
            std::wstring strVal;
            UINT index = clrAtom.m_sColorTo.component0;
            if (index >= 4 || index < 10)
            {
                strVal = L"accent" + std::to_wstring(index - 3);
            }
            pScheme->val = strVal;
            oAnimClr.to.Color = pScheme;
        }
    }
}
}
