#include "DrawingGroup.h"

using namespace PPT;

CRecordDrawingGroup::CRecordDrawingGroup() : m_arrIDs()
{

}

CRecordDrawingGroup::~CRecordDrawingGroup()
{

}

void CRecordDrawingGroup::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nMaxShapeId = (UINT)StreamUtils::ReadDWORD(pStream);
    m_nIdClustersCount = (UINT)StreamUtils::ReadDWORD(pStream) - 1; // Office saves the actual value + 1
    m_nShapesSavedCount = (UINT)StreamUtils::ReadDWORD(pStream);
    m_nDrawingsSavedCount = (UINT)StreamUtils::ReadDWORD(pStream);

    m_arrIDs.clear();
    for (UINT nIndex = 0; nIndex < m_nIdClustersCount; ++nIndex)
    {
        SFileIdCluster elm;
        m_arrIDs.push_back(elm);
        m_arrIDs[nIndex].ReadFromStream(pStream);
    }
}
