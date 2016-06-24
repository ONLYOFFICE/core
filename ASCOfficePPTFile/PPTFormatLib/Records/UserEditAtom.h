/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "../Reader/Records.h"

class CRecordUserEditAtom : public CUnknownRecord
{
public:
	DWORD m_nLastSlideIdRef;

	USHORT m_nVersion;			// must be 0x0000
	BYTE m_nMinorVersion;		// must be 0x00
	BYTE m_nMajorVersion;		// must be 0x03

	DWORD m_nOffsetLastEdit;    // offset to prev UserEditAtom (0 - if no)

	DWORD m_nOffsetPersistDirectory;	// offset to PersistDirectoryAtom in PPTDocStream (for this user)

	DWORD m_nOffsetDocPersistIdRef;		// offset to PersistObjectDirectory in DocContainer must be 0x00000001

	DWORD m_nPersistIdSeed;				// for next

	USHORT m_nLastView;

public:
	
	CRecordUserEditAtom()
	{
	}

	~CRecordUserEditAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		
		m_nLastSlideIdRef = StreamUtils::ReadDWORD(pStream);

		m_nVersion = StreamUtils::ReadWORD(pStream);
		m_nMinorVersion = StreamUtils::ReadBYTE(pStream);
		m_nMajorVersion = StreamUtils::ReadBYTE(pStream);

		m_nOffsetLastEdit = StreamUtils::ReadDWORD(pStream);
		m_nOffsetPersistDirectory = StreamUtils::ReadDWORD(pStream);
		m_nOffsetDocPersistIdRef = StreamUtils::ReadDWORD(pStream);

		m_nPersistIdSeed = StreamUtils::ReadDWORD(pStream);
		m_nLastView = StreamUtils::ReadWORD(pStream);

		StreamUtils::StreamSkip(m_oHeader.RecLen - 22, pStream);
	}

	CRecordUserEditAtom& operator =(const CRecordUserEditAtom& oSrc)
	{
		m_nLastSlideIdRef = oSrc.m_nLastSlideIdRef;

		m_nVersion = oSrc.m_nVersion;
		m_nMinorVersion = oSrc.m_nMinorVersion;
		m_nMajorVersion = oSrc.m_nMajorVersion;

		m_nOffsetLastEdit = oSrc.m_nOffsetLastEdit;

		m_nOffsetPersistDirectory = oSrc.m_nOffsetPersistDirectory;

		m_nOffsetDocPersistIdRef = oSrc.m_nOffsetDocPersistIdRef;
		m_nPersistIdSeed = oSrc.m_nPersistIdSeed;

		m_nLastView = oSrc.m_nLastView;
		return (*this);
	}
};