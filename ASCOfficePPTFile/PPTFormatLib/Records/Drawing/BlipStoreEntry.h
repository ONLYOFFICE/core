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
#include "../../Reader/Records.h"

class CRecordBlipStoreEntry : public CUnknownRecord
{
public:
	NSOfficeDrawing::BlipType m_btWin32;
	NSOfficeDrawing::BlipType m_btMacOS;
	
	BYTE m_pRgbUid[16];
	WORD m_nTag;
	DWORD m_nSize;
	DWORD m_nCountRef;
	DWORD m_nFoDelay;

	NSOfficeDrawing::BlipUsage m_eUsage;
	BYTE m_nLenName;

	BYTE m_nUnused2;
	BYTE m_nUnused3;
	
	CRecordBlipStoreEntry()
	{
	}

	~CRecordBlipStoreEntry()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_btWin32 = (NSOfficeDrawing::BlipType)StreamUtils::ReadBYTE(pStream);
		m_btMacOS = (NSOfficeDrawing::BlipType)StreamUtils::ReadBYTE(pStream);

		pStream->read(m_pRgbUid, 16);

		m_nTag = StreamUtils::ReadWORD(pStream);

		m_nSize = StreamUtils::ReadDWORD(pStream);
		m_nCountRef = StreamUtils::ReadDWORD(pStream);
		m_nFoDelay = StreamUtils::ReadDWORD(pStream);

		m_eUsage = (NSOfficeDrawing::BlipUsage)StreamUtils::ReadBYTE(pStream);
		
		m_nLenName = StreamUtils::ReadBYTE(pStream);
		
		m_nUnused2 = StreamUtils::ReadBYTE(pStream);
		m_nUnused3 = StreamUtils::ReadBYTE(pStream);
	}
};