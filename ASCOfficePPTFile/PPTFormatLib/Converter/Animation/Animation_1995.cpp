#include "Animation_1995.h"
#include "../../Records/Animations/BuildListContainer.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldLst.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldP.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldOleChart.h"
#include "../../Records/Animations/BuildListContainer.h"


namespace PPT {
namespace Converter {

Animation_1995::Animation_1995(std::vector<Intermediate::SOldAnimation> &vecAIC) :
    arrOldAnim(vecAIC)
{}

void Animation_1995::ConvertToTiming(PPTX::Logic::Timing& timimg, CExMedia* pExMedia, CRelsGenerator* pRels)
{
    if (InitAndCheckVars(timimg, pExMedia, pRels) == false)
        return;
    ConvertBldLst();
    ConvertTnLst();
}

bool Animation_1995::InitAndCheckVars(PPTX::Logic::Timing &timimg, CExMedia *pExMedia, CRelsGenerator *pRels)
{
    this->pExMedia  = pExMedia;
    this->pRels     = pRels;
    this->pTiming   = &timimg;

    return pExMedia && pRels && pTiming && !arrOldAnim.empty();
}

void Animation_1995::ConvertBldLst()
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

void Animation_1995::ConvertTnLst()
{
    pTiming->tnLst = new PPTX::Logic::TnLst();
    SplitRawAnim();
    auto mainSeq = ConvertMainSeqAnimation();
    InsertMainSeqToTnLst(mainSeq);
}

PPTX::Logic::Seq *Animation_1995::ConvertMainSeqAnimation()
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

void Animation_1995::InsertMainSeqToTnLst(PPTX::Logic::Seq *mainSeq)
{
    PPTX::Logic::TimeNodeBase timeNodeBase;
    timeNodeBase.m_node = mainSeq;
    auto par1 = InitRootNode();
    par1->cTn.childTnLst = new PPTX::Logic::ChildTnLst;
    par1->cTn.childTnLst->list.push_back(timeNodeBase);
    timeNodeBase.m_node = par1;

    pTiming->tnLst->list.push_back(timeNodeBase);
}

PPTX::Logic::Par* Animation_1995::InitRootNode()
{
    auto par1 = new PPTX::Logic::Par;
    par1->cTn.id = 1;
    par1->cTn.dur = L"indefinite";
    par1->cTn.restart = L"never";
    par1->cTn.nodeType = L"tmRoot";

    return par1;
}

PPTX::Logic::Seq *Animation_1995::InitMainSeq()
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

void Animation_1995::SplitRawAnim()
{
    splitedAnim.clear();
    SortAnim();

    for (auto& oldAnim : arrOldAnim)
    {
        if (splitedAnim.empty())
        {
            std::list<Intermediate::SOldAnimation&> clickPar;
            clickPar.push_back(oldAnim);
            splitedAnim.push_back(clickPar);
        } else if (oldAnim.anim->m_AnimationAtom.m_fAutomatic)
        {
            splitedAnim.back().push_back(oldAnim);
        } else
        {
            std::list<Intermediate::SOldAnimation&> clickPar;
            clickPar.push_back(oldAnim);
            splitedAnim.push_back(clickPar);
        }
    }
}

void Animation_1995::SortAnim()
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

}
}
