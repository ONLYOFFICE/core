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
#include "Timing_2010.h"

#include "../../../../OOXML/PPTXFormat/Logic/Colors/SchemeClr.h"
#include "../../../../OOXML/PPTXFormat/Logic/Colors/SrgbClr.h"

#include "TimingExeption.h"
#include "TimingUtils.h"


namespace PPT {
namespace Converter {

Timing_2010::Timing_2010(const Intermediate::SlideAnimation &sldAnim) :
    slideAnim(sldAnim)
{}

void Timing_2010::Convert(PPTX::Logic::Timing &timing, CExMedia *pExMedia, CRelsGenerator *pRels)
{
    m_pExMedia = pExMedia;
    m_pRels = pRels;


    auto* pTagExtAnim = slideAnim.pAnim_2010;
    if (pTagExtAnim == nullptr || pTagExtAnim->m_haveExtTime == false)
        return;

    ConvertTnLst(timing, pTagExtAnim->m_pExtTimeNodeContainer);
    ConvertBldLst(timing, pTagExtAnim->m_pBuildListContainer);
}

void Timing_2010::ConvertBldLst(PPTX::Logic::Timing &timimg, CRecordBuildListContainer *pBLC)
{
    if (pBLC == nullptr)
        return;
    if (timimg.bldLst.IsInit())
        timimg.bldLst->list.clear();
    if (pBLC->m_arRecords.empty())
        return;
    if (!timimg.bldLst.IsInit())
        timimg.bldLst = new PPTX::Logic::BldLst;


    for (IRecord* pDBC : pBLC->m_arRecords)
    {
        PPTX::Logic::BuildNodeBase oBuildNodeBase;
        auto* pSub = dynamic_cast<CRecordBuildListSubContainer*>(pDBC);
        if (pSub == nullptr)
            throw TimingExeption("Cannot read BuildListSubContainer");
        if (IsCorrectAnimationSpId(pSub->buildAtom.m_nShapeIdRef) == false)
            continue;

        FillBuildNodeBase(pSub, oBuildNodeBase);
        InsertBuildNode(timimg.bldLst.get2(), oBuildNodeBase);
    }
}

void Timing_2010::InsertBuildNode(PPTX::Logic::BldLst &bldLst, PPTX::Logic::BuildNodeBase &bnb)
{
    if (bnb.m_node.is<PPTX::Logic::BldP>())
        InsertBldP(bldLst, bnb);
    else if (bnb.m_node.IsInit())     // TODO BldOleChart and RT_DiagramBuild. Now it is not support (20.11.22).
        bldLst.list.push_back(bnb);
    else
        return;
}

void Timing_2010::InsertBldP(PPTX::Logic::BldLst &bldLst, PPTX::Logic::BuildNodeBase &bnb)
{
    auto bldP = bnb.m_node.as<PPTX::Logic::BldP>();
    _INT32 spid = -1;
    try {
        spid = std::stoi(bldP.spid);
    } catch (...) {
    }
    if (IsCorrectAnimationSpId(spid))
        bldLst.list.push_back(bnb);
}

void Timing_2010::FillBuildNodeBase(CRecordBuildListSubContainer *pSub, PPTX::Logic::BuildNodeBase &oBuildNodeBase)
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
    if (pPBC == nullptr)
        return;

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
    if (pCBC == nullptr)
        return;

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
    if (pDBC == nullptr)
        return;

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

void Timing_2010::ConvertTnLst(PPTX::Logic::Timing &timing, CRecordExtTimeNodeContainer *pETNC)
{
    if (!timing.tnLst.IsInit())
        timing.tnLst = new PPTX::Logic::TnLst;

    auto& tnLst = timing.tnLst.get2();
    if (tnLst.list.empty())
        tnLst.list.push_back(PPTX::Logic::TimeNodeBase());

    FillTnChild(pETNC, tnLst.list.back());
}

bool Timing_2010::FillTnChild(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::TimeNodeBase &oChild)
{
    if (pETNC->m_haveSequenceAtom)
    {
        oChild.m_node = new PPTX::Logic::Seq;
        FillSeq(pETNC, oChild.m_node.as<PPTX::Logic::Seq>());
    }
    else if (pETNC->m_haveSetBehavior)
    {
        oChild.m_node = new PPTX::Logic::Set;
        FillSet(pETNC, oChild.m_node.as<PPTX::Logic::Set>());
    }
    else if (pETNC->m_haveAnimateBehavior)
    {
        oChild.m_node = new PPTX::Logic::Anim;

        auto& anim = oChild.m_node.as<PPTX::Logic::Anim>();
        FillAnim(pETNC->m_pTimeAnimateBehavior, anim);
        FillCTnRecursive(pETNC, anim.cBhvr.cTn);
    }
    else if (pETNC->m_haveColorBehavior)
    {
        oChild.m_node = new PPTX::Logic::AnimClr;
        FillAnimClr(pETNC->m_pTimeColorBehavior, pETNC->m_pTimePropertyList, oChild.m_node.as<PPTX::Logic::AnimClr>());
    }
    else if (pETNC->m_haveEffectBehavior)
    {
        oChild.m_node = new PPTX::Logic::AnimEffect;
        FillAnimEffect(pETNC, oChild.m_node.as<PPTX::Logic::AnimEffect>());
    }
    else if (pETNC->m_haveMotionBehavior)
    {
        oChild.m_node = new PPTX::Logic::AnimMotion;
        FillAnimMotion(pETNC, oChild.m_node.as<PPTX::Logic::AnimMotion>());
    }
    else if (pETNC->m_haveRotationBehavior)
    {
        oChild.m_node = new PPTX::Logic::AnimRot;
        FillAnimRot(pETNC, oChild.m_node.as<PPTX::Logic::AnimRot>());
    }
    else if (pETNC->m_haveScaleBehavior)
    {
        oChild.m_node = new PPTX::Logic::AnimScale;
        FillAnimScale(pETNC, oChild.m_node.as<PPTX::Logic::AnimScale>());
    }
    else if (pETNC->m_haveCommandBehavior)
    {
        oChild.m_node = new PPTX::Logic::Cmd;
        FillCmd(pETNC, oChild.m_node.as<PPTX::Logic::Cmd>());
    }
    else if (pETNC->m_oTimeNodeAtom.m_dwType == TL_TNT_Parallel)
    {
        oChild.m_node = new PPTX::Logic::Par;
        FillPar(pETNC, oChild.m_node.as<PPTX::Logic::Par>());
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
                oChild.m_node = new PPTX::Logic::Video;
                FillVideo(pETNC, oChild.m_node.as<PPTX::Logic::Video>());
            }

            if (pETNC->m_pClientVisualElement->m_oVisualShapeAtom.m_Type == TL_TVET_Audio)
            {
                oChild.m_node = new PPTX::Logic::Audio;
                FillAudio(pETNC, oChild.m_node.as<PPTX::Logic::Audio>());
            }
        }
    }

