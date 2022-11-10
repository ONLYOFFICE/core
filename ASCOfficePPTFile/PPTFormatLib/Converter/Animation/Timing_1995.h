#pragma once

#include "../../PPTXWriter/ImageManager.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Timing.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Par.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Seq.h"
#include "intermediate_anim.h"


namespace PPT {
namespace Converter {
class Animation_1995;

using LstAnim = std::list<Intermediate::SOldAnimation*>;
using LstLstAnim = std::list<LstAnim>;

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

    void FillClickGroup(LstAnim &clickPar, PPTX::Logic::TimeNodeBase &oTimeNodeBase);
    static LstLstAnim SplitClickGroupByParrallelShow(LstAnim &clickGroup);
    void FillCTnParams(PPTX::Logic::CTn &cTn, std::wstring nodeType, std::wstring condDelay=L"", std::wstring fill=L"hold", Intermediate::SOldAnimation* pOldAnim = nullptr);
    void ConvertParallelGroupAnimation(LstAnim& parGroup, PPTX::Logic::TimeNodeBase &oTimeNodeBase, _UINT32& groupDelay);
    static _INT32 GetParallelGroupDuration(LstAnim &parGroup);
    void FillClickEffect(Intermediate::SOldAnimation* pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase);
    void FillAfterEffect(Intermediate::SOldAnimation* pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase);
    void FillWithEffect (Intermediate::SOldAnimation* pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase);

private:
    std::vector<Intermediate::SOldAnimation> arrOldAnim;
    LstLstAnim splitedAnim;
    Animation_1995* animConverter;

    PPTX::Logic::Timing* pTiming = nullptr;
    CExMedia* pExMedia;
    CRelsGenerator* pRels;
    _INT32 cTnId = 3;
};
}
}
