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
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldLst.h"
#include "BldP.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldOleChart.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldDgm.h"

#include "../Records/Animations/BuildListContainer.h"
#include "../Records/Animations/ChartBuildContainer.h"
#include "../Records/Animations/DiagramBuildContainer.h"
#include "../Records/Animations/ExtTimeNodeContainer.h"


namespace PPT_FORMAT
{
void ConvertCRecordBuildListContainerToBldLst(
                            PPT_FORMAT::CRecordBuildListContainer *pBLC,
                            PPTX::Logic::BldLst &oBL)
{
    if (!pBLC)
        return;
    // Write p
    for (unsigned i = 0; i < pBLC->n_arrRgChildRec.size(); i++)
    {
        PPTX::Logic::BuildNodeBase oBuildNodeBase;
        switch ( pBLC->n_arrRgChildRec[i]->m_oHeader.RecType ) {
        case RT_ParaBuild:
        {
            CRecordParaBuildContainer* pRec =
                    (CRecordParaBuildContainer*)pBLC->n_arrRgChildRec[i];
            PPTX::Logic::BldP* pBldP = new PPTX::Logic::BldP();
            FillBldP(pRec, *pBldP);

            oBuildNodeBase.m_node = pBldP;
            break;
        }
        case RT_ChartBuild:
        {
            CRecordChartBuildContainer* pRec =
                    (CRecordChartBuildContainer*)pBLC->n_arrRgChildRec[i];
            PPTX::Logic::BldOleChart* pBldC = new PPTX::Logic::BldOleChart();

            pBldC->spid      = std::to_wstring(pRec->m_oBuildAtom.m_nShapeIdRef);
            pBldC->grpId     = (int)pRec->m_oBuildAtom.m_nBuildId;
            pBldC->uiExpand  = pRec->m_oBuildAtom.m_fExpanded;

            pBldC->animBg    = pRec->m_oChartBuildAtom.m_fAnimBackground;

            std::vector<std::wstring> ST_TLOleChartBuildType =
            {
                L"allAtOnce",
                L"series",
                L"category",
                L"seriesEl",
                L"categoryEl"
            };
            pBldC->bld       = ST_TLOleChartBuildType[pRec->m_oChartBuildAtom.m_ChartBuild % 5];

            oBuildNodeBase.m_node = pBldC;
            break;
        }

        case RT_DiagramBuild:
        {
            CRecordDiagramBuildContainer* pRec =
                    (CRecordDiagramBuildContainer*)pBLC->n_arrRgChildRec[i];
            PPTX::Logic::BldDgm* pBldD = new PPTX::Logic::BldDgm();

            pBldD->spid      = std::to_wstring(pRec->m_oBuildAtom.m_nShapeIdRef);
            pBldD->grpId     = (int)pRec->m_oBuildAtom.m_nBuildId;
            pBldD->uiExpand  = pRec->m_oBuildAtom.m_fExpanded;

            std::vector<std::wstring> ST_TLDiagramBuildType =
            {
                L"whole",
                L"depthByNode",
                L"depthByBranch",
                L"breadthByNode",
                L"breadthByLvl",
                L"cw",
                L"cwIn",
                L"cwOut",
                L"ccw",
                L"ccwIn",
                L"ccwOut",
                L"inByRing",
                L"outByRing",
                L"up",
                L"down",
                L"allAtOnce",
                L"cust"
            };
            pBldD->bld       = ST_TLDiagramBuildType[pRec->m_oDiagramBuildAtom.m_oDiagramBuild % 17];

            oBuildNodeBase.m_node = pBldD;
            break;
        }
        default:
            break;
        }
        oBL.list.push_back(oBuildNodeBase);

    }


    return;
}
}
