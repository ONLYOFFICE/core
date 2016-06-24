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

class CRecordSlideAtom : public CUnknownRecord
{
public:
	SSlideLayoutAtom	m_oLayout;
	UINT				m_nMasterIDRef;
	UINT				m_nNotesIDRef;
	
	bool m_bMasterObjects;
	bool m_bMasterScheme;
	bool m_bMasterBackground;

public:
	
	CRecordSlideAtom()
	{
		m_oLayout.m_hash = 0;
	}

	~CRecordSlideAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_oLayout.m_nGeom = (INT)StreamUtils::ReadLONG(pStream);
		for (int nIndex = 0; nIndex < 8; nIndex++)
		{
			m_oLayout.m_pPlaceHolderID[nIndex] = StreamUtils::ReadBYTE(pStream);
			m_oLayout.m_hash = (m_oLayout.m_hash << 4) + m_oLayout.m_pPlaceHolderID[nIndex];
		}

		m_oLayout.m_hash += (((_UINT64)m_oLayout.m_nGeom) << 32) ;

		m_nMasterIDRef = (UINT)StreamUtils::ReadDWORD(pStream);
		m_nNotesIDRef = (UINT)StreamUtils::ReadDWORD(pStream);

		USHORT nFlag = StreamUtils::ReadWORD(pStream);
		
		m_bMasterObjects	= ((nFlag & 0x01) == 0x01);
		m_bMasterScheme		= ((nFlag & 0x02) == 0x02);
		m_bMasterBackground = ((nFlag & 0x04) == 0x04);

		StreamUtils::StreamSkip(2, pStream);
	}
};