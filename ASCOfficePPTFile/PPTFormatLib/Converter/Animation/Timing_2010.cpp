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

void Timing_2010::ConvertTnLst(PPTX::Logic::TnLst &tnLst, CRecordExtTimeNodeContainer *pETNC)
{
    if (tnLst.list.empty())
        tnLst.list.push_back(PPTX::Logic::TimeNodeBase());

    FillTnChild(pETNC, tnLst.list.front());
}

void Timing_2010::FillTnChild(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::TimeNodeBase &oChild)
{
    if (pETNC->m_haveSequenceAtom)
    {
        if (!oChild.m_node.IsInit())
            oChild.m_node = new PPTX::Logic::Seq;

        FillSeq(pETNC, oChild.m_node.as<PPTX::Logic::Seq>());
    }
    else if (pETNC->m_haveSetBehavior)
    {
        auto set = new PPTX::Logic::Set;
        FillSet(pETNC, *set);
        oChild.m_node = set;
    }
    else if (pETNC->m_haveAnimateBehavior)
    {
        auto anim = new PPTX::Logic::Anim;
        FillAnim(pETNC->m_pTimeAnimateBehavior, *anim);
        FillCTn(pETNC, anim->cBhvr.cTn);
        oChild.m_node = anim;
    }
    else if (pETNC->m_haveColorBehavior)
    {
        auto animClr = new PPTX::Logic::AnimClr;
        FillAnimClr(pETNC->m_pTimeColorBehavior, pETNC->m_pTimePropertyList, *animClr);
        oChild.m_node = animClr;
    }
    else if (pETNC->m_haveEffectBehavior)
    {
        auto animEffect = new PPTX::Logic::AnimEffect;
        FillAnimEffect(pETNC, *animEffect);
        oChild.m_node = animEffect;
    }
    else if (pETNC->m_haveMotionBehavior)
    {
        auto motion = new PPTX::Logic::AnimMotion;
        FillAnimMotion(pETNC, *motion);
        oChild.m_node = motion;
    }
    else if (pETNC->m_haveRotationBehavior)
    {
        auto rot = new PPTX::Logic::AnimRot;
        FillAnimRot(pETNC, *rot);
        oChild.m_node = rot;
    }
    else if (pETNC->m_haveScaleBehavior)
    {
        auto scale = new PPTX::Logic::AnimScale;
        FillAnimScale(pETNC, *scale);
        oChild.m_node = scale;
    }
    else if (pETNC->m_haveCommandBehavior)
    {
        auto cmd = new PPTX::Logic::Cmd;
        FillCmd(pETNC, *cmd);
        oChild.m_node = cmd;
    }
    else if (pETNC->m_oTimeNodeAtom.m_dwType == TL_TNT_Parallel)
    {
        auto par = new PPTX::Logic::Par;
        FillPar(pETNC, *par);
        oChild.m_node = par;
    }
    else if (pETNC->m_haveClientVisualElement)
    {
        if (pETNC->m_pClientVisualElement->m_bVisualPageAtom)
        {

        }
        if (pETNC->m_pClientVisualElement->m_bVisualShapeAtom)
        {
            if (pETNC->m_pClientVisualElement->m_oVisualShapeAtom.m_Type == TL_TVET_Video)
            {
                auto video = new PPTX::Logic::Video;
                FillVideo(pETNC, *video);
                oChild.m_node = video;
            }

            if (pETNC->m_pClientVisualElement->m_oVisualShapeAtom.m_Type == TL_TVET_Audio)
            {
                auto audio = new PPTX::Logic::Audio;
                FillAudio(pETNC, *audio);
                oChild.m_node = audio;
            }
        }
    }
}

void Timing_2010::FillSeq(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::Seq &oSec)
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
        if (m_cTnDeep == 1)
            FillEmptyTargetCond(cond);
        oSec.prevCondLst->list.push_back(cond);
    }

    if (!pETNC->m_arrRgNextTimeCondition.empty())
    {
        oSec.nextCondLst = new PPTX::Logic::CondLst();
        oSec.nextCondLst->node_name = L"nextCondLst";
    }
    for (auto oldCond : pETNC->m_arrRgNextTimeCondition)
    {
        PPTX::Logic::Cond cond;
        cond.node_name = L"cond";
        FillCond(oldCond, cond);
        if (m_cTnDeep == 1)
            FillEmptyTargetCond(cond);
        oSec.nextCondLst->list.push_back(cond);
    }
}

