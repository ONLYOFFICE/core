#include "Timing_1995.h"
#include "Animation_1995.h"
#include "../../Records/Animations/BuildListContainer.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldLst.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldP.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldOleChart.h"
#include "../../Records/Animations/BuildListContainer.h"


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
    SplitRawAnim();
    auto mainSeq = ConvertMainSeqAnimation();
    InsertMainSeqToTnLst(mainSeq);
}

PPTX::Logic::Seq *Timing_1995::ConvertMainSeqAnimation()
{
    auto seq2 = InitMainSeq();
    for (auto& clickPar : splitedAnim)
    {
        PPTX::Logic::TimeNodeBase child;
        FillClickPar(clickPar, child);                                      /// TODO next
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

void Timing_1995::SplitRawAnim()
{
    splitedAnim.clear();
    SortAnim();

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
}

void Timing_1995::SortAnim()
{
    std::sort(arrOldAnim.begin(), arrOldAnim.end(),
              []
              (
              const Intermediate::SOldAnimation& a1,
              const Intermediate::SOldAnimation& a2
              )
    {
        return
                a1.anim->m_AnimationAtom.m_OrderID
                <
                a2.anim->m_AnimationAtom.m_OrderID;
    }
    );
}

void Timing_1995::FillClickPar(std::list<Intermediate::SOldAnimation*> &clickPar, PPTX::Logic::TimeNodeBase &oTimeNodeBase)
{
    const auto& animAtom = clickPar.front()->anim->m_AnimationAtom;

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

void Timing_1995::FillCTnParams(PPTX::Logic::CTn &cTn, std::wstring nodeType, std::wstring condDelay, std::wstring fill, Intermediate::SOldAnimation* pOldAnim)
{
    cTn.id = cTnId++;
    cTn.fill = fill;
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

void Timing_1995::FillGroup(Intermediate::SOldAnimation *pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase, _UINT32 &groupDelay, std::wstring nodeType)
{
    const auto& anim = pOldAnim->anim->m_AnimationAtom;
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

void Timing_1995::FillClickEffect(Intermediate::SOldAnimation *pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase, _UINT32 &groupDelay)
{
    auto par = new PPTX::Logic::Par;

    FillCTnParams(par->cTn, L"clickEffect", L"0", L"hold", pOldAnim);
    groupDelay += 500; // Effect time // TODO for anim

    oTimeNodeBase.m_node = par;
}

void Timing_1995::FillAfterEffect(Intermediate::SOldAnimation *pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase, _UINT32 &groupDelay)
{
    const auto& anim = pOldAnim->anim->m_AnimationAtom;
    auto par = new PPTX::Logic::Par;

    groupDelay += anim.m_DelayTime;
    auto delay = std::to_wstring(anim.m_DelayTime);
    groupDelay += pOldAnim->getAnimDur(); // Effect time

    FillCTnParams(par->cTn, L"afterEffect", delay, L"hold", pOldAnim);
    //    par->cTn.childTnLst = new PPTX::Logic::ChildTnLst; bug #52374, was fixed

    oTimeNodeBase.m_node = par;
}

}
}