    return true;
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
    FillCTnRecursive(pETNC, oSec.cTn);

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
        if (cTNLevel == TimeNodeLevel::root)
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
        if (cTNLevel == TimeNodeLevel::root)
            FillEmptyTargetCond(cond);
        oSec.nextCondLst->list.push_back(cond);
    }
}

void Timing_2010::FillPar(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::Par &oPar)
{
    FillCTnRecursive(pETNC, oPar.cTn);
}

void Timing_2010::FillCBhvr(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::CBhvr &oBhvr)
{
    auto *bhvr = Intermediate::GetTimeBehaviorContainer(pETNC);
    FillCBhvr(bhvr, oBhvr);
    FillCTnRecursive(pETNC, oBhvr.cTn);
}

void Timing_2010::FillCBhvr(CRecordTimeBehaviorContainer *pBhvr, PPTX::Logic::CBhvr &oBhvr)
{
    //// Atom ////

    // additive
    if (pBhvr->m_oBehaviorAtom.m_bAdditivePropertyUsed) {
        oBhvr.additive = new PPTX::Limit::TLAdditive;
        oBhvr.additive = pBhvr->m_oBehaviorAtom.m_nBehaviorAdditive ?
                    L"repl" : L"base";
    }

    // accumulate   - MUST be 0
    // xfrmType     - MUST be 0

    if (pBhvr->m_haveStringList)
    {
        if (!pBhvr->m_pStringList->m_arrRgChildRec.empty())
        {
            oBhvr.attrNameLst = new PPTX::Logic::AttrNameLst();
        }
        for (const auto &oldAttr : pBhvr->m_pStringList->m_arrRgChildRec)
        {
            PPTX::Logic::AttrName addAttr;
            addAttr.text = oldAttr.m_Value;
            oBhvr.attrNameLst->list.push_back(addAttr);

        }
    }

    FillTgtEl(oBhvr.tgtEl, pBhvr->m_oClientVisualElement);

    if (pBhvr->m_pPropertyList == nullptr)
        return;

    for (const auto prop : pBhvr->m_pPropertyList->m_arRecords)
    {
        if (prop == nullptr)
            continue;

        switch (prop->m_oHeader.RecInstance)
        {
        case TL_TBPID_RuntimeContext:
            break;
        case TL_TBPID_MotionPathEditRelative:
            break;
        case TL_TBPID_ColorColorModel:
            break;
        case TL_TBPID_ColorDirection:
            break;
        case TL_TBPID_Override:
        {
            auto override_ = new PPTX::Limit::TLOverride;
            override_->set(L"childStyle");
            oBhvr.override_= override_;
            break;
        }
        case TL_TBPID_PathEditRotationAngle:
            break;
        case TL_TBPID_PathEditRotationX:
            break;
        case TL_TBPID_PathEditRotationY:
            break;
        case TL_TBPID_PointsTypes:
            break;
        case TL_TBPID_UnknownPropertyList:
        default:
            break;
        }
    }
}

bool Timing_2010::isTextShape(UINT spid) const
{
    return slideAnim.nonTextShapes.find(spid) == slideAnim.nonTextShapes.end();
}

