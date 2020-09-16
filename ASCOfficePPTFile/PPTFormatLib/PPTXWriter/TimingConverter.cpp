#include "TimingConverter.h"

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Par.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Seq.h"

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldP.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldOleChart.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldDgm.h"

#include "../../../ASCOfficePPTXFile/PPTXFormat/WrapperWritingElement.h"

#include "../Records/Animations/ChartBuildContainer.h"
#include "../Records/Animations/DiagramBuildContainer.h"
#include "../Records/Animations/ExtTimeNodeContainer.h"

using namespace PPT_FORMAT;

static void ConvertCRecordBuildListContainerToBldLst(
                            PPT_FORMAT::CRecordBuildListContainer *pBLC,
                            PPTX::Logic::BldLst &oBL)
{
    if (!pBLC)
        return;

    for (unsigned i = 0; i < pBLC->n_arrRgChildRec.size(); i++)
    {
        PPTX::Logic::BuildNodeBase oBuildNodeBase;
        switch ( pBLC->n_arrRgChildRec[i]->m_oHeader.RecType ) {
        case RT_ParaBuild:
        {
            CRecordParaBuildContainer* pRec =
                    (CRecordParaBuildContainer*)pBLC->n_arrRgChildRec[i];
            PPTX::Logic::BldP* pBldP = new PPTX::Logic::BldP();

            pBldP->spid      = std::to_wstring(pRec->m_oBuildAtom.m_nShapeIdRef);
            pBldP->grpId     = (int)pRec->m_oBuildAtom.m_nBuildId;
            pBldP->uiExpand  = pRec->m_oBuildAtom.m_fExpanded;

            pBldP->build             = pRec->m_oParaBuildAtom.m_nParaBuild;
            pBldP->advAuto           = std::to_wstring(pRec->m_oParaBuildAtom.m_nDelayTime);
            pBldP->animBg            = pRec->m_oParaBuildAtom.m_fAnimBackground;
            pBldP->rev               = pRec->m_oParaBuildAtom.m_fReverse;
            pBldP->autoUpdateAnimBg  = pRec->m_oParaBuildAtom.m_fAutomatic;


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
            pBldC->bld       = pRec->m_oChartBuildAtom.m_ChartBuild;

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

            pBldD->bld       = pRec->m_oDiagramBuildAtom.m_oDiagramBuild;

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

//static inline void ConvertConvertCRecordExtTimeNodeContainerToPar(
//        CRecordExtTimeNodeContainer *pETNC,
//        PPTX::Logic::Par &oPar)
//{

//}
static void ConvertCRecordExtTimeNodeContainerToTimeNodeBase(PPT_FORMAT::CRecordExtTimeNodeContainer *pETNC,
                                                             PPTX::Logic::TimeNodeBase &oTimeNodeBase)
{
    switch (pETNC->m_oTimeNodeAtom.m_dwType)
    {
        case TL_TNT_Parallel:
    {
        auto pPar = new PPTX::Logic::Par();
        for (auto* pRecChild : pETNC->m_arrRgExtTimeNodeChildren)
        {
            PPTX::Logic::TimeNodeBase oChildTimeNodeBase;
            ConvertCRecordExtTimeNodeContainerToTimeNodeBase(pRecChild, oChildTimeNodeBase);
            pPar->cTn.childTnLst.push_back(oChildTimeNodeBase);

        }
        oTimeNodeBase.m_node = pPar;
        break;
    }
    case TL_TNT_Sequential:
    {
        auto pSeq = new PPTX::Logic::Seq();
        for (auto* pRecChild : pETNC->m_arrRgExtTimeNodeChildren)
        {
            PPTX::Logic::TimeNodeBase oChildTimeNodeBase;
            ConvertCRecordExtTimeNodeContainerToTimeNodeBase(pRecChild, oChildTimeNodeBase);
            pSeq->cTn.childTnLst.push_back(oChildTimeNodeBase);

        }
        oTimeNodeBase.m_node = pSeq;
        break;
    }
    }
}
static void ConvertCRecordExtTimeNodeContainerToTnLst(
                            PPT_FORMAT::CRecordExtTimeNodeContainer *pETNC,
                            PPTX::Logic::TnLst &oTnLst)
{
    if (!pETNC)
        return;

    for (auto* pChildETNC : pETNC->m_arrRgExtTimeNodeChildren)
    {
        PPTX::Logic::TimeNodeBase oChildTimeNodeBase;
        ConvertCRecordExtTimeNodeContainerToTimeNodeBase(pChildETNC, oChildTimeNodeBase);
        oTnLst.list.push_back(oChildTimeNodeBase);
    }
}

void PPT_FORMAT::ConvertPP10SlideBinaryTagExtensionToTiming(PPT_FORMAT::CRecordPP10SlideBinaryTagExtension &oRec, PPTX::Logic::Timing &oTm)
{
    if (oRec.m_haveExtTime)
    {
        oTm.tnLst = new PPTX::Logic::TnLst();
        ConvertCRecordExtTimeNodeContainerToTnLst(oRec.m_pExtTimeNodeContainer, *(oTm.tnLst));
    }

    if (oRec.m_haveBuildList)
    {
        oTm.bldLst = new PPTX::Logic::BldLst();
        ConvertCRecordBuildListContainerToBldLst(oRec.m_pBuildListContainer, *(oTm.bldLst));
    }

    return;
}

