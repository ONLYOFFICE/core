#pragma once
#include "..\Reader\Records.h"

class CRecordStyleTextPropAtom : public CUnknownRecord
{
public:
	// нужно еще хранить позицию в стриме, 
	// указатель на начало
	LONG m_lOffsetInStream;
	
	// просто данные. будут обновляться по мере прибытия
	// Text.Bytes/Chars.Atom
	DWORD m_lCount; 

private:
	CAtlArray<STextPFRun> m_arrPFs;
	CAtlArray<STextCFRun> m_arrCFs;

public:
	
	CRecordStyleTextPropAtom()
	{
		m_lCount = 0;
		m_lOffsetInStream = 0;
	}

	~CRecordStyleTextPropAtom()
	{
		m_lCount = 0;
	}

	void CRecordStyleTextPropAtom::ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;
		StreamUtils::StreamPosition(m_lOffsetInStream, pStream);

		DWORD lMemCount = 0;
		DWORD lCountItems = 0;

		if (0 == m_lCount)
		{
			StreamUtils::StreamSeek(m_lOffsetInStream + m_oHeader.RecLen, pStream);
			return;
		}

		while (lMemCount < m_lCount + 1)
		{
			m_arrPFs.Add();
			m_arrPFs[lCountItems].ReadFromStream(pStream);
			lMemCount += m_arrPFs[lCountItems].lCount;

			++lCountItems;
		}

		lMemCount = 0;
		lCountItems = 0;
		while (lMemCount < m_lCount + 1)
		{
			m_arrCFs.Add();
			m_arrCFs[lCountItems].ReadFromStream(pStream);
			lMemCount += m_arrCFs[lCountItems].lCount;

			++lCountItems;
		}

		// на всякий случай...
		// здесь когда текст сначала другой (т.е. например - placeholder в мастере) - 
		// то у нас неправильно выставился m_lCount...
		StreamUtils::StreamSeek(m_lOffsetInStream + m_oHeader.RecLen, pStream);
	}

	void CRecordStyleTextPropAtom::ApplyProperties(CTextAttributesEx* pTextAttributes)
	{
		if (NULL == pTextAttributes)
			return;

		pTextAttributes->m_arPFs.Copy(m_arrPFs);
		pTextAttributes->m_arCFs.Copy(m_arrCFs);
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
		
		for (size_t nIndexPF = 0; nIndexPF < m_arrPFs.GetCount(); ++nIndexPF)
		{
			oWriter.WriteString(m_arrPFs[nIndexPF].ToString());
		}

		for (size_t nIndexCF = 0; nIndexCF < m_arrCFs.GetCount(); ++nIndexCF)
		{
			oWriter.WriteString(m_arrCFs[nIndexCF].ToString());
		}

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};