void Timing_2010::FillTgtEl(PPTX::Logic::TgtEl &tgtEl, CRecordClientVisualElementContainer& clientVisualElement)
{
    if (!clientVisualElement.m_bVisualShapeAtom)
        return;

    if (clientVisualElement.m_oVisualShapeAtom.m_RefType != TL_ET_ShapeType)
        return; // todo insert next types here

    UINT spid = clientVisualElement.
            m_oVisualShapeAtom.m_nObjectIdRef;

    if (!tgtEl.spTgt.IsInit())
    {
        tgtEl.spTgt = new PPTX::Logic::SpTgt;
        tgtEl.spTgt->spid = std::to_wstring(spid);
    }

    if (m_currentBldP)
    {
        m_currentBldP->spid =
                tgtEl.spTgt->spid;
    }

    auto& vsa = clientVisualElement.m_oVisualShapeAtom;
    if (vsa.m_Type == TL_TVET_TextRange &&
            vsa.m_nData1 != 0xffffffff && vsa.m_nData2 != 0xffffffff &&
            isTextShape(spid))
    {
        tgtEl.spTgt->txEl         = new PPTX::Logic::TxEl;
        tgtEl.spTgt->txEl->charRg = false;
        tgtEl.spTgt->txEl->st     = clientVisualElement.m_oVisualShapeAtom.m_nData1;
        tgtEl.spTgt->txEl->end    = clientVisualElement.m_oVisualShapeAtom.m_nData2;
    } else if (clientVisualElement.m_oVisualShapeAtom.m_Type == TL_TVET_ChartElement)
    {
        tgtEl.spTgt->type = new PPTX::Limit::TLChartSubElement;
        std::wstring chartBA;
        switch (clientVisualElement.m_oVisualShapeAtom.m_nData1) {
        case 0: chartBA = L"gridLegend"; break;
        case 1: chartBA = L"series"; break;
        case 2: chartBA = L"category"; break;
        case 3: chartBA = L"ptInSeries"; break;
        case 4: chartBA = L"ptInCategory"; break;
        case 5: chartBA = L"gridLegend"; break;
        }
        tgtEl.spTgt->type->set(chartBA);
        if (clientVisualElement.m_oVisualShapeAtom.m_nData2)
            tgtEl.spTgt->lvl = clientVisualElement.m_oVisualShapeAtom.m_nData2;
    }
}

void Timing_2010::FillCBhvr(PPTX::Logic::CBhvr &oBhvr, int dur, UINT spid, std::wstring attrname, int delay)
{
    oBhvr.cTn.id = cTnId++;
    oBhvr.cTn.fill = L"hold";
    oBhvr.cTn.dur = std::to_wstring(dur);
    if (delay > -1)
    {
        oBhvr.cTn.stCondLst = new PPTX::Logic::CondLst;
        oBhvr.cTn.stCondLst->node_name = L"stCondLst";
        PPTX::Logic::Cond cond;
        cond.delay = std::to_wstring(delay);
        oBhvr.cTn.stCondLst->list.push_back(cond);
    }

    oBhvr.tgtEl.spTgt = new PPTX::Logic::SpTgt;
    oBhvr.tgtEl.spTgt->spid = std::to_wstring(spid);

    if (!attrname.empty())
    {
        oBhvr.attrNameLst = new PPTX::Logic::AttrNameLst;
        PPTX::Logic::AttrName attrName;
        attrName.text = attrname;
        oBhvr.attrNameLst->list.push_back(attrName);
    }
}

void Timing_2010::FillSet(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::Set &oSet)
{
    if (!pETNC->m_haveSetBehavior)
        return;

    FillCBhvr(pETNC, oSet.cBhvr);
    oSet.to = new PPTX::Logic::AnimVariant();
    oSet.to->node_name = L"to";
    oSet.to->strVal = pETNC->m_pTimeSetBehavior->m_oVarTo.m_Value;
}

void Timing_2010::FillAnim(CRecordTimeAnimateBehaviorContainer *pTimeAnimateBehavior, PPTX::Logic::Anim &oAnim)
{
    if (pTimeAnimateBehavior->m_oAnimateBehaviorAtom.m_bCalcModePropertyUsed)
    {
        std::wstring calcmode;
        switch (pTimeAnimateBehavior->m_oAnimateBehaviorAtom.m_nCalcMode) {
        case 0: calcmode = L"discrete"; break;
        case 1: calcmode = L"lin"; break;
        case 2: calcmode = L"fmla"; break;
        }
        oAnim.calcmode = new PPTX::Limit::TLCalcMode;
        oAnim.calcmode = calcmode;
    }

    if (pTimeAnimateBehavior->m_oAnimateBehaviorAtom.m_bValueTypePropertyUsed)
    {
        std::wstring valueType;
        switch (pTimeAnimateBehavior->m_oAnimateBehaviorAtom.m_ValueType) {
        case TL_TABVT_Color: valueType = L"Color"; break;
        case TL_TABVT_Number: valueType = L"Number"; break;
        case TL_TABVT_String: valueType = L"String"; break;
        }
        if (!valueType.empty())
        {
            oAnim.valueType = new PPTX::Limit::TLValueType;
            oAnim.valueType = valueType;
        }
    }

    // By
    if (pTimeAnimateBehavior->m_oAnimateBehaviorAtom.m_bByPropertyUsed)
    {
        oAnim.by = pTimeAnimateBehavior->m_oVarBy.m_Value;
    }
    // To
    if (pTimeAnimateBehavior->m_oAnimateBehaviorAtom.m_bToPropertyUsed)
    {
        oAnim.to = pTimeAnimateBehavior->m_oVarTo.m_Value;
    }
    //From
    if (pTimeAnimateBehavior->m_oAnimateBehaviorAtom.m_bFromPropertyUsed)
    {
        oAnim.from = pTimeAnimateBehavior->m_oVarFrom.m_Value;
    }

    //// Writing childs

    if (!pTimeAnimateBehavior->m_oAnimateValueList.m_arrEntry.empty())
        oAnim.tavLst = new PPTX::Logic::TavLst;
    for (auto &animValue : pTimeAnimateBehavior->m_oAnimateValueList.m_arrEntry)
    {
        PPTX::Logic::Tav tav;
        tav.val = new PPTX::Logic::AnimVariant;

        tav.val->node_name = L"val";

        if (animValue->m_pVarValue.is_init())
            switch (animValue->m_pVarValue->m_Type) {
            case TL_TVT_String:
            {
                tav.val->strVal = dynamic_cast<const CRecordTimeVariantString&>
                        (animValue->m_pVarValue.get()).m_Value;
                break;
            }
            case TL_TVT_Bool:
            {
                tav.val->boolVal = dynamic_cast<const CRecordTimeVariantBool&>
                        (animValue->m_pVarValue.get()).m_Value;
                break;
            }
            case TL_TVT_Int:
            {
                tav.val->intVal = dynamic_cast<const CRecordTimeVariantInt&>
                        (animValue->m_pVarValue.get()).m_Value;
                break;
            }
            case TL_TVT_Float:
            {
                tav.val->fltVal = dynamic_cast<const CRecordTimeVariantFloat&>
                        (animValue->m_pVarValue.get()).m_Value;
                break;
            }
            }

        auto tavTime = animValue->m_oTimeAnimationValueAtom.m_nTime;
        if (tavTime <= 1000 && tavTime >= 0)    // todo check
            tav.tm = std::to_wstring((/*1000 - */tavTime) * 100);

        if (!animValue->m_VarFormula.m_Value.empty())
        {
            tav.fmla = animValue->m_VarFormula.m_Value;
        }

        oAnim.tavLst->list.push_back(tav);
    }
    FillCBhvr(&(pTimeAnimateBehavior->m_oBehavior), oAnim.cBhvr);
}

