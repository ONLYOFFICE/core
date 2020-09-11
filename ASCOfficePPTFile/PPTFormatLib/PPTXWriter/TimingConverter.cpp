#include "TimingConverter.h"


using namespace PPT_FORMAT;

static inline void ConvertCRecordBuildListContainerToBldLst(
                            CRecordBuildListContainer &oBLC,
                            PPTX::Logic::BldLst &oBL)
{
    return;
}

static inline void ConvertCRecordExtTimeNodeContainerToTnLst(
                            CRecordExtTimeNodeContainer &oETNC,
                            PPTX::Logic::TnLst &oBL)
{
    return;
}

void PPT_FORMAT::ConvertPP10SlideBinaryTagExtensionToTiming(PPT_FORMAT::CRecordPP10SlideBinaryTagExtension &oRec, PPTX::Logic::Timing &oTm)
{
    ConvertCRecordExtTimeNodeContainerToTnLst(*(oRec.m_pExtTimeNodeContainer), *(oTm.tnLst));
    ConvertCRecordBuildListContainerToBldLst(*(oRec.m_pBuildListContainer), *(oTm.bldLst));
    return;
}

