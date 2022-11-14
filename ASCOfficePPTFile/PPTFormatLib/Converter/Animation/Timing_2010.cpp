#include "Timing_2010.h"


namespace PPT {
namespace Converter {

Timing_2010::Timing_2010(CRecordPP10SlideBinaryTagExtension *pAnim_2010, const std::unordered_set<int> &shapesID) :
    pTagExtAnim(pAnim_2010),
    slideShapes(shapesID)
{}

void Timing_2010::Convert(PPTX::Logic::Timing &timimg, CExMedia *pExMedia, CRelsGenerator *pRels)
{
    ConvertBldLst(timimg.bldLst.get2(), pTagExtAnim->m_pBuildListContainer);
}

void Timing_2010::ConvertBldLst(PPTX::Logic::BldLst &bldLst, CRecordBuildListContainer *pBLC)
{
    if (pBLC == nullptr)
        return;

    for (IRecord* pDBC : pBLC->m_arRecords)
    {
        PPTX::Logic::BuildNodeBase oBuildNodeBase;
        auto* pSub = dynamic_cast<CRecordBuildListSubContainer*>(pDBC);
        if (pSub == nullptr)
            continue;
        if (slideShapes.count(pSub->buildAtom.m_nShapeIdRef) == false)
            continue;

        FillBuildNodeBase(pSub, oBuildNodeBase);
    }
}

void Timing_2010::FillBuildNodeBase(CRecordBuildListSubContainer *pSub, PPTX::Logic::BuildNodeBase oBuildNodeBase)
{
    switch (pSub->m_oHeader.RecType)
    {
    case RT_ParaBuild:
    {
        auto pBldP = new PPTX::Logic::BldP();
        FillBldP(dynamic_cast<CRecordParaBuildContainer*>(pSub), *pBldP);

        oBuildNodeBase.m_node = pBldP;
        break;
    }
    case RT_ChartBuild:
    {
        auto pBldOleChart = new PPTX::Logic::BldOleChart();
        FillBldOleChart(dynamic_cast<CRecordChartBuildContainer*>(pSub), *pBldOleChart);

        oBuildNodeBase.m_node = pBldOleChart;
        break;
    }
    case RT_DiagramBuild:
    {
        auto pBldDgm = new PPTX::Logic::BldDgm();
        FillBldDgm(dynamic_cast<CRecordDiagramBuildContainer*>(pSub), *pBldDgm);

        oBuildNodeBase.m_node = pBldDgm;
        break;
    }
    default:
        break;
    }

}

void Timing_2010::FillBldP(CRecordParaBuildContainer* pPBC, PPTX::Logic::BldP &oBP)
{
    oBP.spid      = std::to_wstring(pPBC->buildAtom.m_nShapeIdRef);
    oBP.grpId     = (int)pPBC->buildAtom.m_nBuildId;
    oBP.uiExpand  = pPBC->buildAtom.m_fExpanded;

    oBP.advAuto           = std::to_wstring(pPBC->m_oParaBuildAtom.m_nDelayTime);
    oBP.animBg            = pPBC->m_oParaBuildAtom.m_fAnimBackground;
    oBP.rev               = pPBC->m_oParaBuildAtom.m_fReverse;
    oBP.autoUpdateAnimBg  = pPBC->m_oParaBuildAtom.m_fAutomatic;

    std::vector<std::wstring> ST_TLParaBuildType =
    {
        L"allAtOnce",
        L"p",
        L"cust",
        L"whole"
    };
    oBP.build             = ST_TLParaBuildType[pPBC->m_oParaBuildAtom.m_nParaBuild % 4];
}

void Timing_2010::FillBldOleChart(CRecordChartBuildContainer *pCBC, PPTX::Logic::BldOleChart &oBP)
{
    oBP.spid      = std::to_wstring(pCBC->buildAtom.m_nShapeIdRef);
    oBP.grpId     = (int)pCBC->buildAtom.m_nBuildId;
    oBP.uiExpand  = pCBC->buildAtom.m_fExpanded;

    oBP.animBg    = pCBC->m_oChartBuildAtom.m_fAnimBackground;

    std::vector<std::wstring> ST_TLOleChartBuildType =
    {
        L"allAtOnce",
        L"series",
        L"category",
        L"seriesEl",
        L"categoryEl"
    };
    oBP.bld       = ST_TLOleChartBuildType[pCBC->m_oChartBuildAtom.m_ChartBuild % 5];
}

void Timing_2010::FillBldDgm(CRecordDiagramBuildContainer *pDBC, PPTX::Logic::BldDgm &oBP)
{
    oBP.spid      = std::to_wstring(pDBC->buildAtom.m_nShapeIdRef);
    oBP.grpId     = (int)pDBC->buildAtom.m_nBuildId;
    oBP.uiExpand  = pDBC->buildAtom.m_fExpanded;

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
    oBP.bld       = ST_TLDiagramBuildType[pDBC->m_oDiagramBuildAtom.m_oDiagramBuild % 17];
}




}
}
