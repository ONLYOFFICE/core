#pragma once
#include "../../Reader/Records.h"

class CRecordDrawingGroup : public CUnknownRecord
{
	UINT m_nMaxShapeId;           // Maximum shape ID
    UINT m_nIdClustersCount;      // Number of FileIdClusters
    UINT m_nShapesSavedCount;     // Total number of shapes saved
    UINT m_nDrawingsSavedCount;   // Total number of drawings saved

	std::vector<SFileIdCluster> m_arrIDs;

public:
	
	CRecordDrawingGroup() : m_arrIDs()
	{
	}

	~CRecordDrawingGroup()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
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

};