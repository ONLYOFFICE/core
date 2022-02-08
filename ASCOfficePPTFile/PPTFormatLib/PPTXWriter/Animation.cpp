/*
  *(c) Copyright Ascensio System SIA 2010-2019
 *
  *This program is a free software product. You can redistribute it and/or
  *modify it under the terms of the GNU Affero General Public License (AGPL)
  *version 3 as published by the Free Software Foundation. In accordance with
  *Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
  *that Ascensio System SIA expressly excludes the warranty of non-infringement
  *of any third-party rights.
 *
  *This program is distributed WITHOUT ANY WARRANTY; without even the implied
  *warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
  *details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
  *You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
  *street, Riga, Latvia, EU, LV-1050.
 *
  *The  interactive user interfaces in modified source and object code versions
  *of the Program must display Appropriate Legal Notices, as required under
  *Section 5 of the GNU AGPL version 3.
 *
  *Pursuant to Section 7(b) of the License you must retain the original Product
  *logo when distributing the program. Pursuant to Section 7(e) we decline to
  *grant you any rights under trademark law for use of our trademarks.
 *
  *All the Product's GUI elements, including illustrations and icon sets, as
  *well as technical writing content are licensed under the terms of the
  *Creative Commons Attribution-ShareAlike 4.0 International. See the License
  *terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "Animation.h"

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/SchemeClr.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/SrgbClr.h"

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldLst.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldOleChart.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldDgm.h"

#include "../Records/Animations/BuildListContainer.h"
#include "../Records/Animations/ChartBuildContainer.h"
#include "../Records/Animations/DiagramBuildContainer.h"

#include "../Records/SoundContainer.h"
//#include <algorithm>

using namespace PPT_FORMAT;

void Animation::Convert(PPTX::Logic::Timing &oTiming)
{
    if (m_pPPT10)
    {
        // It must be first to write some reference from ExtTimeNodeContainer
        if (m_pPPT10->m_haveBuildList && !m_pPPT10->m_pBuildListContainer->n_arrRgChildRec.empty())
        {
            oTiming.bldLst = new PPTX::Logic::BldLst();
            FillBldLst(m_pPPT10->m_pBuildListContainer, *(oTiming.bldLst));
            m_pBldLst = oTiming.bldLst.GetPointer();
        }

        if (m_pPPT10->m_haveExtTime)
        {
            oTiming.tnLst = new PPTX::Logic::TnLst();
            FillTnLst(m_pPPT10->m_pExtTimeNodeContainer, *(oTiming.tnLst));
        }
    }
    if (!m_arrOldAnim.empty() || m_isPPT10Broken)
    {
        oTiming = PPTX::Logic::Timing();
        InitTimingTags(oTiming);
    }

    return;
}

void Animation::FillAnim(
        CRecordTimeAnimateBehaviorContainer *pTimeAnimateBehavior,
        PPTX::Logic::Anim &oAnim)
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
        if (tavTime < 1000 && tavTime >= 0)
                tav.tm = std::to_wstring((1000 - tavTime) * 100);

        if (!animValue->m_VarFormula.m_Value.empty())
        {
            tav.fmla = animValue->m_VarFormula.m_Value;
        }

        oAnim.tavLst->list.push_back(tav);
    }
    FillCBhvr(&(pTimeAnimateBehavior->m_oBehavior), oAnim.cBhvr);
}

void Animation::FillAnimClr(
        CRecordTimeColorBehaviorContainer *pColor,
        CRecordTimePropertyList4TimeNodeContainer *pProp,
        PPTX::Logic::AnimClr &oAnimClr)
{
    auto &clrAtom = pColor->m_oColorBehaviorAtom;

    FillCBhvr(&(pColor->m_oBehavior), oAnimClr.cBhvr);
    FillCTn(pProp, oAnimClr.cBhvr.cTn);

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
            oAnimClr.to.Color = pScheme;
        }
    }
}

void Animation::FillAnimEffect(
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

void Animation::FillAnimMotion(
        CRecordExtTimeNodeContainer *pETNC,
        PPTX::Logic::AnimMotion &oAnim)
{
    if (!pETNC || !pETNC->m_pTimeMotionBehavior) return;

    auto pMotion = pETNC->m_pTimeMotionBehavior;
    auto oAtom = pMotion->m_oMotionBehaviorAtom;

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

void Animation::FillAnimRot(
        CRecordExtTimeNodeContainer *pETNC,
        PPTX::Logic::AnimRot &oAnim)
{
    if (!pETNC || !pETNC->m_pTimeRotationBehavior) return;

    auto pRot = pETNC->m_pTimeRotationBehavior;
    auto oAtom = pRot->m_oRotationBehaviorAtom;

    FillCBhvr(pETNC, oAnim.cBhvr);

    const auto mult = 60000;

    if (oAtom.m_fByPropertyUsed)
        oAnim.by = oAtom.m_By  *mult;
    if (oAtom.m_fToPropertyUsed)
        oAnim.to = oAtom.m_To  *mult;
    if (oAtom.m_fFromPropertyUsed)
        oAnim.from = oAtom.m_From  *mult;

}

void Animation::FillAnimScale(
        CRecordExtTimeNodeContainer *pETNC,
        PPTX::Logic::AnimScale &oAnim)
{
    if (!pETNC || !pETNC->m_pTimeScaleBehavior) return;

    auto pScale = pETNC->m_pTimeScaleBehavior;
    auto oAtom =  pScale->m_oScaleBehaviorAtom;

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

// TODO refactoring
void Animation::FillAudio(CRecordExtTimeNodeContainer *pETNC,
                          PPTX::Logic::Audio &oAudio)
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
        } else
            return;
        FillCTn(pETNC, oAudio.cMediaNode.cTn);
    }
}

void Animation::FillAudio(CRecordClientVisualElementContainer *pCVEC,
                          PPTX::Logic::Audio &oAudio)
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

void Animation::FillVideo(
        CRecordExtTimeNodeContainer* pETNC,
        PPTX::Logic::Video& oVideo)
{
    auto video = pETNC->m_pClientVisualElement->m_oVisualShapeAtom;

    FillCTn(pETNC, oVideo.cMediaNode.cTn);

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


    oVideo.cMediaNode.tgtEl.spTgt = PPTX::Logic::SpTgt();
    oVideo.cMediaNode.tgtEl.spTgt->spid = std::to_wstring(video.m_nObjectIdRef);


}

void Animation::FillBldLst(
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
            CRecordParaBuildContainer *pRec =
                    (CRecordParaBuildContainer*)pBLC->n_arrRgChildRec[i];
            PPTX::Logic::BldP *pBldP = new PPTX::Logic::BldP();
            FillBldP(pRec, *pBldP);

            oBuildNodeBase.m_node = pBldP;
            break;
        }
        case RT_ChartBuild:
        {
            CRecordChartBuildContainer *pRec =
                    (CRecordChartBuildContainer*)pBLC->n_arrRgChildRec[i];
            PPTX::Logic::BldOleChart *pBldC = new PPTX::Logic::BldOleChart();

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
            CRecordDiagramBuildContainer *pRec =
                    (CRecordDiagramBuildContainer*)pBLC->n_arrRgChildRec[i];
            PPTX::Logic::BldDgm *pBldD = new PPTX::Logic::BldDgm();

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

void Animation::FillBldP(PPT_FORMAT::CRecordParaBuildContainer *pPBC,
                         PPTX::Logic::BldP &oBP)
{

    oBP.spid      = std::to_wstring(pPBC->m_oBuildAtom.m_nShapeIdRef);
    oBP.grpId     = (int)pPBC->m_oBuildAtom.m_nBuildId;
    oBP.uiExpand  = pPBC->m_oBuildAtom.m_fExpanded;

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

void Animation::FillCBhvr(
        CRecordExtTimeNodeContainer *pETNC,
        PPTX::Logic::CBhvr &oBhvr)
{
    CRecordTimeBehaviorContainer *bhvr = nullptr;
    if      (pETNC->m_haveSetBehavior)      bhvr = &(pETNC->m_pTimeSetBehavior->m_oBehavior);
    else if (pETNC->m_haveEffectBehavior)   bhvr = &(pETNC->m_pTimeEffectBehavior->m_oBehavior);
    else if (pETNC->m_haveAnimateBehavior)  bhvr = &(pETNC->m_pTimeAnimateBehavior->m_oBehavior);
    else if (pETNC->m_haveColorBehavior)    bhvr = &(pETNC->m_pTimeColorBehavior->m_oBehavior);
    else if (pETNC->m_haveMotionBehavior)   bhvr = &(pETNC->m_pTimeMotionBehavior->m_oTimeBehavior);
    else if (pETNC->m_haveRotationBehavior) bhvr = &(pETNC->m_pTimeRotationBehavior->m_oBehavior);
    else if (pETNC->m_haveScaleBehavior)    bhvr = &(pETNC->m_pTimeScaleBehavior->m_oBehavior);
    else                                    bhvr = &(pETNC->m_pTimeCommandBehavior->m_oBevavior);

    FillCBhvr(bhvr, oBhvr);

    FillCTn(pETNC, oBhvr.cTn);

}


void Animation::FillCBhvr(
        CRecordTimeBehaviorContainer *pBhvr,
        PPTX::Logic::CBhvr &oBhvr)
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

    if (pBhvr->m_oClientVisualElement.m_bVisualShapeAtom)
    {
        UINT spid = pBhvr->
                m_oClientVisualElement.
                m_oVisualShapeAtom.m_nObjectIdRef;

        if (isSpidReal(spid) == false)
        {
            m_isPPT10Broken = true;
        }

        oBhvr.tgtEl.spTgt = new PPTX::Logic::SpTgt();
        oBhvr.tgtEl.spTgt->spid =
                std::to_wstring(spid);
        if (m_currentBldP)
        {
            m_currentBldP->spid =
                    oBhvr.tgtEl.spTgt->spid;
        }
        if (pBhvr->m_oClientVisualElement.m_oVisualShapeAtom.m_nData2 != 0xFFFFFFFF &&
                pBhvr->m_oClientVisualElement.m_oVisualShapeAtom.m_nData1 != 0xFFFFFFFF)
        {
            oBhvr.tgtEl.spTgt->txEl         = new PPTX::Logic::TxEl;
            oBhvr.tgtEl.spTgt->txEl->charRg = false;
            oBhvr.tgtEl.spTgt->txEl->st     = pBhvr->m_oClientVisualElement.m_oVisualShapeAtom.m_nData1;
            oBhvr.tgtEl.spTgt->txEl->end    = pBhvr->m_oClientVisualElement.m_oVisualShapeAtom.m_nData2;
        }
    }
}
void Animation::FillCBhvr(
        PPTX::Logic::CBhvr &oBhvr, int dur,
        UINT spid, std::wstring attrname, int delay)
{
    oBhvr.cTn.id = m_cTnId++;
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
void Animation::FillCmd(
        CRecordExtTimeNodeContainer *pETNC,
        PPTX::Logic::Cmd &oCmd)
{
    if (!pETNC || !pETNC->m_pTimeCommandBehavior) return;

    auto pCommand = pETNC->m_pTimeCommandBehavior;
    auto oAtom =    pCommand->m_oCommandBehaviorAtom;

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

void Animation::FillCond(
        PPT_FORMAT::CRecordTimeConditionContainer *oldCond,
        PPTX::Logic::Cond &cond)
{
    if (oldCond->m_oTimeConditionAtom.m_nTimeDelay != -1)
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
    }
}


// Not called
void Animation::FillCTn(
        CRecordExtTimeNodeContainer *pETNC,
        PPTX::Logic::CTn &oCTn)
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

        if (iter->m_fIterateIntervalTypePropertyUsed)
            oCTn.iterate->tmPct = iter->m_nIterateInterval;

        if (iter->m_fIterateIntervalPropertyUsed)
            oCTn.iterate->tmAbs = std::to_wstring(iter->m_nIterateIntervalType);

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

void Animation::FillStCondLst(const std::vector<CRecordTimeConditionContainer *> &timeCondCont,
                              PPTX::Logic::CondLst &stCondLst)
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

void Animation::FillPar(
        CRecordExtTimeNodeContainer *pETNC,
        PPTX::Logic::Par &oPar)
{
    FillCTn(pETNC, oPar.cTn);
}

void Animation::FillSeq(
        PPT_FORMAT::CRecordExtTimeNodeContainer *pETNC,
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

void Animation::FillSet(
        PPT_FORMAT::CRecordExtTimeNodeContainer *pETNC,
        PPTX::Logic::Set& oSet)
{
    if (!pETNC->m_haveSetBehavior) return;

    // TODO
    FillCBhvr(pETNC, oSet.cBhvr);
    oSet.to = new PPTX::Logic::AnimVariant();
    oSet.to->node_name = L"to";
    oSet.to->strVal = pETNC->m_pTimeSetBehavior->m_oVarTo.m_Value;

}

void Animation::FillTnChild(
        CRecordExtTimeNodeContainer *pETNC,
        PPTX::Logic::TimeNodeBase &oChild)
{
    if (pETNC->m_haveSequenceAtom)
    {
        auto seq = new PPTX::Logic::Seq;
        FillSeq(pETNC, *seq);
        oChild.m_node = seq;
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

void Animation::FillTnLst(
        CRecordExtTimeNodeContainer *pETNC,
        PPTX::Logic::TnLst &oTnLst)
{
    if (!pETNC)
        return;

    PPTX::Logic::TimeNodeBase oChildTimeNodeBase;
    FillTnChild(pETNC, oChildTimeNodeBase);
    oTnLst.list.push_back(oChildTimeNodeBase);
}

void Animation::FillSubTnLst (
        std::vector<CRecordSubEffectContainer*> &vecSEC,
        PPTX::Logic::TnLst &oSubTnLst)
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
            FillCTn(pSEC->m_pTimePropertyList, audio->cMediaNode.cTn);
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

void Animation::FillCondLst(
        std::vector<CRecordTimeConditionContainer*>& oCondVec,
        PPTX::Logic::CondLst &oCondLst)
{
    for (auto *oldCond : oCondVec) {
        PPTX::Logic::Cond cond;
        cond.node_name = L"cond";
        FillCond(oldCond, cond);
        oCondLst.list.push_back(cond);
    }
}

void Animation::FillEmptyTargetCond(PPTX::Logic::Cond &cond)
{
    cond.tgtEl = new PPTX::Logic::TgtEl;
    // p:sldTgt will be pasted by default
}

void Animation::FillCTn(
        CRecordTimePropertyList4TimeNodeContainer *pProp,
        PPTX::Logic::CTn &oCTn)
{
    if (pProp && !pProp->m_bEmtyNode)
    {
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
            case TL_TPID_HideWhenStopped:	break;
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
            case TL_TPID_MediaVolume:		break;
            case TL_TPID_MediaMute:			break;
            case TL_TPID_ZoomToFullScreen:	break;
            default :
                break;
            }
        }
    }
}

void Animation::InitTimingTags(PPTX::Logic::Timing &oTiming)
{
    // p:bldLst
    oTiming.bldLst = new PPTX::Logic::BldLst();
    for (auto oldAnim : m_arrOldAnim)
    {
        PPTX::Logic::BuildNodeBase oBuildNodeBase;
        PPTX::Logic::BldP *pBldP = new PPTX::Logic::BldP();
        pBldP->spid = std::to_wstring(oldAnim.shapeId);
        pBldP->grpId = false;
        pBldP->animBg = (bool)(oldAnim.anim->m_AnimationAtom.m_fAnimateBg != 0);

        oBuildNodeBase.m_node = pBldP;
        oTiming.bldLst->list.push_back(oBuildNodeBase);
    }

    // p:tnLst
    oTiming.tnLst = new PPTX::Logic::TnLst();

    auto par1 = new PPTX::Logic::Par;
    par1->cTn.id = m_cTnId++;
    par1->cTn.dur = L"indefinite";
    par1->cTn.restart = L"never";
    par1->cTn.nodeType = L"tmRoot";

    auto seq2 = new PPTX::Logic::Seq;
    seq2->cTn.id = m_cTnId++;
    seq2->cTn.dur = L"indefinite";
    seq2->cTn.nodeType = L"mainSeq";
    seq2->concurrent = L"1";
    seq2->nextAc = L"seek";
    seq2->cTn.childTnLst = new PPTX::Logic::ChildTnLst;

    std::list<std::list<SOldAnimation*> > arrClickPar;
    SplitAnim(arrClickPar);

    for (auto& clickPar : arrClickPar)
    {
        PPTX::Logic::TimeNodeBase child;
        FillClickPar(clickPar, child);
        seq2->cTn.childTnLst->list.push_back(child);
    }

    PPTX::Logic::Cond cond;
    cond.tgtEl = new PPTX::Logic::TgtEl;
    if (m_arrOldAnim[0].anim->m_AnimationAtom.m_fAutomatic)
        cond.delay = L"0";

    seq2->nextCondLst = new PPTX::Logic::CondLst;
    seq2->nextCondLst->node_name = L"nextCondLst";
    cond.evt = L"onNext";
    cond.delay = L"0";
    seq2->nextCondLst->list.push_back(cond);

    seq2->prevCondLst = new PPTX::Logic::CondLst;
    seq2->prevCondLst->node_name = L"prevCondLst";
    cond.evt = L"onPrev";
    cond.delay = L"0";
    seq2->prevCondLst->list.push_back(cond);

    // push back
    PPTX::Logic::TimeNodeBase timeNodeBase;
    timeNodeBase.m_node = seq2;
    par1->cTn.childTnLst = new PPTX::Logic::ChildTnLst;
    par1->cTn.childTnLst->list.push_back(timeNodeBase);
    timeNodeBase.m_node = par1;

    oTiming.tnLst->list.push_back(timeNodeBase);

}

void Animation::SplitAnim(std::list<std::list<SOldAnimation*> >& arrClickPar)
{
    std::sort(m_arrOldAnim.begin(), m_arrOldAnim.end(), [] (
              const SOldAnimation& a1,
              const SOldAnimation& a2)
    {
        return  a1.anim->m_AnimationAtom.m_OrderID <
                a2.anim->m_AnimationAtom.m_OrderID;
    });

    for (auto& oldAnim : m_arrOldAnim)
    {
        if (arrClickPar.empty())
        {
            std::list<SOldAnimation*> clickPar;
            clickPar.push_back(&oldAnim);
            arrClickPar.push_back(clickPar);
        } else if (oldAnim.anim->m_AnimationAtom.m_fAutomatic)
        {
            arrClickPar.back().push_back(&oldAnim);
        } else
        {
            std::list<SOldAnimation*> clickPar;
            clickPar.push_back(&oldAnim);
            arrClickPar.push_back(clickPar);
        }
    }

}

void Animation::FillClickPar(std::list<SOldAnimation*>& clickPar, PPTX::Logic::TimeNodeBase &oTimeNodeBase)
{
    auto animAtom = clickPar.front()->anim->m_AnimationAtom;

    auto par1 = new PPTX::Logic::Par;
    FillCTnParams(par1->cTn, L"clickPar", L"indefinite");

    PPTX::Logic::Cond cond;
    if (animAtom.m_OrderID == 1 &&
            animAtom.m_fAutomatic)
    {
        cond.evt = L"onBegin";
        cond.delay = L"0";
        cond.tn = 2;
        par1->cTn.stCondLst->list.push_back(cond);
    }

    // p:childTnLst
    PPTX::Logic::TimeNodeBase childTimeNode;
    _UINT32 groupDelay = 0;
    FillGroup(clickPar.front(), childTimeNode, groupDelay, L"withGroup");
    par1->cTn.childTnLst = new PPTX::Logic::ChildTnLst;
    par1->cTn.childTnLst->list.push_back(childTimeNode);
    clickPar.pop_front();

    while (!clickPar.empty())
    {
        PPTX::Logic::TimeNodeBase childTimeNode;
        FillGroup(clickPar.front(), childTimeNode, groupDelay, L"afterGroup");
        par1->cTn.childTnLst->list.push_back(childTimeNode);
        clickPar.pop_front();
    }

    oTimeNodeBase.m_node = par1;
}

void Animation::FillGroup  (SOldAnimation* pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase, _UINT32& groupDelay, std::wstring nodeType)
{
    auto anim = pOldAnim->anim->m_AnimationAtom;
    auto par = new PPTX::Logic::Par;

    FillCTnParams(par->cTn, nodeType, std::to_wstring(groupDelay));
    PPTX::Logic::TimeNodeBase childTimeNode;
    if (anim.m_fAutomatic)
    {
        FillAfterEffect(pOldAnim, childTimeNode, groupDelay);
    } else
    {
        FillClickEffect(pOldAnim, childTimeNode, groupDelay);
    }

    par->cTn.childTnLst = new PPTX::Logic::ChildTnLst;
    par->cTn.childTnLst->list.push_back(childTimeNode);

    oTimeNodeBase.m_node = par;
}

void Animation::FillClickEffect(SOldAnimation* pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase, _UINT32& groupDelay)
{
    auto par = new PPTX::Logic::Par;

    FillCTnParams(par->cTn, L"clickEffect", L"0", L"hold", pOldAnim);
    groupDelay += 500; // Effect time // TODO for anim

    oTimeNodeBase.m_node = par;
}

void Animation::FillAfterEffect(SOldAnimation* pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase, _UINT32& groupDelay)
{
    auto anim = pOldAnim->anim->m_AnimationAtom;
    auto par = new PPTX::Logic::Par;

    groupDelay += anim.m_DelayTime;
    auto delay = std::to_wstring(anim.m_DelayTime);
    groupDelay += pOldAnim->getAnimDur(); // Effect time

    FillCTnParams(par->cTn, L"afterEffect", delay, L"hold", pOldAnim);
//    par->cTn.childTnLst = new PPTX::Logic::ChildTnLst; bug #52374, was fixed

    oTimeNodeBase.m_node = par;
}

void Animation::FillCBhvr      (PPTX::Logic::CBhvr &oCBhvr, SOldAnimation* pOldAnim, int delay)
{
    //    auto anim = pOldAnim->anim->m_AnimationAtom;
    FillCBhvr(oCBhvr, 1, pOldAnim->shapeId, L"style.visibility", delay);
}

void Animation::FillCTnParams  (PPTX::Logic::CTn &oCTN, std::wstring nodeType, std::wstring condDelay,
                                std::wstring fill, SOldAnimation* pOldAnim)
{
    oCTN.id = m_cTnId++;
    oCTN.fill = fill;
    oCTN.nodeType = nodeType;

    if (pOldAnim)
    {
        FillCTnAnimation(oCTN, pOldAnim);
    }

    // p:stCondLst
    oCTN.stCondLst = new PPTX::Logic::CondLst;
    oCTN.stCondLst->node_name = L"stCondLst";
    PPTX::Logic::Cond cond;
    cond.delay = condDelay;
    oCTN.stCondLst->list.push_back(cond);
}

void Animation::FillCBhvrForAnim (PPTX::Logic::Anim& oAnim, SOldAnimation* pOldAnim, int dur, std::wstring attrname)
{
    FillCBhvr(oAnim.cBhvr, dur, pOldAnim->shapeId, attrname, -1);

    oAnim.cBhvr.additive = new PPTX::Limit::TLAdditive;
    oAnim.cBhvr.additive->set(L"base");

    oAnim.calcmode       = new PPTX::Limit::TLCalcMode;
    oAnim.calcmode->set(L"lin");
    oAnim.valueType      = new PPTX::Limit::TLValueType;
    oAnim.valueType->set(L"num");
}

void Animation::FillAnim (PPTX::Logic::Anim& oAnim, SOldAnimation* pOldAnim, int dur, std::wstring attrname,
                          SValue val1, SValue val2, std::wstring fmla)
{
    FillCBhvrForAnim(oAnim, pOldAnim, dur, attrname);

    oAnim.tavLst = new PPTX::Logic::TavLst;
    PPTX::Logic::Tav tav;

    tav.tm = L"0";
    if (fmla.size())
    {
        tav.fmla = fmla;
    }
    auto val = new PPTX::Logic::AnimVariant;
    val->node_name = L"val";
    if (val1.type == SValue::str)
        val->strVal = val1.strVal;
    else if (val1.type == SValue::dbl)
        val->fltVal = val1.dblVal;

    tav.val = val;
    oAnim.tavLst->list.push_back(tav);
    tav.fmla.reset();

    tav.tm = L"100000";
    val = new PPTX::Logic::AnimVariant;
    val->node_name = L"val";
    if (val2.type == SValue::str)
        val->strVal = val2.strVal;
    else if (val2.type == SValue::dbl)
        val->fltVal = val2.dblVal;

    tav.val = val;
    oAnim.tavLst->list.push_back(tav);

}

void Animation::FillAnimEffect (PPTX::Logic::AnimEffect& oAnimEffect, SOldAnimation* pOldAnim, std::wstring filter, std::wstring transition)
{
    oAnimEffect.transition = new PPTX::Limit::TLTransition();
    oAnimEffect.transition->set(transition);
    oAnimEffect.filter = filter;

    oAnimEffect.cBhvr.cTn.id = m_cTnId++;
    oAnimEffect.cBhvr.cTn.dur = std::to_wstring(pOldAnim->getAnimDur());
    oAnimEffect.cBhvr.tgtEl.spTgt = new PPTX::Logic::SpTgt;
    oAnimEffect.cBhvr.tgtEl.spTgt->spid = std::to_wstring(pOldAnim->shapeId);
}


void Animation::FillSetAndAnim (SOldAnimation* pOldAnim,
                                PPTX::Logic::ChildTnLst& oParent)
{
    //    bool isAppear = pOldAnim->anim->m_AnimationAtom.m_AnimEffect == 0;
    //    PPTX::Logic::TimeNodeBase childTimeNode;
    //    auto set = new PPTX::Logic::Set;
    //    if (isAppear)
    //        FillCBhvr(set->cBhvr, pOldAnim);
    //    else
    //    {
    //        FillCBhvr(set->cBhvr, pOldAnim, 0);
    //    }
    //    set->to = new PPTX::Logic::AnimVariant;
    //    set->to->node_name = L"to";
    //    set->to->strVal = L"visible";

    //    childTimeNode.m_node = set;
    //    oParent.list.push_back(childTimeNode);

    //    if (!isAppear)
    //    {
    //        auto anim = new PPTX::Logic::Anim;
    //        FillAnim(*anim, pOldAnim, 500, L"ppt_x", L"0-#ppt_w/2", L"#ppt_x");
    //        childTimeNode.m_node = anim;
    //        oParent.list.push_back(childTimeNode);

    //        anim = new PPTX::Logic::Anim;
    //        FillAnim(*anim, pOldAnim, 500, L"ppt_y", L"1+#ppt_h/2", L"#ppt_y");
    //        childTimeNode.m_node = anim;
    //        oParent.list.push_back(childTimeNode);
    //    }
}

void Animation::FillCTnAnimation  (PPTX::Logic::CTn &oCTN, SOldAnimation *pOldAnim)
{
    oCTN.presetClass = L"entr";
    oCTN.grpId = 0;
    oCTN.childTnLst = new PPTX::Logic::ChildTnLst;
    oCTN.presetSubtype = 0;
    int presetSub = -1;

    const UINT effect = pOldAnim->anim->m_AnimationAtom.m_AnimEffect;
    const UINT direct = pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection;

    switch (effect)
    {
    case 0x00:
    {
        oCTN.presetID = 1;
        ConvertAppear(oCTN.childTnLst.get2(), pOldAnim);
        break;
    }
    case 0x01:
    {
        oCTN.presetID = 24;
        ConvertRandomEffect(oCTN.childTnLst.get2(), pOldAnim);
        break;
    }
    case 0x02:
    {
        oCTN.presetID = 3;
        ConvertBlinds(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        break;
    }
    case 0x03:
    {
        oCTN.presetID = 5;
        ConvertCheckerboard(oCTN.childTnLst.get2(), pOldAnim);
        break;
    }
    case 0x05:
    {
        oCTN.presetID = 9;
        ConvertDissolveIn(oCTN.childTnLst.get2(), pOldAnim);
        break;
    }
    case 0x08:
    {
        oCTN.presetID = 14;
        ConvertRandomBars(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        break;
    }
    case 0x09:
    {
        oCTN.presetID = 18;
        ConvertStrips(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        break;
    }
    case 0x0A:
    {
        oCTN.presetID = 22;
        ConvertWipe(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        break;
    }
    case 0x0B:
    {
        oCTN.presetID = 4;
        ConvertShape(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        break;
    }
    case 0x0C:
    {
        if (direct >= 0 && direct <= 7)
        {
            oCTN.presetID = 2;
            ConvertFlyIn(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        } else if (direct >= 8 && direct <= 11)
        {
            oCTN.presetID = 12;
            ConvertPeekIn(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        } else if (direct >= 12 && direct <= 15)
        {
            oCTN.presetID = 7;
            ConvertCrawlIn(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        } else if (direct >= 16 && direct <= 21)
        {
            oCTN.presetID = 23;
            ConvertBasicZoom(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        } else if (direct >= 22 && direct <= 26)
        {
            oCTN.presetID = 17;
            ConvertStretch(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        } else if (direct == 27)
        {
            oCTN.presetID = 19;
            ConvertBasicSwivel(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        } else if (direct == 28)
        {
            oCTN.presetID = 15;
            ConvertSpiralIn(oCTN.childTnLst.get2(), pOldAnim);
        }
        break;
    }
    case 0x0D:
    {
        oCTN.presetID = 16;
        ConvertSplit(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        break;
    }
    case 0x0E:
    {
        oCTN.presetID = 11;
        ConvertFlashOnce(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        break;
    }
    }

    if (presetSub != -1)
    {
        oCTN.presetSubtype = presetSub;
    }

}

// This methods fill ChildTnLst with anim nodes
void Animation::ConvertAppear(PPTX::Logic::ChildTnLst& oParent, SOldAnimation *pOldAnim)
{
    PushSet(oParent, pOldAnim, 499);
}

void Animation::ConvertFlyIn(PPTX::Logic::ChildTnLst& oParent, SOldAnimation *pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 0:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"0-#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"#ppt_y", L"#ppt_y");

        presetSub = 8;
        break;
    }
    case 1:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"#ppt_x", L"#ppt_x",
                 L"ppt_y", L"0-#ppt_h/2", L"#ppt_y");

        presetSub = 1;
        break;
    }
    case 2:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"1+#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"#ppt_y", L"#ppt_y");

        presetSub = 2;
        break;
    }
    case 3:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"#ppt_x", L"#ppt_x",
                 L"ppt_y", L"1+#ppt_h/2", L"#ppt_y");

        presetSub = 4;
        break;
    }
    case 4:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"0-#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"0-#ppt_h/2", L"#ppt_y");

        presetSub = 9;
        break;
    }
    case 5:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"1+#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"0-#ppt_h/2", L"#ppt_y");

        presetSub = 3;
        break;
    }
    case 6:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"0-#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"1+#ppt_h/2", L"#ppt_y");

        presetSub = 12;
        break;
    }
    case 7:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"1+#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"1+#ppt_h/2", L"#ppt_y");

        presetSub = 6;
        break;
    }
    }
}

void Animation::ConvertBlinds(PPTX::Logic::ChildTnLst& oParent, SOldAnimation *pOldAnim, int &presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 0:
    {
        PushAnimEffect(oParent, pOldAnim, L"blinds(vertical)", L"in");

        presetSub = 5;
        break;
    }
    case 1:
    {
        PushAnimEffect(oParent, pOldAnim, L"blinds(horizontal)", L"in");

        presetSub = 10;
        break;
    }
    }
}

void Animation::ConvertShape(PPTX::Logic::ChildTnLst& oParent, SOldAnimation *pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 0:
    {
        PushAnimEffect(oParent, pOldAnim, L"box(out)", L"in");

        presetSub = 32;
        break;
    }
    case 1:
    {
        PushAnimEffect(oParent, pOldAnim, L"box(in)", L"in");

        presetSub = 16;
        break;
    }
    }
}

void Animation::ConvertCheckerboard(PPTX::Logic::ChildTnLst& oParent, SOldAnimation *pOldAnim)
{
    PushSet(oParent, pOldAnim);
    PushAnimEffect(oParent, pOldAnim, L"checkerboard(across)", L"in");
}

void Animation::ConvertCrawlIn(PPTX::Logic::ChildTnLst& oParent, SOldAnimation *pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 15:
    {
        PushAnim(oParent, pOldAnim, 5000,
                 L"ppt_x", L"#ppt_x", L"#ppt_x",
                 L"ppt_y", L"1+#ppt_h/2", L"#ppt_y");

        presetSub = 4;
        break;
    }
    case 12:
    {
        PushAnim(oParent, pOldAnim, 5000,
                 L"ppt_x", L"0-#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"#ppt_y", L"#ppt_y");

        presetSub = 8;
        break;
    }
    case 14:
    {
        PushAnim(oParent, pOldAnim, 5000,
                 L"ppt_x", L"1+#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"#ppt_y", L"#ppt_y");

        presetSub = 2;
        break;
    }
    case 13:
    {
        PushAnim(oParent, pOldAnim, 5000,
                 L"ppt_x", L"#ppt_x", L"#ppt_x",
                 L"ppt_y", L"0-#ppt_h/2", L"#ppt_y");

        presetSub = 1;
        break;
    }
    }
}

void Animation::ConvertDissolveIn(PPTX::Logic::ChildTnLst& oParent, SOldAnimation *pOldAnim)
{
    PushSet(oParent, pOldAnim);
    PushAnimEffect(oParent, pOldAnim, L"dissolve", L"in");
}

void Animation::ConvertFlashOnce(PPTX::Logic::ChildTnLst& oParent, SOldAnimation *pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);
    presetSub = 0;

    // correct cTn params
    auto set = dynamic_cast<PPTX::Logic::Set*>(oParent.list.front().m_node.GetPointer());
    if (!set) return;
    set->cBhvr.cTn.dur = std::to_wstring(pOldAnim->getAnimDur());
    set->cBhvr.cTn.fill.reset();
}

void Animation::ConvertPeekIn(PPTX::Logic::ChildTnLst& oParent, SOldAnimation *pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 8:
    {
        PushAnim(oParent, pOldAnim, L"ppt_x", L"#ppt_x-#ppt_w*1.125000", L"#ppt_x");
        PushAnimEffect(oParent, pOldAnim, L"wipe(right)", L"in");

        presetSub = 8;
        break;
    }
    case 9:
    {
        PushAnim(oParent, pOldAnim, L"ppt_y", L"#ppt_y+#ppt_h*1.125000", L"#ppt_y");
        PushAnimEffect(oParent, pOldAnim, L"wipe(up)", L"in");

        presetSub = 4;
        break;
    }
    case 10:
    {
        PushAnim(oParent, pOldAnim, L"ppt_x", L"#ppt_x+#ppt_w*1.125000", L"#ppt_x");
        PushAnimEffect(oParent, pOldAnim, L"wipe(left)", L"in");

        presetSub = 2;
        break;
    }
    case 11:
    {
        PushAnim(oParent, pOldAnim, L"ppt_y", L"#ppt_y-#ppt_h*1.125000", L"#ppt_y");
        PushAnimEffect(oParent, pOldAnim, L"wipe(down)", L"in");

        presetSub = 1;
        break;
    }
    }
}

void Animation::ConvertRandomBars(PPTX::Logic::ChildTnLst& oParent, SOldAnimation *pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 0:
    {
        PushAnimEffect(oParent, pOldAnim, L"randombar(horizontal)", L"in");

        presetSub = 10;
        break;
    }
    case 1:
    {
        PushAnimEffect(oParent, pOldAnim, L"randombar(vertical)", L"in");

        presetSub = 5;
        break;
    }
    }
}

void Animation::ConvertSpiralIn(PPTX::Logic::ChildTnLst& oParent, SOldAnimation* pOldAnim)
{
    PushSet(oParent, pOldAnim);
//     TODO
    PushAnim(oParent, pOldAnim, 1000,
             L"ppt_w", double(0), L"#ppt_w",
             L"ppt_h", double(0), L"#ppt_h");
    PushAnim(oParent, pOldAnim, 1000,
             L"ppt_x", double(0), double(1),
             L"ppt_y", double(0), double(1),
             L"#ppt_x+(cos(-2*pi*(1-$))*-#ppt_x-sin(-2*pi*(1-$))*(1-#ppt_y))*(1-$)",
             L"#ppt_y+(sin(-2*pi*(1-$))*-#ppt_x+cos(-2*pi*(1-$))*(1-#ppt_y))*(1-$)");

    // remove additive from <p:anim>
    for (auto& child : oParent.list)
    {
        auto anim = dynamic_cast<PPTX::Logic::Anim*>(child.m_node.GetPointer());
        if (!anim) continue;
        anim->cBhvr.additive.reset();
    }

}

void Animation::ConvertSplit(PPTX::Logic::ChildTnLst& oParent, SOldAnimation* pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 0:
    {
        PushAnimEffect(oParent, pOldAnim, L"barn(outHorizontal)", L"in");

        presetSub = 42;
        break;
    }
    case 1:
    {
        PushAnimEffect(oParent, pOldAnim, L"barn(inHorizontal)", L"in");

        presetSub = 26;
        break;
    }
    case 2:
    {
        PushAnimEffect(oParent, pOldAnim, L"barn(outVertical)", L"in");

        presetSub = 37;
        break;
    }
    case 3:
    {
        PushAnimEffect(oParent, pOldAnim, L"barn(inVertical)", L"in");

        presetSub = 21;
        break;
    }
    }
}

void Animation::ConvertStretch(PPTX::Logic::ChildTnLst& oParent, SOldAnimation* pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 22:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", double(0), L"#ppt_w",
                 L"ppt_h", L"#ppt_h", L"#ppt_h");

        presetSub = 10;
        break;
    }
    case 26:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"#ppt_x", L"#ppt_x",
                 L"ppt_y", L"#ppt_y+#ppt_h/2", L"#ppt_y");
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", L"#ppt_w", L"#ppt_w",
                 L"ppt_h", double(0), L"#ppt_h");

        presetSub = 4;
        break;
    }
    case 23:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"##ppt_x-#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"#ppt_y", L"#ppt_y");
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", double(0), L"#ppt_w",
                 L"ppt_h", L"#ppt_h", L"#ppt_h");

        presetSub = 8;
        break;
    }
    case 25:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"#ppt_x-#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"#ppt_y", L"#ppt_y");
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", double(0), L"#ppt_w",
                 L"ppt_h", L"#ppt_h", L"#ppt_h");

        presetSub = 2;
        break;
    }
    case 24:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"#ppt_x+#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"#ppt_y", L"#ppt_y");
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", double(0), L"#ppt_w",
                 L"ppt_h", L"#ppt_h", L"#ppt_h");

        presetSub = 1;
        break;
    }
    }
}

void Animation::ConvertStrips(PPTX::Logic::ChildTnLst& oParent, SOldAnimation* pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 6:
    {
        PushAnimEffect(oParent, pOldAnim, L"strips(downLeft)", L"in");

        presetSub = 12;
        break;
    }
    case 4:
    {
        PushAnimEffect(oParent, pOldAnim, L"strips(upLeft)", L"in");

        presetSub = 9;
        break;
    }
    case 7:
    {
        PushAnimEffect(oParent, pOldAnim, L"strips(downRight)", L"in");

        presetSub = 6;
        break;
    }
    case 5:
    {
        PushAnimEffect(oParent, pOldAnim, L"strips(upRight)", L"in");

        presetSub = 3;
        break;
    }
    }
}

void Animation::ConvertBasicSwivel(PPTX::Logic::ChildTnLst& oParent, SOldAnimation* pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);
    presetSub = 10;

    PushAnim(oParent, pOldAnim, 5000,
             L"ppt_w", double(0), double(1),
             L"ppt_h", L"#ppt_h", L"#ppt_h",
             L"#ppt_w*sin(2.5*pi*$)");
}

void Animation::ConvertWipe(PPTX::Logic::ChildTnLst& oParent, SOldAnimation* pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 3:
    {
        PushAnimEffect(oParent, pOldAnim, L"wipe(up)", L"in");

        presetSub = 1;
        break;
    }
    case 0:
    {
        PushAnimEffect(oParent, pOldAnim, L"wipe(left)", L"in");

        presetSub = 2;
        break;
    }
    case 2:
    {
        PushAnimEffect(oParent, pOldAnim, L"wipe(right)", L"in");

        presetSub = 8;
        break;
    }
    case 1:
    {
        PushAnimEffect(oParent, pOldAnim, L"wipe(down)", L"in");

        presetSub = 4;
        break;
    }
    }
}

void Animation::ConvertBasicZoom(PPTX::Logic::ChildTnLst& oParent, SOldAnimation* pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 16:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", double(0), L"#ppt_w",
                 L"ppt_h", double(0), L"#ppt_h");

        presetSub = 16;
        break;
    }
    case 20:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", double(0), L"#ppt_w",
                 L"ppt_h", double(0), L"#ppt_h");
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", double(0.5), L"#ppt_x",
                 L"ppt_y", double(0.5), L"#ppt_y");

        presetSub = 528;
        break;
    }
    case 17:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", L"2/3*#ppt_w", L"#ppt_w",
                 L"ppt_h", L"2/3*#ppt_h", L"#ppt_h");

        presetSub = 272;
        break;
    }
    case 18:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", L"#ppt_w", L"#ppt_w",
                 L"ppt_h", L"4*#ppt_h", L"#ppt_h");

        presetSub = 32;
        break;
    }
    case 21:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", L"(6*min(max(#ppt_w*#ppt_h,.3),1)-7.4)/-.7*#ppt_w", L"#ppt_w",
                 L"ppt_h", L"(6*min(max(#ppt_w*#ppt_h,.3),1)-7.4)/-.7*#ppt_h", L"#ppt_h");
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", double(0.5), L"#ppt_x",
                 L"ppt_y", L"1+(6*min(max(#ppt_w*#ppt_h,.3),1)-7.4)/-.7*#ppt_h/2", L"#ppt_y");

        presetSub = 36;
        break;
    }
    case 19:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", L"4/3*#ppt_w", L"#ppt_w",
                 L"ppt_h", L"4/3*#ppt_h", L"#ppt_h");

        presetSub = 288;
        break;
    }
    }
}

void Animation::ConvertRandomEffect(PPTX::Logic::ChildTnLst& oParent, SOldAnimation* pOldAnim)
{
    PushSet(oParent, pOldAnim, 499);

    PPTX::Logic::TimeNodeBase childTimeNode;

    auto anim = new PPTX::Logic::Anim;
    anim->to = L"";
    anim->calcmode = new PPTX::Limit::TLCalcMode;
    anim->calcmode->set(L"lin");
    anim->valueType = new PPTX::Limit::TLValueType;
    anim->valueType->set(L"num");

    FillCBhvr(anim->cBhvr, 1, pOldAnim->shapeId, L"", -1);
    childTimeNode.m_node = anim;
    oParent.list.push_back(childTimeNode);
}

void Animation::PushAnim(PPTX::Logic::ChildTnLst& oParent, SOldAnimation *pOldAnim, int dur,
              std::wstring attrname1, SValue val1, SValue val2,
                         std::wstring attrname2, SValue val3, SValue val4, std::wstring fmla1, std::wstring fmla2)
{
    PushAnim(oParent, pOldAnim, attrname1, val1, val2, fmla1);
    PushAnim(oParent, pOldAnim, attrname2, val3, val4, fmla2);
}

void Animation::PushAnim(PPTX::Logic::ChildTnLst& oParent, SOldAnimation *pOldAnim,
         std::wstring attrname1, SValue val1, SValue val2,std::wstring fmla1)
{
    PPTX::Logic::TimeNodeBase childTimeNode;

    auto anim = new PPTX::Logic::Anim;
    FillAnim(*anim, pOldAnim, pOldAnim->getAnimDur(), attrname1, val1, val2, fmla1);
    childTimeNode.m_node = anim;
    oParent.list.push_back(childTimeNode);
}

void Animation::PushAnimEffect(PPTX::Logic::ChildTnLst& oParent, SOldAnimation *pOldAnim, std::wstring filter, std::wstring transition)
{
    PPTX::Logic::TimeNodeBase childTimeNode;

    auto animEffect = new PPTX::Logic::AnimEffect;
    FillAnimEffect(*animEffect, pOldAnim, filter, transition);
    childTimeNode.m_node = animEffect;
    oParent.list.push_back(childTimeNode);
}

void Animation::PushSet(PPTX::Logic::ChildTnLst& oParent, SOldAnimation *pOldAnim, int dur)
{
    PPTX::Logic::TimeNodeBase childTimeNode;
    auto set = new PPTX::Logic::Set;

    FillCBhvr(set->cBhvr, pOldAnim, dur);
    set->to = new PPTX::Logic::AnimVariant;
    set->to->node_name = L"to";
    set->to->strVal = L"visible";

    childTimeNode.m_node = set;
    oParent.list.push_back(childTimeNode);
}

bool Animation::isSpidReal(const UINT spid)
{
    if (m_arrOldAnim.empty())
        return true;

    for (const auto& oldAnim : m_arrOldAnim)
        if (oldAnim.shapeId == spid)
            return true;

    return false;
}


unsigned SOldAnimation::getAnimDur()const
{
    unsigned dur = 500;
    if (!anim)
        return 0;

    const UINT effect = anim->m_AnimationAtom.m_AnimEffect;
    const UINT direct = anim->m_AnimationAtom.m_AnimEffectDirection;

    if (effect == 12 && ((direct >= 12 && direct <= 15) || direct == 27))   // Crawl In, Basic Swivel
        dur = 5000;
    else if (effect == 12 && direct == 28)
        dur = 1000;
    else if (effect == 14)
    {
        switch (direct)
        {
        case 0: dur = 75; break;
        case 1: dur = 500; break;
        case 2: dur = 1000; break;
        }
    }

    return dur;
}
