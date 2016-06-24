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
#include "ExMediaAtom.h"

class CRecordExCDAudioContainer : public CRecordsContainer
{
public:
	CRecordExMediaAtom	m_oMedia;
	
	double				m_dStartTime;
	double				m_dEndTime;

public:
	
	CRecordExCDAudioContainer()
	{
		m_dStartTime	= 0;
		m_dEndTime		= 0;
	}

	~CRecordExCDAudioContainer()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		SRecordHeader oExHeader;
		oExHeader.ReadFromStream(pStream);

		m_oMedia.ReadFromStream(oExHeader, pStream);

		StreamUtils::StreamSkip(8, pStream);

		BYTE nTrack1		= StreamUtils::ReadBYTE(pStream);
		BYTE nMinute1		= StreamUtils::ReadBYTE(pStream);
		BYTE nSecond1		= StreamUtils::ReadBYTE(pStream);
		BYTE nFrame1		= StreamUtils::ReadBYTE(pStream);

		BYTE nTrack2		= StreamUtils::ReadBYTE(pStream);
		BYTE nMinute2		= StreamUtils::ReadBYTE(pStream);
		BYTE nSecond2		= StreamUtils::ReadBYTE(pStream);
		BYTE nFrame2		= StreamUtils::ReadBYTE(pStream);
		
		m_dStartTime		= 60000 * nMinute1 + 1000 * nSecond1;
		m_dEndTime			= 60000 * nMinute2 + 1000 * nSecond2;
	}
};