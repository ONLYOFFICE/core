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

#include "../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLRestart.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Limit/TLNodeFillType.h"

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Par.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Seq.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Set.h"


#include "../../../ASCOfficePPTXFile/PPTXFormat/WrapperWritingElement.h"


#include "../../Records/Animations/ExtTimeNodeContainer.h"


#include "Seq.h"


namespace PPT_FORMAT
{
void ConvertCRecordExtTimeNodeContainerToTimeNodeBase(PPT_FORMAT::CRecordExtTimeNodeContainer *pETNC,
                                                             PPTX::Logic::TimeNodeBase &oTimeNodeBase);
//inline void ConvertConvertCRecordExtTimeNodeContainerToPar(
//        CRecordExtTimeNodeContainer *pETNC,
//        PPTX::Logic::Par &oPar)
//{

//}

void FillCTn(PPT_FORMAT::CRecordExtTimeNodeContainer *pETNC,
                    PPTX::Logic::CTn &oCTn)
{
    if (!pETNC->m_arrRgExtTimeNodeChildren.empty()) oCTn.childTnLst = new PPTX::Logic::ChildTnLst();
    for (auto children : pETNC->m_arrRgExtTimeNodeChildren)
    {
        PPTX::Logic::TimeNodeBase oTimeNodeBase;
        ConvertCRecordExtTimeNodeContainerToTimeNodeBase(children, oTimeNodeBase);
        oCTn.childTnLst->list.push_back(oTimeNodeBase);
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

}

void FillSet(PPT_FORMAT::CRecordExtTimeNodeContainer *pETNC,
                    PPTX::Logic::Set *pSet)
{
    auto& oSetBeh = *(pETNC->m_pTimeSetBehavior);
    // TODO
}

void ConvertCRecordExtTimeNodeContainerToTimeNodeBase(PPT_FORMAT::CRecordExtTimeNodeContainer *pETNC,
                                                             PPTX::Logic::TimeNodeBase &oTimeNodeBase)
{
    switch (pETNC->m_oTimeNodeAtom.m_dwType)
    {
        case TL_TNT_Parallel:
    {
        auto pPar = new PPTX::Logic::Par();

        FillCTn(pETNC, pPar->cTn);

        oTimeNodeBase.m_node = pPar;
        break;
    }
    case TL_TNT_Sequential:
    {
        auto pSeq = new PPTX::Logic::Seq();

        FillSeq(pETNC, pSeq);
        FillCTn(pETNC, pSeq->cTn);

        oTimeNodeBase.m_node = pSeq;
        break;
    }
    case TL_TNT_Behavior:
    {
        //TODO
        auto pSet = new PPTX::Logic::Set();

        FillSet(pETNC, pSet);

        oTimeNodeBase.m_node = pSet;
        break;
    }
    case TL_TNT_Media:
    {
        // TODO
        break;
    }
    }
}
void ConvertCRecordExtTimeNodeContainerToTnLst(
                            PPT_FORMAT::CRecordExtTimeNodeContainer *pETNC,
                            PPTX::Logic::TnLst &oTnLst)
{
    if (!pETNC)
        return;

    PPTX::Logic::TimeNodeBase oChildTimeNodeBase;
    ConvertCRecordExtTimeNodeContainerToTimeNodeBase(pETNC, oChildTimeNodeBase);
    oTnLst.list.push_back(oChildTimeNodeBase);

}
}
