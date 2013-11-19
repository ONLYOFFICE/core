#pragma once
#include "..\Records\RecordsIncluder.h"

#define			NO_ENCRYPT		0xE391C05F
#define			ENCRYPT			0xF3D1C4DF

class CRecordCurrentUserAtom : public CUnknownRecord
{
public:
	DWORD m_nSize;		// must be 0x00000014
	DWORD m_nToken;		// encrypt or not

	DWORD m_nOffsetToCurEdit;   // offset to UserEditAtom in PPTDocStream

	USHORT m_nLenUserName;  

	USHORT m_nDocFileVersion;	// must be 0x03F4
	BYTE m_nMajorVersion;		// must be 0x03
	BYTE m_nMinorVersion;		// must be 0x00

	CString m_strANSIUserName;
	CStringW m_strUNICODEUserName;

	DWORD m_nRelVersion;  // 0x00000008 or 0x00000009

public:
	
	CRecordCurrentUserAtom() : m_nToken(0)
	{
	}

	~CRecordCurrentUserAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;
		
		m_nSize = StreamUtils::ReadDWORD(pStream);
		m_nToken = StreamUtils::ReadDWORD(pStream);

		m_nOffsetToCurEdit = StreamUtils::ReadDWORD(pStream);

		m_nLenUserName = StreamUtils::ReadWORD(pStream);

		m_nDocFileVersion = StreamUtils::ReadWORD(pStream);
		m_nMajorVersion = StreamUtils::ReadBYTE(pStream);
		m_nMinorVersion = StreamUtils::ReadBYTE(pStream);

		StreamUtils::StreamSkip(2, pStream);

		m_strANSIUserName = StreamUtils::ReadCString(pStream, m_nLenUserName);

		m_nRelVersion = StreamUtils::ReadDWORD(pStream);

		m_strUNICODEUserName = StreamUtils::ReadCStringW(pStream, m_nLenUserName);

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
		
		CDirectory::WriteValueToNode(_T("Size"), m_nSize, &oWriter);
		CDirectory::WriteValueToNode(_T("Token"), m_nToken, &oWriter);
		
		CDirectory::WriteValueToNode(_T("OffsetToCurEdit"), m_nOffsetToCurEdit, &oWriter);

		CDirectory::WriteValueToNode(_T("Name"), m_strANSIUserName, &oWriter);
		CDirectory::WriteValueToNode(_T("RelVersion"), m_nRelVersion, &oWriter);

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}

	BOOL IsSupported()
	{
		return (NO_ENCRYPT == m_nToken);
	}
};