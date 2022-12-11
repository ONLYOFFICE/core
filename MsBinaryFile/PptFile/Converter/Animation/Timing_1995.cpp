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
#include "Timing_1995.h"
#include "Animation_1995.h"
#include "../../Records/Animations/BuildListContainer.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/BldLst.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/BldP.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/BldOleChart.h"
#include "../../Records/Animations/BuildListContainer.h"
#include <algorithm>


namespace PPT {
namespace Converter {

Timing_1995::Timing_1995(const std::vector<Intermediate::SOldAnimation> &vecAIC) :
    arrOldAnim(vecAIC),
    animConverter(new Animation_1995(cTnId))
{}

void Timing_1995::Convert(PPTX::Logic::Timing& timimg, CExMedia* pExMedia, CRelsGenerator* pRels)
{
    if (InitAndCheckVars(timimg, pExMedia, pRels) == false)
        return;
    ConvertBldLst();
    ConvertTnLst();
}

Timing_1995::~Timing_1995()
{
    delete animConverter;
}

bool Timing_1995::InitAndCheckVars(PPTX::Logic::Timing &timimg, CExMedia *pExMedia, CRelsGenerator *pRels)
{
    this->pExMedia  = pExMedia;
    this->pRels     = pRels;
    this->pTiming   = &timimg;

    return pExMedia && pRels && pTiming && !arrOldAnim.empty();
}

void Timing_1995::ConvertBldLst()
{
    pTiming->bldLst = new PPTX::Logic::BldLst();
    for (auto oldAnim : arrOldAnim)
    {
        PPTX::Logic::BuildNodeBase oBuildNodeBase;
        PPTX::Logic::BldP *pBldP = new PPTX::Logic::BldP();
        pBldP->spid = std::to_wstring(oldAnim.shapeId);
        pBldP->grpId = false;
        pBldP->animBg = (bool)(oldAnim.anim->m_AnimationAtom.m_fAnimateBg != 0);

        oBuildNodeBase.m_node = pBldP;
        pTiming->bldLst->list.push_back(oBuildNodeBase);
    }
}

void Timing_1995::ConvertTnLst()
{
    pTiming->tnLst = new PPTX::Logic::TnLst();
    auto clickGroupsAnim = SplitRawAnim();
    auto mainSeq = ConvertMainSeqAnimation(clickGroupsAnim);
    InsertMainSeqToTnLst(mainSeq);
}

LstLstAnim Timing_1995::SplitRawAnim()
{
    SortAnim();

    LstLstAnim splitedAnim;

    for (auto& oldAnim : arrOldAnim)
    {
        if (splitedAnim.empty())
        {
            std::list<Intermediate::SOldAnimation*> clickPar;
            clickPar.push_back(&oldAnim);
            splitedAnim.push_back(clickPar);
        } else if (oldAnim.anim->m_AnimationAtom.m_fAutomatic)
        {
            splitedAnim.back().push_back(&oldAnim);
        } else
        {
            std::list<Intermediate::SOldAnimation*> clickPar;
            clickPar.push_back(&oldAnim);
            splitedAnim.push_back(clickPar);
        }
    }

    return splitedAnim;
}

PPTX::Logic::Seq *Timing_1995::ConvertMainSeqAnimation(LstLstAnim &splitedAnim)
{
    auto seq2 = InitMainSeq();
    for (auto& clickGroup : splitedAnim)
    {
        PPTX::Logic::TimeNodeBase child;
        FillClickGroup(clickGroup, child);
        seq2->cTn.childTnLst->list.push_back(child);
    }
    return seq2;
}

void Timing_1995::InsertMainSeqToTnLst(PPTX::Logic::Seq *mainSeq)
{
    PPTX::Logic::TimeNodeBase timeNodeBase;
    timeNodeBase.m_node = mainSeq;
    auto par1 = InitRootNode();
    par1->cTn.childTnLst = new PPTX::Logic::ChildTnLst;
    par1->cTn.childTnLst->list.push_back(timeNodeBase);
    timeNodeBase.m_node = par1;

    pTiming->tnLst->list.push_back(timeNodeBase);
}

PPTX::Logic::Par* Timing_1995::InitRootNode()
{
    auto par1 = new PPTX::Logic::Par;
    par1->cTn.id = 1;
    par1->cTn.dur = L"indefinite";
    par1->cTn.restart = L"never";
    par1->cTn.nodeType = L"tmRoot";

    return par1;
}

PPTX::Logic::Seq *Timing_1995::InitMainSeq()
{
    auto seq2 = new PPTX::Logic::Seq;
    seq2->cTn.id = 2;
    seq2->cTn.dur = L"indefinite";
    seq2->cTn.nodeType = L"mainSeq";
    seq2->concurrent = L"1";
    seq2->nextAc = L"seek";
    seq2->cTn.childTnLst = new PPTX::Logic::ChildTnLst;

    PPTX::Logic::Cond cond;
    cond.tgtEl = new PPTX::Logic::TgtEl;
    if (arrOldAnim.front().anim->m_AnimationAtom.m_fAutomatic)
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

    return seq2;
}

void Timing_1995::SortAnim()
{
    std::sort(arrOldAnim.begin(), arrOldAnim.end(),
              [] (const Intermediate::SOldAnimation& a1, const Intermediate::SOldAnimation& a2)
    {
        return  a1.anim->m_AnimationAtom.m_OrderID
                <
                a2.anim->m_AnimationAtom.m_OrderID;
    }
    );
}

void Timing_1995::FillClickGroup(LstAnim &clickGroup, PPTX::Logic::TimeNodeBase &oTimeNodeBase)
{
    const auto& animAtom = clickGroup.front()->anim->m_AnimationAtom;
    auto parGroups = SplitClickGroupByParrallelShow(clickGroup);

    auto par3 = new PPTX::Logic::Par;
    FillCTnParams(par3->cTn, L"", L"indefinite", L"hold");

    FillFirstAutomaticGroup(par3->cTn, animAtom);

    par3->cTn.childTnLst = new PPTX::Logic::ChildTnLst;
    _UINT32 groupDelay = 0;

    for (auto& group : parGroups)
    {
        PPTX::Logic::TimeNodeBase childTimeNode;
        ConvertParallelGroupAnimation(group, childTimeNode, groupDelay);
        par3->cTn.childTnLst->list.push_back(childTimeNode);
    }

    oTimeNodeBase.m_node = par3;
}

LstLstAnim Timing_1995::SplitClickGroupByParrallelShow(LstAnim &clickGroup)
{
    LstLstAnim lstParallelGroups;
    bool needNewGroup = true;
    for (auto* anim : clickGroup)
    {
        auto& animAtom = anim->anim->m_AnimationAtom;
        if (needNewGroup)
            lstParallelGroups.push_back({anim});
        else
            lstParallelGroups.back().push_back(anim);

        needNewGroup = animAtom.m_fSynchronous;
    }
    return lstParallelGroups;
}

void Timing_1995::FillFirstAutomaticGroup(PPTX::Logic::CTn &cTn, const CRecordAnimationInfoAtom &animAtom) const
{
    PPTX::Logic::Cond cond;
    if (animAtom.m_OrderID == arrOldAnim[0].anim->m_AnimationAtom.m_OrderID &&
            animAtom.m_fAutomatic)
    {
        cond.evt = L"onBegin";
        cond.delay = L"0";
        cond.tn = 2;
        cTn.stCondLst->list.push_back(cond);
    }
}

void Timing_1995::FillCTnParams(PPTX::Logic::CTn &cTn, std::wstring nodeType, std::wstring condDelay, std::wstring fill, Intermediate::SOldAnimation* pOldAnim)
{
    cTn.id = cTnId++;
    if (!fill.empty())
        cTn.fill = fill;
    if (!nodeType.empty())
        cTn.nodeType = nodeType;

    if (pOldAnim)
        animConverter->FillCTnAnimation(cTn, pOldAnim);

    // p:stCondLst
    cTn.stCondLst = new PPTX::Logic::CondLst;
    cTn.stCondLst->node_name = L"stCondLst";
    PPTX::Logic::Cond cond;
    cond.delay = condDelay;
    cTn.stCondLst->list.push_back(cond);
}

void Timing_1995::ConvertParallelGroupAnimation(LstAnim &parGroup, PPTX::Logic::TimeNodeBase &oTimeNodeBase, _UINT32 &groupDelay)
{
    // par4 - 4 level - all 1995 animation level
    auto par4 = new PPTX::Logic::Par;
    par4->cTn.childTnLst = new PPTX::Logic::ChildTnLst;

    FillCTnParams(par4->cTn, L"", std::to_wstring(groupDelay));
    groupDelay += GetParallelGroupDuration(parGroup);

    ConvertHeadEfectInParallelShow(parGroup, par4->cTn);
    ConvertWithEfectsInParallelShow(parGroup, par4->cTn);

    oTimeNodeBase.m_node = par4;
}

_INT32 Timing_1995::GetParallelGroupDuration(LstAnim &parGroup)
{
    auto iterSlowestAnim = std::max_element(parGroup.begin(), parGroup.end(),
                                            [] (Intermediate::SOldAnimation const* a1, Intermediate::SOldAnimation const* a2)
    {
            return a1->getAnimDur() < a2->getAnimDur();
});

    return (*iterSlowestAnim)->getAnimDur();;
}

void Timing_1995::ConvertHeadEfectInParallelShow(const LstAnim& group, PPTX::Logic::CTn &cTn)
{
    PPTX::Logic::TimeNodeBase childTimeNode;
    auto* pOldAnimation = group.front();

    if (pOldAnimation->anim->m_AnimationAtom.m_fAutomatic)
        FillAfterEffect(pOldAnimation, childTimeNode);
    else
        FillClickEffect(pOldAnimation, childTimeNode);
    cTn.childTnLst->list.push_back(childTimeNode);
}

void Timing_1995::ConvertWithEfectsInParallelShow(const LstAnim& group, PPTX::Logic::CTn &cTn)
{
    for (auto itAnim = ++group.begin(); itAnim != group.end(); itAnim++)
    {
        PPTX::Logic::TimeNodeBase childTimeNode;
        FillWithEffect(*itAnim, childTimeNode);
        cTn.childTnLst->list.push_back(childTimeNode);
    }
}

void Timing_1995::FillClickEffect(Intermediate::SOldAnimation *pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase)
{
    FillEffectType(pOldAnim, oTimeNodeBase, L"clickEffect");
}

void Timing_1995::FillAfterEffect(Intermediate::SOldAnimation *pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase)
{
    FillEffectType(pOldAnim, oTimeNodeBase, L"afterEffect");
}

void Timing_1995::FillWithEffect(Intermediate::SOldAnimation *pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase)
{
    FillEffectType(pOldAnim, oTimeNodeBase, L"withEffect");
}

void Timing_1995::FillEffectType(Intermediate::SOldAnimation *pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase, std::wstring nodeType)
{
    auto par = new PPTX::Logic::Par;
    FillCTnParams(par->cTn, nodeType, L"0", L"hold", pOldAnim);
    oTimeNodeBase.m_node = par;
}

}
}
