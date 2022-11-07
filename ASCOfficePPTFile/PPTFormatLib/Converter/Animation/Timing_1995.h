#pragma once

#include "../../PPTXWriter/ImageManager.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Timing.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Par.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Seq.h"
#include "intermediate_anim.h"


namespace PPT {
namespace Converter {
class Animation_1995;

class Timing_1995
{
public:
    Timing_1995(const std::vector<Intermediate::SOldAnimation>& vecAIC);
    void Convert(PPTX::Logic::Timing& timimg, CExMedia* pExMedia, CRelsGenerator* pRels);
    ~Timing_1995();

private:
    bool InitAndCheckVars(PPTX::Logic::Timing& timimg, CExMedia* pExMedia, CRelsGenerator* pRels);
    void ConvertBldLst();
    void ConvertTnLst();
    PPTX::Logic::Seq *ConvertMainSeqAnimation();
    void InsertMainSeqToTnLst(PPTX::Logic::Seq *mainSeq);
    PPTX::Logic::Par *InitRootNode();
    PPTX::Logic::Seq *InitMainSeq();
    void SplitRawAnim();
    void SortAnim();

    void FillClickPar(std::list<Intermediate::SOldAnimation *> &clickPar, PPTX::Logic::TimeNodeBase &oTimeNodeBase);
    void FillCTnParams(PPTX::Logic::CTn &cTn, std::wstring nodeType, std::wstring condDelay=L"", std::wstring fill=L"hold", Intermediate::SOldAnimation* pOldAnim = nullptr);
    void FillGroup(Intermediate::SOldAnimation* pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase, _UINT32& groupDelay, std::wstring nodeType);
    void FillClickEffect(Intermediate::SOldAnimation* pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase, _UINT32& groupDelay);
    void FillAfterEffect(Intermediate::SOldAnimation* pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase, _UINT32& groupDelay);

private:
    std::vector<Intermediate::SOldAnimation> arrOldAnim;
    std::list<std::list<Intermediate::SOldAnimation*> > splitedAnim;
    Animation_1995* animConverter;

    PPTX::Logic::Timing* pTiming = nullptr;
    CExMedia* pExMedia;
    CRelsGenerator* pRels;
    _INT32 cTnId = 3;
};
}
}