void Timing_2010::FillCTnRecursive(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::CTn &oCTn)
{
    if (cTNLevel+1 == TimeNodeLevel::oneAnim &&
            !CheckAnimation5Level(pETNC, oCTn))
        throw TimingExeption("Slide was edited without animation 2010 synchronization");

    cTNLevel++;

    FillCTnHeadArgs(pETNC, oCTn);

    ConvertChildTnLst(pETNC, oCTn);
    ConvertCTnIterate(pETNC, oCTn);
    ConvertCTnEndSync(pETNC, oCTn);
    ConvertCTnIterate(pETNC, oCTn);
    ConvertCTnStCondLst(pETNC, oCTn);

    cTNLevel--;
}

bool Timing_2010::CheckAnimation5Level(const CRecordExtTimeNodeContainer *pETNC, const PPTX::Logic::CTn &oCTn)
{
    auto anim_2010 = Intermediate::ParseExisting5Level_ETNC(pETNC);
    bool isSlideShape = CheckSlideSpid(anim_2010.spid);
    bool isSyncWithAnim95 = CheckMainSeqSyncWithAnim95(anim_2010.spid);
    return isSlideShape && isSyncWithAnim95;
}

bool Timing_2010::CheckSlideSpid(_INT32 spid)
{
    bool isSlideShape = IsSlideSpId(spid);
    if (isSlideShape)
        InsertAnimationSpId(spid);
    return isSlideShape;
}

bool Timing_2010::IsSlideSpId(_INT32 spid) const
{
    return slideAnim.realShapesIds.find(spid) != slideAnim.realShapesIds.end();
}

bool Timing_2010::CheckMainSeqSyncWithAnim95(_INT32 spid) const
{
    if (!isMainSeq)
        return true;

    return true;
}

bool Timing_2010::IsCorrectAnimationSpId(_INT32 spid) const
{
    return correctAnimatedShapes.find(spid) != correctAnimatedShapes.end();
}

void Timing_2010::InsertAnimationSpId(_INT32 spid)
{
    if (spid != -1)
        correctAnimatedShapes.insert(spid);
}

void Timing_2010::ConvertChildTnLst(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::CTn &oCTn)
{
    if (oCTn.childTnLst.IsInit() == false)
        oCTn.childTnLst = new PPTX::Logic::ChildTnLst;

    for (size_t i = 0; i < pETNC->m_arrRgExtTimeNodeChildren.size(); i++)
    {
        auto *pETNC_child = pETNC->m_arrRgExtTimeNodeChildren[i];
        if (oCTn.childTnLst->list.size() == i)
            oCTn.childTnLst->list.push_back(PPTX::Logic::TimeNodeBase());

        auto& pptx_child = oCTn.childTnLst->list[i];
        FillTnChild(pETNC_child, pptx_child);
    }
}

void Timing_2010::ConvertCTnIterate(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::CTn &oCTn)
{
    if (!pETNC->m_haveIterateDataAtom)
        return;

    auto *iter = pETNC->m_pTimeIterateDataAtom;
    oCTn.iterate = new PPTX::Logic::Iterate;

    std::wstring type[] = {L"el", L"wd", L"lt"};
    if (iter->m_fIterateTypePropertyUsed)
        oCTn.iterate->type = type[iter->m_nIterateType % 3];

    if (iter->m_fIterateDirectionPropertyUsed)
        oCTn.iterate->backwards = (bool)iter->m_nIterateDirection;

    int intervalType = iter->m_fIterateIntervalTypePropertyUsed ?
                iter->m_nIterateIntervalType : 0;
    unsigned int iterateInterval = iter->m_fIterateIntervalPropertyUsed ?
                iter->m_nIterateInterval : 0;

    if (intervalType)
        oCTn.iterate->tmPct = iterateInterval > 1000 ? 10000 : iterateInterval * 10;
    else
        oCTn.iterate->tmAbs = std::to_wstring(iterateInterval);
}

