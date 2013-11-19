#pragma once
#include "..\..\Reader\Records.h"

class CRecordDrawingGroup : public CUnknownRecord
{
	UINT m_nMaxShapeId;           // Maximum shape ID
    UINT m_nIdClustersCount;      // Number of FileIdClusters
    UINT m_nShapesSavedCount;     // Total number of shapes saved
    UINT m_nDrawingsSavedCount;   // Total number of drawings saved

	CAtlArray<SFileIdCluster> m_arrIDs;

public:
	
	CRecordDrawingGroup() : m_arrIDs()
	{
	}

	~CRecordDrawingGroup()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;
		
		m_nMaxShapeId = (UINT)StreamUtils::ReadDWORD(pStream);
		m_nIdClustersCount = (UINT)StreamUtils::ReadDWORD(pStream) - 1; // Office saves the actual value + 1
		m_nShapesSavedCount = (UINT)StreamUtils::ReadDWORD(pStream);
		m_nDrawingsSavedCount = (UINT)StreamUtils::ReadDWORD(pStream);

		m_arrIDs.RemoveAll();
		for (UINT nIndex = 0; nIndex < m_nIdClustersCount; ++nIndex)
		{
			m_arrIDs.Add();
			m_arrIDs[nIndex].ReadFromStream(pStream);
		}
	}
	virtual CString ToString()
	{
		XmlUtils::CXmlWriter oWriter;
		CString strName = GetRecordName((DWORD)m_oHeader.RecType);
		
		oWriter.WriteNodeBegin(strName, TRUE);
		oWriter.WriteAttribute(_T("length"), CDirectory::ToString(m_oHeader.RecLen));
		oWriter.WriteAttribute(_T("type"), CDirectory::ToString(m_oHeader.RecType));
		oWriter.WriteAttribute(_T("instance"), CDirectory::ToString(m_oHeader.RecInstance));

		oWriter.WriteNodeEnd(strName, TRUE, FALSE);
		
		CDirectory::WriteValueToNode(_T("MaxShapeID"), (DWORD)m_nMaxShapeId, &oWriter);
		CDirectory::WriteValueToNode(_T("ClustersCount"), (DWORD)m_nIdClustersCount, &oWriter);
		CDirectory::WriteValueToNode(_T("ShapesSavedCount"), (DWORD)m_nShapesSavedCount, &oWriter);
		CDirectory::WriteValueToNode(_T("DrawingsSavedCount"), (DWORD)m_nDrawingsSavedCount, &oWriter);

		for (size_t nIndex = 0; nIndex < m_arrIDs.GetCount(); ++nIndex)
		{
			oWriter.WriteString(m_arrIDs[nIndex].ToString());
		}
		
		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};