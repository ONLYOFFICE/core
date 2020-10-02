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

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/CBhvr.h"
#include "TgtEl.h"
#include "CTn.h"


namespace PPT_FORMAT
{
    void ConvertCRecordTimeSetBehaviorContainerToCBhvr(CRecordTimeSetBehaviorContainer *pOld, PPTX::Logic::CBhvr &oBhvr)
    {
        if (!pOld) return;

        if (!pOld->m_oBehavior.m_pStringList->m_arrRgChildRec.empty())
        {
            oBhvr.attrNameLst = new PPTX::Logic::AttrNameLst();
        }
        for (const auto& oldAttr : pOld->m_oBehavior.m_pStringList->m_arrRgChildRec)
        {
            PPTX::Logic::AttrName addAttr;
            addAttr.text = oldAttr.m_stringValue;
            oBhvr.attrNameLst->list.push_back(addAttr);

        }

        oBhvr.tgtEl.spTgt = new PPTX::Logic::SpTgt();
        oBhvr.tgtEl.spTgt->spid =
                std::to_wstring(pOld->m_oBehavior.
                                m_oClientVisualElement.
                                m_oVisualShapeAtom.m_nObjectIdRef);
    }

    // TODO
    void FillCBhvr(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::CBhvr &oBhvr)
    {
        CRecordTimeBehaviorContainer* bhvr = nullptr;
        if (pETNC->m_haveSetBehavior)           bhvr = &(pETNC->m_pTimeSetBehavior->m_oBehavior);
        else if (pETNC->m_haveEffectBehavior)   bhvr = &(pETNC->m_pTimeEffectBehavior->m_oBehavior);
        else if (pETNC->m_haveAnimateBehavior)  bhvr = &(pETNC->m_pTimeAnimateBehavior->m_oBehavior);
        else if (pETNC->m_haveColorBehavior)    bhvr = &(pETNC->m_pTimeColorBehavior->m_oBehavior);
        else if (pETNC->m_haveMotionBehavior)   bhvr = &(pETNC->m_pTimeMotionBehavior->m_oTimeBehavior);
        else if (pETNC->m_haveRotationBehavior) bhvr = &(pETNC->m_pTimeRotationBehavior->m_oBehavior);
        else if (pETNC->m_haveScaleBehavior)    bhvr = &(pETNC->m_pTimeScaleBehavior->m_oBehavior);
        else                                    bhvr = &(pETNC->m_pTimeCommandBehavior->m_oBevavior);


        if (bhvr->m_haveStringList)
        {
            if (!bhvr->m_pStringList->m_arrRgChildRec.empty())
            {
                oBhvr.attrNameLst = new PPTX::Logic::AttrNameLst();
            }
            for (const auto& oldAttr : bhvr->m_pStringList->m_arrRgChildRec)
            {
                PPTX::Logic::AttrName addAttr;
                addAttr.text = oldAttr.m_stringValue;
                oBhvr.attrNameLst->list.push_back(addAttr);

            }
        }

        FillCTn(pETNC, oBhvr.cTn);

        if (bhvr->m_oClientVisualElement.m_bVisualShapeAtom)
        {
            oBhvr.tgtEl.spTgt = new PPTX::Logic::SpTgt();
            oBhvr.tgtEl.spTgt->spid =
                    std::to_wstring(bhvr->
                                    m_oClientVisualElement.
                                    m_oVisualShapeAtom.m_nObjectIdRef);
        }
    }
}
