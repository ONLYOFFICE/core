#include "ChartBuildAtom.h"

using namespace PPT;

CRecordChartBuildAtom::CRecordChartBuildAtom()
{

}

CRecordChartBuildAtom::~CRecordChartBuildAtom()
{

}

void CRecordChartBuildAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_ChartBuild        = (ChartBuildEnum)StreamUtils::ReadDWORD(pStream);
    m_fAnimBackground   = StreamUtils::ReadBYTE(pStream);

    StreamUtils::StreamSkip(3, pStream);
}
