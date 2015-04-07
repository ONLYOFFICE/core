#pragma once
#include "../Reader/Records.h"

class CRecordStyleTextPropAtom : public CUnknownRecord
{
public:
	// нужно еще хранить позицию в стриме, 
	// указатель на начало
	LONG m_lOffsetInStream;
	
	// просто данные. будут обновляться по мере прибытия
	// Text.Bytes/Chars.Atom
	DWORD m_lCount; 

public:
	std::vector<CTextPFRun_ppt> m_arrPFs;
	std::vector<CTextCFRun_ppt> m_arrCFs;

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

    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
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
			CTextPFRun_ppt elm;
			m_arrPFs.push_back(elm);
			m_arrPFs[lCountItems].LoadFromStream(pStream);
			lMemCount += m_arrPFs[lCountItems].m_lCount;

			++lCountItems;
		}

		lMemCount = 0;
		lCountItems = 0;
		while (lMemCount < m_lCount + 1)
		{
			CTextCFRun_ppt elm;
			m_arrCFs.push_back(elm);

			m_arrCFs[lCountItems].LoadFromStream(pStream);
			lMemCount += m_arrCFs[lCountItems].m_lCount;

			++lCountItems;
		}

		// на всякий случай...
		// здесь когда текст сначала другой (т.е. например - placeholder в мастере) - 
		// то у нас неправильно выставился m_lCount...
		StreamUtils::StreamSeek(m_lOffsetInStream + m_oHeader.RecLen, pStream);
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
		
		for (size_t nIndexPF = 0; nIndexPF < m_arrPFs.size(); ++nIndexPF)
		{
			oWriter.WriteString(m_arrPFs[nIndexPF].m_oRun.ToString(m_arrPFs[nIndexPF].m_lCount));
		}

		for (size_t nIndexCF = 0; nIndexCF < m_arrCFs.size(); ++nIndexCF)
		{
			oWriter.WriteString(m_arrCFs[nIndexCF].m_oRun.ToString(m_arrCFs[nIndexCF].m_lCount));
		}

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};
