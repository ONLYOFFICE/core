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

class CRecordDocumentAtom : public CUnknownRecord
{
public:
	SPointAtom m_oSlideSize;
	SPointAtom m_oNotesSize;

	SRatioAtom m_oServerZoom; 
	
	UINT m_nNotesMasterPersistIDRef;
	UINT m_nHandoutMasterPersistIDRef;
	
	USHORT m_nFirstSlideNum; 
	NSOfficePPT::SlideSizeType m_nSlideSizeType;

	BOOL1 m_bSaveWithFonts;  
	BOOL1 m_bOmitTitlePlace;

	BOOL1 m_bRightToLeft;

	BOOL1 m_bShowComments;

public:
	
	CRecordDocumentAtom()
	{
	}

	~CRecordDocumentAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_oSlideSize.X = StreamUtils::ReadLONG(pStream);
		m_oSlideSize.Y = StreamUtils::ReadLONG(pStream);

		m_oNotesSize.X = StreamUtils::ReadLONG(pStream);
		m_oNotesSize.Y = StreamUtils::ReadLONG(pStream);

		m_oServerZoom.Number = StreamUtils::ReadLONG(pStream);
		m_oServerZoom.Denom = StreamUtils::ReadLONG(pStream);

		m_nNotesMasterPersistIDRef = (UINT)StreamUtils::ReadDWORD(pStream);
		m_nHandoutMasterPersistIDRef = (UINT)StreamUtils::ReadDWORD(pStream);

		m_nFirstSlideNum = StreamUtils::ReadWORD(pStream);
		m_nSlideSizeType = (NSOfficePPT::SlideSizeType)StreamUtils::ReadWORD(pStream);

		m_bSaveWithFonts = StreamUtils::ReadBYTE(pStream);
		m_bOmitTitlePlace = StreamUtils::ReadBYTE(pStream);
		m_bRightToLeft = StreamUtils::ReadBYTE(pStream);
		m_bShowComments = StreamUtils::ReadBYTE(pStream);
	}

};