void Timing_2010::ConvertCTnEndSync(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::CTn &oCTn)
{
    if (!pETNC->m_haveTimeEndSyncTime)
        return;

    auto *sync = pETNC->m_pTimeEndSyncTimeCondition;
    oCTn.endSync = new PPTX::Logic::Cond;
    oCTn.endSync->node_name = L"endSync";
    FillCond(sync, *(oCTn.endSync));
}

void Timing_2010::ConvertCTnSubTnLst(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::CTn &oCTn)
{
    if (pETNC->m_arrRgSubEffect.empty())
        return;

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

void Timing_2010::ConvertCTnStCondLst(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::CTn &oCTn)
{
    if (pETNC->m_arrRgBeginTimeCondition.empty())
        return;

    oCTn.stCondLst = new PPTX::Logic::CondLst;
    oCTn.stCondLst->node_name = L"stCondLst";
    FillStCondLst(pETNC->m_arrRgBeginTimeCondition, oCTn.stCondLst.get2());
}

void Timing_2010::FillCond(CRecordTimeConditionContainer *oldCond, PPTX::Logic::Cond &cond)
{
    if (oldCond->m_oTimeConditionAtom.m_nTimeDelay != 0xFFFFFFFF)
        cond.delay = std::to_wstring(oldCond->m_oTimeConditionAtom.m_nTimeDelay);
    else
        cond.delay = L"indefinite";

    //    if (oldCond->m_oTimeConditionAtom.m_TriggerObject == TL_TOT_RuntimeNodeRef ||
    //            oldCond->m_oTimeConditionAtom.m_TriggerObject == TL_TOT_TimeNode)
    //    {
    //        cond.tn = oldCond->m_oTimeConditionAtom.m_nID;
    //    }

    if (oldCond->m_oTimeConditionAtom.m_TriggerObject == TL_TOT_RuntimeNodeRef)
    {
        cond.rtn = new PPTX::Limit::TLRuntimeTrigger;
        cond.rtn->SetBYTECode(0);
    }

    std::wstring str;

    switch (oldCond->m_oTimeConditionAtom.m_nTriggerEvent)
    {
    case 1:     str =   L"onBegin"; break;
    case 3:     str =   L"begin"; break;
    case 4:     str =   L"end"; break;
    case 5:     str =   L"onClick"; break;
    case 7:     str =   L"onMouseOver"; break;
    case 9:     str =   L"onNext"; break;
    case 10:    str =   L"onPrev"; break;
    case 11:    str =   L"onStopAudio"; break;
    default:    str.clear();
    }

    if (!str.empty()) cond.evt = str;

    if (oldCond->m_oVisualElement.m_bVisualShapeAtom)
    {
        cond.tgtEl = new PPTX::Logic::TgtEl;
        cond.tgtEl->spTgt = new PPTX::Logic::SpTgt;
        cond.tgtEl->spTgt->spid = std::to_wstring(
                    oldCond->m_oVisualElement.m_oVisualShapeAtom.m_nObjectIdRef);
    } else if (oldCond->m_oVisualElement.m_bVisualPageAtom)
    {
        cond.tgtEl = new PPTX::Logic::TgtEl;
    }
}

void Timing_2010::FillStCondLst(const std::vector<CRecordTimeConditionContainer *> &timeCondCont, PPTX::Logic::CondLst &stCondLst)
{
    for (const auto& pCond : timeCondCont)
    {
        int target = -1;
        if (pCond->m_oVisualElement.m_bVisualShapeAtom)
            target = pCond->m_oVisualElement.m_oVisualShapeAtom.m_nObjectIdRef;

        PPTX::Logic::Cond cond;
        FillCond(pCond, cond);

        stCondLst.list.push_back(cond);
    }
}

void Timing_2010::FillSubTnLst(std::vector<CRecordSubEffectContainer *> &vecSEC, PPTX::Logic::TnLst &oSubTnLst)
{
    for (auto pSEC : vecSEC)
    {
        if (pSEC->m_haveClientVisualElement)
        {
            PPTX::Logic::TimeNodeBase TNB;
            auto audio = new PPTX::Logic::Audio;

            FillAudio(pSEC->m_pClientVisualElement, *audio);
            TNB.m_node = audio;
            oSubTnLst.list.push_back(TNB);

            // Write endCondLst
            if (pSEC->m_arrRgEndTimeCondition.empty() == false)
            {
                audio->cMediaNode.cTn.endCondLst = new PPTX::Logic::CondLst;
                audio->cMediaNode.cTn.endCondLst->node_name = L"endCondLst";
            }
            FillCondLst(pSEC->m_arrRgEndTimeCondition,
                        audio->cMediaNode.cTn.endCondLst.get2());

            // Write stCondLst
            if (pSEC->m_arrRgBeginTimeCondition.empty() == false)
            {
                audio->cMediaNode.cTn.stCondLst = new PPTX::Logic::CondLst;
                audio->cMediaNode.cTn.stCondLst->node_name = L"stCondLst";
            }
            FillCondLst(pSEC->m_arrRgBeginTimeCondition,
                        audio->cMediaNode.cTn.stCondLst.get2());
            FillCTnProps(pSEC->m_pTimePropertyList, audio->cMediaNode.cTn);
        }

        if (pSEC->m_haveColorBehavior)
        {
            PPTX::Logic::TimeNodeBase TNB;
            auto color = new PPTX::Logic::AnimClr;

            FillAnimClr(pSEC->m_pTimeColorBehavior, pSEC->m_pTimePropertyList, *color);
            TNB.m_node = color;
            oSubTnLst.list.push_back(TNB);
        }
    }
}

void Timing_2010::FillAudio(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::Audio &oAudio)
{
    auto* pCVEC = pETNC->m_pClientVisualElement;
    if (pCVEC->m_bVisualShapeAtom && m_pRels)
    {
        CExFilesInfo* pInfo1 = m_pExMedia->LockAudioFromCollection(pCVEC->m_oVisualShapeAtom.m_nObjectIdRef);
        if (pInfo1)
        {
            bool bExternal(false);
            oAudio.cMediaNode.tgtEl.embed =
                    new OOX::RId(m_pRels->WriteAudio(pInfo1->m_strFilePath, bExternal));
            oAudio.cMediaNode.tgtEl.name = XmlUtils::EncodeXmlString(pInfo1->m_name);
        } else if (pCVEC->m_oVisualShapeAtom.m_RefType == TL_ET_ShapeType)
        {
            oAudio.cMediaNode.tgtEl.spTgt = new PPTX::Logic::SpTgt;
            oAudio.cMediaNode.tgtEl.spTgt->spid = std::to_wstring(pCVEC->m_oVisualShapeAtom.m_nObjectIdRef);
            //            oAudio.isNarration = true;
            //            oAudio.cMediaNode.showWhenStopped = false;
        } else
            return;
        FillCTnRecursive(pETNC, oAudio.cMediaNode.cTn);
    }
}

void Timing_2010::FillAudio(CRecordClientVisualElementContainer *pCVEC, PPTX::Logic::Audio &oAudio)
{
    if (pCVEC->m_bVisualShapeAtom)
    {
        CExFilesInfo* pInfo1 = m_pExMedia->LockAudioFromCollection(pCVEC->m_oVisualShapeAtom.m_nObjectIdRef);
        if (pInfo1 && m_pRels)
        {
            bool bExternal(false);
            oAudio.cMediaNode.tgtEl.embed =
                    new OOX::RId(m_pRels->WriteAudio(pInfo1->m_strFilePath, bExternal));
            oAudio.cMediaNode.tgtEl.name = XmlUtils::EncodeXmlString(pInfo1->m_name);
        }
    }
}

void Timing_2010::FillCmd(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::Cmd &oCmd)
{
    if (!pETNC || !pETNC->m_pTimeCommandBehavior)
        return;

    auto pCommand = pETNC->m_pTimeCommandBehavior;
    const auto& oAtom =    pCommand->m_oCommandBehaviorAtom;

    FillCBhvr(pETNC, oCmd.cBhvr);

    if (oAtom.m_fTypePropertyUsed)
    {
        oCmd.type = new PPTX::Limit::TLCommandType;
        std::wstring type;
        switch (oAtom.m_eCommandBehaviorType)
        {
        case TL_TCBT_Eventv:    type = L"evt";  break;
        case TL_TCBT_Call:      type = L"call"; break;
        case TL_TCBT_OleVerb:   type = L"verb"; break;
        }
        oCmd.type->set(type);
    }
    if (oAtom.m_fCommandPropertyUsed)
    {
        oCmd.cmd = pCommand->m_oVarCommand.m_Value;
    }
}

void Timing_2010::FillCondLst(std::vector<CRecordTimeConditionContainer *> &oCondVec, PPTX::Logic::CondLst &oCondLst)
{
    for (auto *oldCond : oCondVec) {
        PPTX::Logic::Cond cond;
        cond.node_name = L"cond";
        FillCond(oldCond, cond);
        oCondLst.list.push_back(cond);
    }
}

void Timing_2010::FillEmptyTargetCond(PPTX::Logic::Cond &cond)
{
    cond.tgtEl = new PPTX::Logic::TgtEl;
}

void Timing_2010::FillCTnProps(CRecordTimePropertyList4TimeNodeContainer *pProp, PPTX::Logic::CTn &oCTn)
{
    if (pProp == nullptr || pProp->m_bEmtyNode)
        return;
    for (auto *pRec : pProp->m_arrElements)
    {
        TimePropertyID4TimeNode VariableType = ( TimePropertyID4TimeNode ) pRec->m_oHeader.RecInstance;

        switch ( VariableType )
        {
        case TL_TPID_Display:
        {
            oCTn.display = !(bool)dynamic_cast<CRecordTimeDisplayType*>(pRec)->m_Value;
            break;
        }
        case TL_TPID_MasterPos:
        {
            oCTn.masterRel = new PPTX::Limit::TLMasterRelation;
            oCTn.masterRel = dynamic_cast<CRecordTimeMasterRelType*>(pRec)->m_Value ?
                        L"nextClick" : L"sameClick";
            break;
        }
        case TL_TPID_SubType:			break;
        case TL_TPID_EffectID:
        {
            oCTn.presetID = dynamic_cast<CRecordTimeEffectID*>(pRec)->m_Value;
            break;
        }
        case TL_TPID_EffectDir:
        {
            oCTn.presetSubtype = dynamic_cast<CRecordTimeEffectDir*>(pRec)->m_Value;
            break;
        }
        case TL_TPID_EffectType:
        {
            // Write presetClass
            std::wstring presetClass;
            switch (dynamic_cast<CRecordTimeEffectType*>(pRec)->m_Value) {
            case 0: break;
            case 1: presetClass = L"entr";      break;
            case 2: presetClass = L"exit";      break;
            case 3: presetClass = L"emph";      break;
            case 4: presetClass = L"path";      break;
            case 5: presetClass = L"verb";      break;
            case 6: presetClass = L"mediacall"; break;
            }
            if (!presetClass.empty())
            {
                oCTn.presetClass = new PPTX::Limit::TLPresetClass;
                oCTn.presetClass = presetClass;
            }
            break;
        }
        case TL_TPID_AfterEffect:
        {
            oCTn.afterEffect = (bool)dynamic_cast<CRecordTimeAfterEffect*>(pRec)->m_Value;
            break;
        }
        case TL_TPID_SlideCount:		break;
        case TL_TPID_TimeFilter:
        {
            oCTn.tmFilter = dynamic_cast<CRecordTimeNodeTimeFilter*>(pRec)->m_Value;
            break;
        }
        case TL_TPID_EventFilter:
        {
            oCTn.evtFilter = dynamic_cast<CRecordTimeEventFilter*>(pRec)->m_Value;
            break;
        }
        case TL_TPID_HideWhenStopped:
            break;
        case TL_TPID_GroupID:
        {
            oCTn.grpId = dynamic_cast<CRecordTimeGroupID*>(pRec)->m_Value;
            break;
        }
        case TL_TPID_EffectNodeType:
        {
            // Write nodeType
            std::wstring nodeType;
            switch (dynamic_cast<CRecordTimeEffectNodeType*>(pRec)->m_Value)
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
            {
                oCTn.nodeType = new PPTX::Limit::TLNodeType;
                oCTn.nodeType = nodeType;
            }

            break;
        }
        case TL_TPID_PlaceholderNode:
        {
            oCTn.nodePh = (bool)dynamic_cast<CRecordTimePlaceholderNode*>(pRec)->m_Value;
            break;
        }
        case TL_TPID_MediaVolume:
            break;
        case TL_TPID_MediaMute:
            break;
        case TL_TPID_ZoomToFullScreen:
            break;
        default :
            break;
        }
    }
}

void Timing_2010::FillCTnHeadArgs(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::CTn &oCTn)
{
    oCTn.id = cTnId++;

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
        if (oTimeNodeAtom.m_nDuration == 0xFFFFFFFF)
            oCTn.dur = L"indefinite";
        else
            oCTn.dur = std::to_wstring(oTimeNodeAtom.m_nDuration);
    }

    if (pETNC->m_haveTimePropertyList && !pETNC->m_pTimePropertyList->m_bEmtyNode)
        FillCTnProps(pETNC->m_pTimePropertyList, oCTn);

    if (cTNLevel == TimeNodeLevel::mainSeqOrTrigger)
        isMainSeq = oCTn.nodeType.get_value_or(L"") == L"mainSeq";

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
}

