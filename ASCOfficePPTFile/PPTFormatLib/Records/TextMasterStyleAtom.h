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

class CRecordTextMasterStyleAtom : public CUnknownRecord, public NSPresentationEditor::CTextStyles
{
public:
	LONG m_nTextType;
	
	CRecordTextMasterStyleAtom() : NSPresentationEditor::CTextStyles()
	{
		m_nTextType = 0;
	}

	~CRecordTextMasterStyleAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		LONG lPosition = 0;
		StreamUtils::StreamPosition(lPosition, pStream);

		m_nTextType = m_oHeader.RecInstance;

		bool bIsLevelsPresent = (0x05 <= m_oHeader.RecInstance);
		
		int lLevels = StreamUtils::ReadWORD(pStream);
		if (0 < lLevels)
			LoadLevel(0, pStream, bIsLevelsPresent);
		if (1 < lLevels)
			LoadLevel(1, pStream, bIsLevelsPresent);
		if (2 < lLevels)
			LoadLevel(2, pStream, bIsLevelsPresent);
		if (3 < lLevels)
			LoadLevel(3, pStream, bIsLevelsPresent);
		if (4 < lLevels)
			LoadLevel(4, pStream, bIsLevelsPresent);

		// походу нужно делать так: ----------------------------------------------
		if (m_pLevels[0].is_init())
		{
			if (!m_pLevels[1].is_init())
				m_pLevels[1] = m_pLevels[0].get();
			else
				m_pLevels[1]->ApplyBefore(m_pLevels[0].get());
		}
		if (m_pLevels[1].is_init())
		{
			if (!m_pLevels[2].is_init())
				m_pLevels[2] = m_pLevels[1].get();
			else
				m_pLevels[2]->ApplyBefore(m_pLevels[1].get());
		}
		if (m_pLevels[2].is_init())
		{
			if (!m_pLevels[3].is_init())
				m_pLevels[3] = m_pLevels[2].get();
			else
				m_pLevels[3]->ApplyBefore(m_pLevels[2].get());
		}
		if (m_pLevels[3].is_init())
		{
			if (!m_pLevels[4].is_init())
				m_pLevels[4] = m_pLevels[3].get();
			else
				m_pLevels[4]->ApplyBefore(m_pLevels[3].get());
		}
		// -----------------------------------------------------------------------

		// это на всякий случай...
		StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
	}
protected:
	void LoadLevel(LONG lLevel, POLE::Stream* pStream, bool bIsLevelPresent)
	{
		LONG lLevelOld = lLevel;
		
		if (bIsLevelPresent)
			lLevel = StreamUtils::ReadSHORT(pStream);

		CTextPFRun_ppt oPF;
		CTextCFRun_ppt oCF;
		oPF.LoadFromStream(pStream, false);
		oCF.LoadFromStream(pStream, false);

		m_pLevels[lLevelOld] = new NSPresentationEditor::CTextStyleLevel();
		m_pLevels[lLevelOld]->m_oPFRun = oPF.m_oRun;
		m_pLevels[lLevelOld]->m_oCFRun = oCF.m_oRun;
	}
};