#include "TimingConverter.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Par.h"
#include "../Records/Animations/ExtTimeNodeContainer.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldP.h"


using namespace PPT_FORMAT;

static inline void ConvertCRecordBuildListContainerToBldLst(
                            CRecordBuildListContainer *pBLC,
                            PPTX::Logic::BldLst &oBL)
{
    if (!pBLC)
        return;

    for (unsigned i = 0; i < pBLC->n_arrRgChildRec.size(); i++)
    {
        switch ( pBLC->n_arrRgChildRec[i]->m_oHeader.RecType ) {
        case RT_ParaBuild:
        {
            CRecordParaBuildContainer* pRec =
                    (CRecordParaBuildContainer*)pBLC->n_arrRgChildRec[i];
            PPTX::Logic::BldP* pBldP = new PPTX::Logic::BldP();

            pBldP->spid = pRec->m_oBuildAtom.m_nShapeIdRef;
            pBldP->grpId = pRec->m_oBuildAtom.m_nBuildId;

            pBldP->build = pRec->m_oParaBuildAtom.m_nParaBuild;
            pBldP->advAuto = std::to_wstring(pRec->m_oParaBuildAtom.m_nDelayTime);
            pBldP->animBg = pRec->m_oParaBuildAtom.m_fAnimBackground;
            pBldP->rev = pRec->m_oParaBuildAtom.m_fReverse;
            pBldP->uiExpand = pRec->m_oParaBuildAtom.m_fUserSetAnimBackground;
            pBldP->autoUpdateAnimBg = pRec->m_oParaBuildAtom.m_fAutomatic;

            oBL.list.push_back(*dynamic_cast<PPTX::Logic::BuildNodeBase*>(pBldP));
            break;
        }
        default:
            break;
        }

    }


    return;
}

//static inline void ConvertConvertCRecordExtTimeNodeContainerToPar(
//        CRecordExtTimeNodeContainer *pETNC,
//        PPTX::Logic::Par &oPar)
//{

//}

static inline void ConvertCRecordExtTimeNodeContainerToTnLst(
                            PPT_FORMAT::CRecordExtTimeNodeContainer *pETNC,
                            PPTX::Logic::TnLst &oTnLst)
{
    if (!pETNC)
        return;

    for (auto* child : pETNC->m_arrRgExtTimeNodeChildren)
    {

    }
}

void PPT_FORMAT::ConvertPP10SlideBinaryTagExtensionToTiming(PPT_FORMAT::CRecordPP10SlideBinaryTagExtension &oRec, PPTX::Logic::Timing &oTm)
{
    if (oRec.m_haveExtTime)
    {
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