void Timing_2010::FillAnimClr(
        CRecordTimeColorBehaviorContainer *pColor,
        CRecordTimePropertyList4TimeNodeContainer *pProp,
        PPTX::Logic::AnimClr &oAnimClr)
{
    auto &clrAtom = pColor->m_oColorBehaviorAtom;

    FillCBhvr(&(pColor->m_oBehavior), oAnimClr.cBhvr);
    FillCTnProps(pProp, oAnimClr.cBhvr.cTn);

    // Write Attributes
    if (pColor->m_oBehavior.m_havePropertyList){
        for (auto pRec : pColor->m_oBehavior.m_pPropertyList->m_arRecords)
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
            if (index >= 4 && index < 10)
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
            if (index >= 4 && index < 10)
            {
                strVal = L"accent" + std::to_wstring(index - 3);
            }
            pScheme->val = strVal;
            oAnimClr.to.Color = smart_ptr<PPTX::Logic::ColorBase>(pScheme);
        }
    }
}

void Timing_2010::FillAnimEffect(
        CRecordExtTimeNodeContainer *pETNC,
        PPTX::Logic::AnimEffect &oAnim)
{
    auto *bhvr = pETNC->m_pTimeEffectBehavior;
    oAnim.filter = bhvr->m_oVarType.m_Value;

    if (bhvr->m_effectBehaviorAtom.m_bProgressPropertyUsed)
    {
        if (!oAnim.progress.is_init())
            oAnim.progress = new PPTX::Logic::AnimVariant;
        oAnim.progress->fltVal = bhvr->m_oVarProgress.m_Value;
    }

    if (bhvr->m_effectBehaviorAtom.m_bRuntimeContextObsolete)
    {
        if (!oAnim.progress.is_init())
            oAnim.progress = new PPTX::Logic::AnimVariant;
        oAnim.progress->strVal = bhvr->m_oVarRuntimeContext.m_Value;
    }

    if (bhvr->m_effectBehaviorAtom.m_bTransitionPropertyUsed)
    {
        oAnim.transition = bhvr->m_effectBehaviorAtom.m_nEffectTransition ?
                    L"out" : L"in";
    }

    FillCBhvr(pETNC, oAnim.cBhvr);

}

