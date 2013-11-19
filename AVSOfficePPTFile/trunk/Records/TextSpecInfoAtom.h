#pragma once
#include "..\Reader\Records.h"

class CRecordTextSpecInfoAtom : public CUnknownRecord
{
public:
	// нужно еще хранить позицию в стриме, 
	// указатель на начало
	LONG m_lOffsetInStream;
	DWORD m_lCount; 

private:
	CAtlArray<NSPresentationEditor::CTextSIRun> m_arrSIs;

public:
	
	CRecordTextSpecInfoAtom()
	{
		m_lCount = 0;
		m_lOffsetInStream = 0;
	}

	~CRecordTextSpecInfoAtom()
	{
		m_lCount = 0;
	}

	void CRecordTextSpecInfoAtom::ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		//return CUnknownRecord::ReadFromStream(oHeader, pStream);
		
		m_oHeader = oHeader;
		StreamUtils::StreamPosition(m_lOffsetInStream, pStream);

		if (0 == m_lCount)
		{
			StreamUtils::StreamSeek(m_lOffsetInStream + m_oHeader.RecLen, pStream);
			return;
		}

		DWORD lMemCount = 0;
		DWORD lCountItems = 0;
		while (lMemCount < m_lCount + 1)
		{
			m_arrSIs.Add();
			
			NSStreamReader::Read(pStream, m_arrSIs[lCountItems]);
			lMemCount += m_arrSIs[lCountItems].lCount;

			++lCountItems;
		}

		// на всякий случай...
		// здесь когда текст сначала другой (т.е. например - placeholder в мастере) - 
		// то у нас неправильно выставился m_lCount...
		StreamUtils::StreamSeek(m_lOffsetInStream + m_oHeader.RecLen, pStream);
	}

	void CRecordTextSpecInfoAtom::ApplyProperties(CTextAttributesEx* pTextAttributes)
	{
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
		
		for (size_t nIndexSI = 0; nIndexSI < m_arrSIs.GetCount(); ++nIndexSI)
		{
			oWriter.WriteString(m_arrSIs[nIndexSI].ToString());
		}

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};