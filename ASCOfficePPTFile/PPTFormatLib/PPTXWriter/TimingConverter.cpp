#include "TimingConverter.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Par.h"


using namespace PPT_FORMAT;

static inline void ConvertCRecordBuildListContainerToBldLst(
                            CRecordBuildListContainer *pBLC,
                            PPTX::Logic::BldLst &oBL)
{
    return;
}

//static inline void ConvertConvertCRecordExtTimeNodeContainerToPar(
//        CRecordExtTimeNodeContainer *pETNC,
//        PPTX::Logic::Par &oPar)
//{

//}

static inline void ConvertCRecordExtTimeNodeContainerToTnLst(
                            CRecordExtTimeNodeContainer *pETNC,
                            PPTX::Logic::TnLst &oTnLst)
{
    if (!pETNC)
        return;

    oTnLst = *(new PPTX::Logic::TnLst);
    for (auto* child : pETNC->m_arrRgExtTimeNodeChildren)
    {

    }
}

void PPT_FORMAT::ConvertPP10SlideBinaryTagExtensionToTiming(PPT_FORMAT::CRecordPP10SlideBinaryTagExtension &oRec, PPTX::Logic::Timing &oTm)
{
    ConvertCRecordExtTimeNodeContainerToTnLst(oRec.m_pExtTimeNodeContainer, *(oTm.tnLst));
    ConvertCRecordBuildListContainerToBldLst(oRec.m_pBuildListContainer, *(oTm.bldLst));
    return;
}

