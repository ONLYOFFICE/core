#pragma once
#include "..\Reader\Records.h"

class CRecordMasterTextPropAtom : public CUnknownRecord
{
public:
	struct SMasterTextPropRun
	{
		DWORD lCount;
		WORD lIndentLevel;

		SMasterTextPropRun()
		{
			lCount = 0;
			lIndentLevel = 0;
		}

		SMasterTextPropRun(const SMasterTextPropRun& oSrc)
		{
			lCount = oSrc.lCount;
			lIndentLevel = oSrc.lIndentLevel;
		}

		SMasterTextPropRun& operator=(const SMasterTextPropRun& oSrc)
		{
			lCount = oSrc.lCount;
			lIndentLevel = oSrc.lIndentLevel;

			return *this;
		}
	};

private:

	CAtlArray<SMasterTextPropRun> m_arrProps;

public:
	
	CRecordMasterTextPropAtom()
	{
	}

	~CRecordMasterTextPropAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;
		m_arrProps.RemoveAll();

		size_t nCount = m_oHeader.RecLen / 6;
		while (nCount != 0)
		{
			--nCount;

			SMasterTextPropRun oRun;
			oRun.lCount = StreamUtils::ReadDWORD(pStream);
			oRun.lIndentLevel = StreamUtils::ReadWORD(pStream);

			m_arrProps.Add(oRun);
		}
	}
	virtual CString ToString()
	{
		CString strName = GetRecordName((DWORD)m_oHeader.RecType);
		
		XmlUtils::CXmlWriter oWriter;
		oWriter.WriteNodeBegin(strName, TRUE);
		
		oWriter.WriteAttribute(_T("type"), CDirectory::ToString(m_oHeader.RecType));
		oWriter.WriteAttribute(_T("length"), CDirectory::ToString(m_oHeader.RecLen));
		oWriter.WriteAttribute(_T("instance"), CDirectory::ToString(m_oHeader.RecInstance));

		oWriter.WriteNodeEnd(strName, TRUE, FALSE);

		for (size_t nIndex = 0; nIndex < m_arrProps.GetCount(); ++nIndex)
		{
			CString str = _T("");
			str.Format(_T("<Run count='%d' indentlevel='%d' />"), m_arrProps[nIndex].lCount, m_arrProps[nIndex].lIndentLevel);

			oWriter.WriteString(str);
		}

		oWriter.WriteNodeEnd(strName);
		return oWriter.GetXmlString();
	}
};