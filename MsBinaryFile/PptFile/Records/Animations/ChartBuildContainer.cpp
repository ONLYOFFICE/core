#include "ChartBuildContainer.h"

using namespace PPT;

CRecordChartBuildContainer::CRecordChartBuildContainer()
{

}

CRecordChartBuildContainer::~CRecordChartBuildContainer()
{

}

void CRecordChartBuildContainer::ReadFromStream(SRecordHeader &header, POLE::Stream *pStream)
{
    CRecordBuildListSubContainer::ReadFromStream(header, pStream);

    SRecordHeader childHeader;
    if (childHeader.ReadFromStream(pStream))
        m_oChartBuildAtom.ReadFromStream ( childHeader, pStream );
}
