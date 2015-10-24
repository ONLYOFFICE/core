#pragma once
#include "../Records/RecordsIncluder.h"

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

	std::string		m_strANSIUserName;
	std::wstring	m_strUNICODEUserName;

	DWORD m_nRelVersion;  // 0x00000008 or 0x00000009

public:
	
	CRecordCurrentUserAtom() : m_nToken(0)
	{
		m_nRelVersion = m_nDocFileVersion = m_nMinorVersion = m_nMajorVersion = m_nLenUserName = m_nOffsetToCurEdit = 0;
		m_nSize = m_nToken =0;

	}

	~CRecordCurrentUserAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream * pStream)
	{
		m_oHeader = oHeader;

		m_nSize				= StreamUtils::ReadDWORD(pStream);
		
		long sz = pStream->size() - pStream->tell();

		if (m_nSize >  sz )
		{
			m_nSize =  sz ;
		}

		if (m_nSize < 16) return;

		m_nToken			= StreamUtils::ReadDWORD(pStream);

		m_nOffsetToCurEdit	= StreamUtils::ReadDWORD(pStream);

		m_nLenUserName		= StreamUtils::ReadWORD(pStream);

		m_nDocFileVersion	= StreamUtils::ReadWORD(pStream);

		m_nMajorVersion		= StreamUtils::ReadBYTE(pStream);
		m_nMinorVersion		= StreamUtils::ReadBYTE(pStream);

		StreamUtils::StreamSkip(2, pStream);

		m_strANSIUserName = StreamUtils::ReadStringA(pStream, m_nLenUserName);

		m_nRelVersion = StreamUtils::ReadDWORD(pStream);

        m_strUNICODEUserName = StreamUtils::ReadStringW(pStream, m_nLenUserName );

	}
	virtual CString ToString()
	{
		return L"";
	}

    bool IsSupported()
	{
		return (NO_ENCRYPT == m_nToken);
	}
};