void Timing_2010::FillAnimMotion(
        CRecordExtTimeNodeContainer *pETNC,
        PPTX::Logic::AnimMotion &oAnim)
{
    if (!pETNC || !pETNC->m_pTimeMotionBehavior)
        return;

    auto pMotion = pETNC->m_pTimeMotionBehavior;
    const auto& oAtom = pMotion->m_oMotionBehaviorAtom;

    FillCBhvr(pETNC, oAnim.cBhvr);

    if (oAtom.m_bFromPropertyUsed)
    {
        oAnim.fromX = oAtom.m_nXFROM;
        oAnim.fromY = oAtom.m_nYFROM;
    }
    if (oAtom.m_bToPropertyUsed)
    {
        oAnim.toX   = oAtom.m_nXTO;
        oAnim.toY   = oAtom.m_nYTO;
    }
    if (oAtom.m_bByPropertyUsed)
    {
        oAnim.byX   = oAtom.m_nXBY;
        oAnim.byY   = oAtom.m_nYBY;
    }

    if (oAtom.m_bOriginPropertyUsed)
    {
        oAnim.origin = new PPTX::Limit::TLOrigin;
        oAnim.origin->set(oAtom.m_nBehaviorOrigin == 2 ?
                              L"layout" : L"parent");
    }

    if (!pMotion->m_pVarPath->m_Value.empty())
        oAnim.path = pMotion->m_pVarPath->m_Value;

    //    oAnim.ptsTypes


    oAnim.pathEditMode = new PPTX::Limit::TLPathEditMode;
    oAnim.pathEditMode->set(oAtom.m_bEditRotationPropertyUsed ? L"fixed" : L"relative");
}

