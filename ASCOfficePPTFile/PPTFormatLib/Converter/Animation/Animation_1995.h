#pragma once

#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Timing.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldP.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Anim.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AnimClr.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AnimEffect.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AnimMotion.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AnimRot.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AnimScale.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AnimVariant.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Audio.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/CBhvr.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/ChildTnLst.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Cmd.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/CondLst.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Par.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Seq.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Set.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Video.h"
#include "../../Records/Animations/AnimationInfoContainer.h"
#include "intermediate_anim.h"


namespace PPT {
namespace Converter {
class Animation_1995
{
public:
    Animation_1995(std::vector<Intermediate::SOldAnimation>& vecAIC);
    void ConvertToTiming(PPTX::Logic::Timing& timimg, CExMedia* pExMedia, CRelsGenerator* pRels);

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
    void FillClickPar(std::list<Intermediate::SOldAnimation&>& clickPar, PPTX::Logic::TimeNodeBase &oTimeNodeBase);

private:
    std::vector<Intermediate::SOldAnimation>& arrOldAnim;
    std::list<std::list<Intermediate::SOldAnimation&> > splitedAnim;

    PPTX::Logic::Timing* pTiming = nullptr;
    CExMedia* pExMedia;
    CRelsGenerator* pRels;
    _INT32 cTnId = 3;
};
}
}
