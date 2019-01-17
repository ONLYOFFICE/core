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
#include "../Reader/Records.h"
#include "../Records/Animations/AnimationTypes.h"

#include "../Records/SSSlideInfoAtom.h"

class CRecordSlide : public CRecordsContainer
{
public:
	SSlidePersist			m_oPersist;
	SlideProgTagsContainer*	m_pSlideProgTagsContainer;

	bool					m_bExistsTransition;
	
	CSlideShowSlideInfoAtom	m_oSlideShowSlideInfoAtom;

	int						m_Index;
	int						m_IndexUser;
	
private:
	_UINT32 m_lCountReferences;

public:
	
	CRecordSlide()
	{
		m_bExistsTransition			=	false;
		m_pSlideProgTagsContainer	=	NULL;
		m_lCountReferences			=	0;

		m_Index						=	-1;
		m_IndexUser					=	0;

		AddRef();
	}

    virtual ~CRecordSlide()
	{
        Clear();
		RELEASEOBJECT ( m_pSlideProgTagsContainer );
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		m_arRecords.clear();

		LONG lPosition = 0;
		StreamUtils::StreamPosition(lPosition, pStream);

		m_oHeader = oHeader;

		UINT lCurLen = 0;
		SRecordHeader oRec;
	
		while (lCurLen < m_oHeader.RecLen)
		{		
			if (oRec.ReadFromStream(pStream) == FALSE)
			{
				break;
			}

			if ( 0x03F9 == oRec.RecType )
			{
				m_bExistsTransition	=	true;
				m_oSlideShowSlideInfoAtom.ReadFromStream ( oRec, pStream );

				// TODO : временно
				if ( 0 == m_oSlideShowSlideInfoAtom.m_nEffectType )
					m_bExistsTransition	=	false;
				
				lCurLen += (8 + oRec.RecLen);
				continue;
			}

			IRecord* pRecord	=	CreateByType ( oRec );

			if ( RECORD_PROG_TAGS == oRec.RecType )
			{
				m_pSlideProgTagsContainer	=	new	SlideProgTagsContainer ();
				m_pSlideProgTagsContainer->ReadFromStream(oRec, pStream);
			}
			else
			{
				pRecord->ReadFromStream(oRec, pStream);
				m_arRecords.push_back(pRecord);
			}
		
			lCurLen += (8 + oRec.RecLen);
		}
		if (lCurLen != m_oHeader.RecLen)
		{
			// нужно разобраться, что тут такое!!!
			LONG lPosition = 0;
			StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
		}
	}

    bool IsSlide()
	{
		for (size_t nIndex = 0; nIndex < m_arRecords.size(); ++nIndex)
		{
			if ((RECORD_TYPE_SLIDE			== m_arRecords[nIndex]->m_oHeader.RecType) ||
				(RECORD_TYPE_MAINMASTER		== m_arRecords[nIndex]->m_oHeader.RecType) ||
				(RECORD_TYPE_NOTES			== m_arRecords[nIndex]->m_oHeader.RecType))
			{
				return true;
			}
		}
		return false;
	}

	void AddRef()
	{
		++m_lCountReferences;
	}
	void Release()
	{
		--m_lCountReferences;
		if (0 == m_lCountReferences)
			delete this;
	}
	CRecordSlide* QueryInterface()
	{
		AddRef();
		return this;
	}
};