void Timing_2010::FillAnimRot(
        CRecordExtTimeNodeContainer *pETNC,
        PPTX::Logic::AnimRot &oAnim)
{
    if (!pETNC || !pETNC->m_pTimeRotationBehavior) return;

    auto pRot = pETNC->m_pTimeRotationBehavior;
    const auto& oAtom = pRot->m_oRotationBehaviorAtom;

    FillCBhvr(pETNC, oAnim.cBhvr);

    const auto mult = 60000;

    if (oAtom.m_fByPropertyUsed)
        oAnim.by = oAtom.m_By  *mult;
    if (oAtom.m_fToPropertyUsed)
        oAnim.to = oAtom.m_To  *mult;
    if (oAtom.m_fFromPropertyUsed)
        oAnim.from = oAtom.m_From  *mult;

}

void Timing_2010::FillAnimScale(
        CRecordExtTimeNodeContainer *pETNC,
        PPTX::Logic::AnimScale &oAnim)
{
    if (!pETNC || !pETNC->m_pTimeScaleBehavior) return;

    auto pScale = pETNC->m_pTimeScaleBehavior;
    const auto& oAtom =  pScale->m_oScaleBehaviorAtom;

    FillCBhvr(pETNC, oAnim.cBhvr);

    const auto mult = 1000;
    if (oAtom.m_fByPropertyUsed)
    {
        oAnim.byX = oAtom.m_XBy  *mult;
        oAnim.byY = oAtom.m_YBy  *mult;
    }
    if (oAtom.m_fToPropertyUsed)
    {
        oAnim.toX = oAtom.m_XTo  *mult;
        oAnim.toY = oAtom.m_YTo  *mult;
    }
    if (oAtom.m_fFromPropertyUsed)
    {
        oAnim.fromX = oAtom.m_XFrom  *mult;
        oAnim.fromY = oAtom.m_YFrom  *mult;
    }

    if (oAtom.m_fZoomContentsUsed)
        oAnim.zoomContents = oAtom.m_fZoomContents;
}



void Timing_2010::FillVideo(
        CRecordExtTimeNodeContainer* pETNC,
        PPTX::Logic::Video& oVideo)
{
    const auto& video = pETNC->m_pClientVisualElement->m_oVisualShapeAtom;

    FillCTnRecursive(pETNC, oVideo.cMediaNode.cTn);

    if (pETNC->m_pTimePropertyList->m_arrElements.size() >= 5)
    {
        try {
            oVideo.cMediaNode.vol = (int)(static_cast<CRecordTimeVariantFloat*>
                                          (pETNC->m_pTimePropertyList->m_arrElements[1])->
                    m_Value * 100000);
            oVideo.cMediaNode.mute = static_cast<CRecordTimeVariantBool*>
                    (pETNC->m_pTimePropertyList->m_arrElements[2])->
                    m_Value;
            oVideo.fullScrn = static_cast<CRecordTimeVariantBool*>
                    (pETNC->m_pTimePropertyList->m_arrElements[3])->
                    m_Value;
            oVideo.cMediaNode.showWhenStopped = static_cast<CRecordTimeVariantBool*>
                    (pETNC->m_pTimePropertyList->m_arrElements[4])->
                    m_Value;
        } catch (...) {

        }
    }


    oVideo.cMediaNode.tgtEl.spTgt = new PPTX::Logic::SpTgt();
    oVideo.cMediaNode.tgtEl.spTgt->spid = std::to_wstring(video.m_nObjectIdRef);
}

}
}
