#pragma once
#include "..\Reader\Records.h"

class CRecordExMediaAtom : public CUnknownRecord
{
public:
	DWORD m_nExObjID;
	
	bool m_bLoop;
	bool m_bRewind;
	bool m_bNarration;

public:
	
	CRecordExMediaAtom()
	{
	}

	~CRecordExMediaAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;

		m_nExObjID = StreamUtils::ReadDWORD(pStream);

		USHORT nFlag = StreamUtils::ReadWORD(pStream);

		m_bLoop = ((nFlag & 0x01) == 0x01);
		m_bRewind = ((nFlag & 0x02) == 0x02);
		m_bNarration = ((nFlag & 0x04) == 0x04);

		StreamUtils::StreamSkip(2, pStream);
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
		
		CDirectory::WriteValueToNode(_T("ID"), m_nExObjID, &oWriter);
		CDirectory::WriteValueToNode(_T("Loop"), m_bLoop, &oWriter);
		CDirectory::WriteValueToNode(_T("Rewind"), m_bRewind, &oWriter);
		CDirectory::WriteValueToNode(_T("Narration"), m_bNarration, &oWriter);

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};