#pragma once
#include "..\..\Reader\Records.h"

class CRecordRegGroupItems : public CUnknownRecord
{
public:
	CAtlArray<WORD> m_arItemsNew;
	CAtlArray<WORD> m_arItemsOld;

public:
	
	CRecordRegGroupItems()
	{
	}

	~CRecordRegGroupItems()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;

		m_arItemsNew.RemoveAll();
		m_arItemsOld.RemoveAll();

		LONG lCount = (LONG)(oHeader.RecLen / 4);

		for (LONG i = 0; i < lCount; ++i)
		{
			WORD w1 = StreamUtils::ReadWORD(pStream);
			WORD w2 = StreamUtils::ReadWORD(pStream);

			m_arItemsNew.Add(w1);
			m_arItemsOld.Add(w2);
		}
	}
	virtual CString ToString()
	{
		if (0 == m_arItemsNew.GetCount())
		{
			return CUnknownRecord::ToString();
		}
		
		XmlUtils::CXmlWriter oWriter;
		CString strName = GetRecordName((DWORD)m_oHeader.RecType);
		
		oWriter.WriteNodeBegin(strName, TRUE);
		oWriter.WriteAttribute(_T("length"), CDirectory::ToString(m_oHeader.RecLen));
		oWriter.WriteAttribute(_T("type"), CDirectory::ToString(m_oHeader.RecType));
		oWriter.WriteAttribute(_T("instance"), CDirectory::ToString(m_oHeader.RecInstance));

		oWriter.WriteNodeEnd(strName, TRUE, FALSE);
		
		for (size_t i = 0; i < m_arItemsNew.GetCount(); ++i)
		{
			CString str = _T("");
			str.Format(_T("<Item new='%d' old='%d' />"), m_arItemsNew[i], m_arItemsOld[i]);
			
			oWriter.WriteString(str);
		}

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();

	}
};