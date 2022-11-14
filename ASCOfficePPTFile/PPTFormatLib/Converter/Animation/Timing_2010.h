#pragma once

#include "../../PPTXWriter/ImageManager.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Timing.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Par.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Seq.h"

#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldP.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldDgm.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldOleChart.h"

#include "../../Records/Animations/ParaBuildContainer.h"
#include "../../Records/Animations/ChartBuildContainer.h"
#include "../../Records/Animations/DiagramBuildContainer.h"

#include "intermediate_anim.h"
//#include <set>


namespace PPT {
namespace Converter {
class Animation_2010;

class Timing_2010
{
public:
    Timing_2010(CRecordPP10SlideBinaryTagExtension* pAnim_2010, const std::unordered_set<int>& shapesID);
    void Convert(PPTX::Logic::Timing& timimg, CExMedia* pExMedia, CRelsGenerator* pRels);

private:
    void ConvertBldLst(PPTX::Logic::BldLst &bldLst, CRecordBuildListContainer *pBLC);
    void FillBuildNodeBase(CRecordBuildListSubContainer* pSub, PPTX::Logic::BuildNodeBase oBuildNodeBase);
    void FillBldP(CRecordParaBuildContainer *pPBC, PPTX::Logic::BldP &oBP);
    void FillBldOleChart(CRecordChartBuildContainer* pCBC, PPTX::Logic::BldOleChart &oBP);
    void FillBldDgm(CRecordDiagramBuildContainer *pDBC, PPTX::Logic::BldDgm &oBP);

private:
    CRecordPP10SlideBinaryTagExtension* pTagExtAnim = nullptr;
    const std::unordered_set<int> slideShapes;
};

}
}
