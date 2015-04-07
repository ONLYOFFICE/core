#pragma once
#include "../../Reader/Records.h"

class CRecordDrawingRecord : public CUnknownRecord
{
	UINT m_nCountShapes;
    /// The last MSOSPID given to an SP in this DG
    UINT m_nPidCur;

public:
	
	CRecordDrawingRecord()
	{
	}

	~CRecordDrawingRecord()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		m_nCountShapes = (UINT)StreamUtils::ReadDWORD(pStream);
		m_nPidCur = (UINT)StreamUtils::ReadDWORD(pStream);
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
		
		CDirectory::WriteValueToNode(_T("DrawingID"), (DWORD)m_oHeader.RecInstance, &oWriter);
		CDirectory::WriteValueToNode(_T("CountShapes"), (DWORD)m_nCountShapes, &oWriter);
		CDirectory::WriteValueToNode(_T("PidCur"), (DWORD)m_nPidCur, &oWriter);

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};