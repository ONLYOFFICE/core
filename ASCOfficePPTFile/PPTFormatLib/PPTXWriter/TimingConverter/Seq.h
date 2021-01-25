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

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Seq.h"
#include "../../Records/Animations/ExtTimeNodeContainer.h"
#include "CTn.h"

namespace PPT_FORMAT
{
void FillSeq(PPT_FORMAT::CRecordExtTimeNodeContainer *pETNC,
             PPTX::Logic::Seq& oSec)
{
    if (pETNC->m_haveSequenceAtom)
    {
        if (pETNC->m_pTimeSequenceDataAtom->m_fConcurrencyPropertyUsed)
            oSec.concurrent = (bool)pETNC->m_pTimeSequenceDataAtom->m_nConcurrency;
        if (pETNC->m_pTimeSequenceDataAtom->m_fNextActionPropertyUsed)
            oSec.nextAc = pETNC->m_pTimeSequenceDataAtom->m_nNextAction ? L"seek" : L"none";
        if (pETNC->m_pTimeSequenceDataAtom->m_fPreviousActionPropertyUsed)
            oSec.prevAc = pETNC->m_pTimeSequenceDataAtom->m_nPreviousAction ? L"skipTimed" : L"none";
    }
    FillCTn(pETNC, oSec.cTn);

    // Fill cond lists
    if (!pETNC->m_arrRgEndTimeCondition.empty())
    {
        oSec.prevCondLst = new PPTX::Logic::CondLst();
        oSec.prevCondLst->node_name = L"prevCondLst";
    }
    for (auto oldCond : pETNC->m_arrRgEndTimeCondition)
    {
        PPTX::Logic::Cond cond;
        cond.node_name = L"cond";
        FillCond(oldCond, cond);
        oSec.prevCondLst->list.push_back(cond);
    }

    if (!pETNC->m_arrRgBeginTimeCondition.empty())
    {
        oSec.nextCondLst = new PPTX::Logic::CondLst();
        oSec.nextCondLst->node_name = L"nextCondLst";
    }
    for (auto oldCond : pETNC->m_arrRgBeginTimeCondition)
    {
        PPTX::Logic::Cond cond;
        cond.node_name = L"cond";
        FillCond(oldCond, cond);
        oSec.nextCondLst->list.push_back(cond);
    }


}
}
