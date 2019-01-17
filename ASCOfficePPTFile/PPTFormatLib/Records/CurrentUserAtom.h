/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once
#include "../Records/RecordsIncluder.h"

#define			NO_ENCRYPT		0xE391C05F
#define			ENCRYPT			0xF3D1C4DF

class CRecordCurrentUserAtom : public CUnknownRecord
{
public:
	_UINT32 m_nSize;		// must be 0x00000014
	_UINT32 m_nToken;		// encrypt or not

	_UINT32 m_nOffsetToCurEdit;   // offset to UserEditAtom in PPTDocStream

	USHORT m_nLenUserName;  

	USHORT m_nDocFileVersion;	// must be 0x03F4
	BYTE m_nMajorVersion;		// must be 0x03
	BYTE m_nMinorVersion;		// must be 0x00

	std::string		m_strANSIUserName;
	std::wstring	m_strUNICODEUserName;

	_UINT32 m_nRelVersion;  // 0x00000008 or 0x00000009


	CRecordCurrentUserAtom() : m_nToken(0)
	{
		m_nSize = m_nRelVersion = m_nToken = m_nOffsetToCurEdit = 0;
		m_nLenUserName = m_nDocFileVersion = 0;
		m_nMinorVersion = m_nMajorVersion = 0;

	}

	~CRecordCurrentUserAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream * pStream)
	{
		m_oHeader = oHeader;

		m_nSize				= StreamUtils::ReadDWORD(pStream);
		
		long sz = (long)(pStream->size() - pStream->tell());

		if ((long)m_nSize >  sz )
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

    bool IsSupported()
	{
		return (NO_ENCRYPT == m_nToken);
	}
};
