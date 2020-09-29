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

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/CTn.h"
#include "../../Records/Animations/ExtTimeNodeContainer.h"

#include "TnChildren.h"
#include "Cond.h"


namespace PPT_FORMAT
{
void ConvertTnChild(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::TimeNodeBase &oChild);

    void FillCTn(CRecordExtTimeNodeContainer* pETNC, PPTX::Logic::CTn &oCTn)
    {
        if (pETNC->m_haveTimePropertyList)
        {
            for (auto* pTimePropertyID4TimeNode : pETNC->m_pTimePropertyList->m_arrElements)
            {
                // TODO Unknown
            }

            std::wstring nodeType;
            switch (pETNC->m_pTimePropertyList->m_oEffectNodeType.m_Value)
            {
            case 1: nodeType = L"clickEffect"; break;
            case 2: nodeType = L"withEffect"; break;
            case 3: nodeType = L"afterEffect"; break;
            case 4: nodeType = L"mainSeq"; break;
            case 5: nodeType = L"interactiveSeq"; break;
            case 6: nodeType = L"clickPar"; break;
            case 7: nodeType = L"withGroup"; break;
            case 8: nodeType = L"afterGroup"; break;
            case 9: nodeType = L"tmRoot"; break;
            }
            if (!nodeType.empty())
                oCTn.nodeType = nodeType;
    }

        // Reading TimeNodeAtom
        const auto& oTimeNodeAtom = pETNC->m_oTimeNodeAtom;

        // Write restart
        oCTn.restart = oTimeNodeAtom.m_fRestartProperty ?
                    PPTX::Limit::TLRestart(oTimeNodeAtom.m_dwRestart) :
                    PPTX::Limit::TLRestart();


        // Write fill
        oCTn.fill = oTimeNodeAtom.m_fFillProperty ?
                    PPTX::Limit::TLNodeFillType(oTimeNodeAtom.m_dwFill) :
                    PPTX::Limit::TLNodeFillType();

        // Write dur
        if (oTimeNodeAtom.m_fDurationProperty)
        {
            if (oTimeNodeAtom.m_nDuration == -1)
                oCTn.dur = L"indefinite";
            else
                oCTn.dur = std::to_wstring(oTimeNodeAtom.m_nDuration);
        }


        //// Write Children ////

        // Write stCondLst
        if (pETNC->m_arrRgBeginTimeCondition.empty() == false)
        {
            oCTn.stCondLst = new PPTX::Logic::CondLst;
            oCTn.stCondLst->name = L"stCondLst";
        }
        for (auto* oldCond : pETNC->m_arrRgBeginTimeCondition) {
                    PPTX::Logic::Cond cond;
                    cond.name = L"cond";
                    FillCond(oldCond, cond);
                    oCTn.stCondLst->list.push_back(cond);
        }


        // Write childTnLst
        if (pETNC->m_arrRgExtTimeNodeChildren.empty() == false)
        {
            oCTn.childTnLst = new PPTX::Logic::ChildTnLst;
        }
        for (auto* oldChild : pETNC->m_arrRgExtTimeNodeChildren) {
                    PPTX::Logic::TimeNodeBase child;
                    ConvertTnChild(oldChild, child);
                    oCTn.childTnLst->list.push_back(child);
        }


        // Write endCondLst
        if (pETNC->m_arrRgEndTimeCondition.empty() == false)
        {
            oCTn.endCondLst = new PPTX::Logic::CondLst;
            oCTn.endCondLst->name = L"endCondLst";
        }
        for (auto* oldCond : pETNC->m_arrRgEndTimeCondition) {
                    PPTX::Logic::Cond cond;
                    cond.name = L"cond";
                    FillCond(oldCond, cond);
                    oCTn.endCondLst->list.push_back(cond);
        }


        // Write iterate
        if (pETNC->m_haveIterateDataAtom)
        {
            auto* iter = pETNC->m_pTimeIterateDataAtom;
            oCTn.iterate = new PPTX::Logic::Iterate;

            std::wstring type[] = {L"el", L"wd", L"lt"};
            if (iter->m_fIterateTypePropertyUsed)
                oCTn.iterate->type = type[iter->m_nIterateType % 3];

            if (iter->m_fIterateDirectionPropertyUsed)
                oCTn.iterate->backwards = (bool)iter->m_nIterateDirection;

            if (iter->m_fIterateIntervalTypePropertyUsed)
                oCTn.iterate->tmAbs = std::to_wstring(iter->m_nIterateIntervalType);

            if (iter->m_fIterateIntervalPropertyUsed)
                oCTn.iterate->tmPct = iter->m_nIterateInterval;

        }


        // Write endSync
        if (pETNC->m_haveTimeEndSyncTime)
        {
            auto* sync = pETNC->m_pTimeEndSyncTimeCondition;
            oCTn.endSync = new PPTX::Logic::Cond;
            oCTn.endSync->name = L"endSync";
            FillCond(sync, *(oCTn.endSync));
        }


        // Write subTnLst
        if (pETNC->m_arrRgSubEffect.empty() == false)
        {
            oCTn.subTnLst = new PPTX::Logic::TnLst;
        }
        // TODO
//        for (auto* oldChild : pETNC->m_arrRgSubEffect) {
//                    PPTX::Logic::TimeNodeBase child;
//                    ConvertCRecordExtTimeNodeContainerToTnLst(oldChild, child);
//                    oCTn.childTnLst->list.push_back(child);
//        }

    }

}