void Timing_2010::FillCTn(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::CTn &oCTn)
{
    oCTn.id = m_cTnId++;
    m_cTnDeep++;

    // Reading TimeNodeAtom
    const auto &oTimeNodeAtom = pETNC->m_oTimeNodeAtom;

    // Write restart
    if (oTimeNodeAtom.m_fRestartProperty)
        oCTn.restart = PPTX::Limit::TLRestart(oTimeNodeAtom.m_dwRestart) ;


    // Write fill
    if (oTimeNodeAtom.m_fFillProperty)
        oCTn.fill = PPTX::Limit::TLNodeFillType(oTimeNodeAtom.m_dwFill);

    // Write dur
    if (oTimeNodeAtom.m_fDurationProperty)
    {
        if (oTimeNodeAtom.m_nDuration == -1)
            oCTn.dur = L"indefinite";
        else
            oCTn.dur = std::to_wstring(oTimeNodeAtom.m_nDuration);
    }


    //// Write Children ////

    //Write cTn attr
    if (pETNC->m_haveTimePropertyList && !pETNC->m_pTimePropertyList->m_bEmtyNode)
    {
        FillCTn(pETNC->m_pTimePropertyList, oCTn);
    }

    if (!pETNC->m_haveSequenceAtom)
    {
        // Write stCondLst
        if (pETNC->m_arrRgBeginTimeCondition.empty() == false)
        {
            oCTn.stCondLst = new PPTX::Logic::CondLst;
            oCTn.stCondLst->node_name = L"stCondLst";
        }
        for (auto *oldCond : pETNC->m_arrRgBeginTimeCondition) {
            PPTX::Logic::Cond cond;
            cond.node_name = L"cond";
            FillCond(oldCond, cond);
            oCTn.stCondLst->list.push_back(cond);
        }


        // Write endCondLst
        if (pETNC->m_arrRgEndTimeCondition.empty() == false)
        {
            oCTn.endCondLst = new PPTX::Logic::CondLst;
            oCTn.endCondLst->node_name = L"endCondLst";
            FillCondLst(pETNC->m_arrRgEndTimeCondition, oCTn.endCondLst.get2());
        }
    }


    // Write childTnLst
    if (pETNC->m_arrRgExtTimeNodeChildren.empty() == false)
    {
        oCTn.childTnLst = new PPTX::Logic::ChildTnLst;
    }
    for (auto *oldChild : pETNC->m_arrRgExtTimeNodeChildren) {
        PPTX::Logic::TimeNodeBase child;
        FillTnChild(oldChild, child);
        oCTn.childTnLst->list.push_back(child);
    }


    // Write iterate
    if (pETNC->m_haveIterateDataAtom)
    {
        auto *iter = pETNC->m_pTimeIterateDataAtom;
        oCTn.iterate = new PPTX::Logic::Iterate;

        std::wstring type[] = {L"el", L"wd", L"lt"};
        if (iter->m_fIterateTypePropertyUsed)
            oCTn.iterate->type = type[iter->m_nIterateType % 3];

        if (iter->m_fIterateDirectionPropertyUsed)
            oCTn.iterate->backwards = (bool)iter->m_nIterateDirection;

        int intervalType = iter->m_fIterateIntervalTypePropertyUsed ?
                    iter->m_nIterateIntervalType : 0;
        uint iterateInterval = iter->m_fIterateIntervalPropertyUsed ?
                    iter->m_nIterateInterval : 0;

        if (intervalType)
            oCTn.iterate->tmPct = iterateInterval > 1000 ? 10000 : iterateInterval * 10;
        else
            oCTn.iterate->tmAbs = std::to_wstring(iterateInterval);
    }


    // Write endSync
    if (pETNC->m_haveTimeEndSyncTime)
    {
        auto *sync = pETNC->m_pTimeEndSyncTimeCondition;
        oCTn.endSync = new PPTX::Logic::Cond;
        oCTn.endSync->node_name = L"endSync";
        FillCond(sync, *(oCTn.endSync));
    }


    // Write subTnLst
    if (pETNC->m_arrRgSubEffect.empty() == false)
    {
        //        if (
        //                pETNC->m_arrRgSubEffect[0]->m_oTimeNodeAtom.m_fGroupingTypeProperty &&
        //                m_pBldLst &&
        //                m_pPPT10->m_haveBuildList
        //           )
        //        {
        //            oCTn.grpId = 0;
        //            auto bldP = new PPTX::Logic::BldP;
        //            bldP->grpId = 0;
        //            m_currentBldP = bldP;
        //        }

        auto sub = new PPTX::Logic::TnLst;
        sub->node_name = L"subTnLst";
        FillSubTnLst(pETNC->m_arrRgSubEffect, *sub);
        oCTn.subTnLst = sub;

        if (m_currentBldP)
        {
            PPTX::Logic::BuildNodeBase oBuildNodeBase;
            oBuildNodeBase.m_node = m_currentBldP;
            m_pBldLst->list.push_back(oBuildNodeBase);
            m_currentBldP = nullptr;
        }

        for (auto timeModAtom : pETNC->m_arrRgTimeModifierAtom)
        {
            switch (timeModAtom->m_nType)
            {
            case 0:
            {
                oCTn.repeatCount = std::to_wstring((int)
                                                   timeModAtom->m_Value * 1000);
                break;
            }
            case 1:
            {
                // Check 1000
                oCTn.repeatDur = std::to_wstring((int)
                                                 timeModAtom->m_Value * 1000);
                break;
            }
            case 2:
            {
                // Check 1000
                oCTn.spd = std::to_wstring((int)
                                           timeModAtom->m_Value * 1000);
                break;
            }
            case 3:
            {
                // Check 1000
                oCTn.accel = std::to_wstring((int)
                                             timeModAtom->m_Value * 1000);
                break;
            }
            case 4:
            {
                // Check 1000
                oCTn.decel = std::to_wstring((int)
                                             timeModAtom->m_Value * 1000);
                break;
            }
            case 5:
            {
                // Check 1000
                oCTn.autoRev = (bool)timeModAtom->m_Value;
                break;
            }

            }
        }

    }

    // Write stCondLst
    if (pETNC->m_arrRgBeginTimeCondition.empty() == false)
    {
        oCTn.stCondLst = new PPTX::Logic::CondLst;
        oCTn.stCondLst->node_name = L"stCondLst";
        FillStCondLst(pETNC->m_arrRgBeginTimeCondition, oCTn.stCondLst.get2());
    }

    if (oCTn.nodeType.IsInit() == false && (m_cTnDeep == 3 || m_cTnDeep == 4))
    {
        oCTn.nodeType = new PPTX::Limit::TLNodeType();
        oCTn.nodeType->set( m_cTnDeep == 3 ? L"clickPar" : L"withGroup");
    }
    m_cTnDeep--;
}





}
}
