#include "TimingConverter.h"

#include "TimingConverter/BldLst.h"
#include "TimingConverter/TnLst.h"


using namespace PPT_FORMAT;


void PPT_FORMAT::ConvertPP10SlideBinaryTagExtensionToTiming(PPT_FORMAT::CRecordPP10SlideBinaryTagExtension &oRec, PPTX::Logic::Timing &oTm)
{
    if (oRec.m_haveExtTime)
    {
//        gCTnID = 1;
        oTm.tnLst = new PPTX::Logic::TnLst();
        ConvertCRecordExtTimeNodeContainerToTnLst(oRec.m_pExtTimeNodeContainer, *(oTm.tnLst));
    }

    if (oRec.m_haveBuildList)
    {
        oTm.bldLst = new PPTX::Logic::BldLst();
        ConvertCRecordBuildListContainerToBldLst(oRec.m_pBuildListContainer, *(oTm.bldLst));
    }

    return;
}

