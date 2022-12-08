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

#include "OOXFootnotesReader.h"
#include "OOXTextItemReader.h"

OOXFootnotesReader::OOXFootnotesReader(OOX::CEndnotes * ooxNotes)
{
	m_ooxFootNotes	= NULL;
	m_ooxEndNotes	= ooxNotes;
}
OOXFootnotesReader::OOXFootnotesReader(OOX::CFootnotes * ooxNotes)
{
	m_ooxFootNotes	= ooxNotes;
	m_ooxEndNotes	= NULL;
}
bool OOXFootnotesReader::Parse( ReaderParameter oParam)
{
	int nSeparatorId = 0;
	int nContinueId = 1;

	if (m_ooxEndNotes)
	{
		if( oParam.oRtf->m_oProperty.m_aSpecialEndnotes.size() > 0 )
		{
			for (size_t i = 0 ; i < m_ooxEndNotes->m_arrEndnote.size() ; i++)
			{
				for (size_t j = 0; j < oParam.oRtf->m_oProperty.m_aSpecialEndnotes.size(); j++ )
				{
					if (m_ooxEndNotes->m_arrEndnote[i]->m_oId.IsInit() &&
							m_ooxEndNotes->m_arrEndnote[i]->m_oId->GetValue() == oParam.oRtf->m_oProperty.m_aSpecialEndnotes[j])
					{
						if (m_ooxEndNotes->m_arrEndnote[i]->m_oType.IsInit())
						{
							if (m_ooxEndNotes->m_arrEndnote[i]->m_oType->GetValue() == SimpleTypes::ftnednSeparator)
							{
								nSeparatorId = oParam.oRtf->m_oProperty.m_aSpecialEndnotes[j];
								break;
							}
							else if (m_ooxEndNotes->m_arrEndnote[i]->m_oType->GetValue() == SimpleTypes::ftnednContinuationSeparator)
							{
								nContinueId = oParam.oRtf->m_oProperty.m_aSpecialEndnotes[j];
								break;
							}
						}
					}
				}
			}
		}

		for (size_t i =0 ; i < m_ooxEndNotes->m_arrEndnote.size() ; i++)
		{
			ParseOne(m_ooxEndNotes->m_arrEndnote[i], oParam, false, nSeparatorId, nContinueId);
		}
	}

	if (m_ooxFootNotes)
	{
		if( oParam.oRtf->m_oProperty.m_aSpecialFootnotes.size() > 0 )
		{
			for (size_t i = 0 ; i < m_ooxFootNotes->m_arrFootnote.size() ; i++)
			{
				for (size_t j = 0; j < oParam.oRtf->m_oProperty.m_aSpecialFootnotes.size(); j++ )
				{
					if (m_ooxFootNotes->m_arrFootnote[i]->m_oId.IsInit() &&
							m_ooxFootNotes->m_arrFootnote[i]->m_oId->GetValue() == oParam.oRtf->m_oProperty.m_aSpecialFootnotes[j])
					{
						if (m_ooxFootNotes->m_arrFootnote[i]->m_oType.IsInit())
						{
							if (m_ooxFootNotes->m_arrFootnote[i]->m_oType->GetValue() == SimpleTypes::ftnednSeparator)
							{
								nSeparatorId = oParam.oRtf->m_oProperty.m_aSpecialFootnotes[j];
								break;
							}
							else if (m_ooxFootNotes->m_arrFootnote[i]->m_oType->GetValue() == SimpleTypes::ftnednContinuationSeparator)
							{
								nContinueId = oParam.oRtf->m_oProperty.m_aSpecialFootnotes[j];
								break;
							}
						}
					}
				}
			}
		}
		for (size_t i =0 ; i < m_ooxFootNotes->m_arrFootnote.size() ; i++)
		{
			ParseOne(m_ooxFootNotes->m_arrFootnote[i], oParam, true, nSeparatorId, nContinueId);
		}
	}
	return true;
}
bool OOXFootnotesReader::ParseOne(OOX::CFtnEdn* ooxFtnEdn, ReaderParameter & oParam, bool bFootnote, int nSeparatorId, int nContinueId)
{
	if (ooxFtnEdn == NULL) return false;
	if (ooxFtnEdn->m_oId.IsInit() == false) return false;

	int nId = ooxFtnEdn->m_oId->GetValue();
	OOXTextItemReader oTextItemReader;

	for (size_t i = 0; i < ooxFtnEdn->m_arrItems.size(); ++i)
	{
		if( nSeparatorId == nId )
		{
			TextItemContainerPtr oNewTextItem ( new TextItemContainer() );
			oTextItemReader.m_oTextItems = oNewTextItem;

			if( true == oTextItemReader.Parse( ooxFtnEdn->m_arrItems[i], oParam ) )
			{
				if( true == bFootnote )
					oParam.oRtf->m_oFootnoteSep = oNewTextItem;
				else
					oParam.oRtf->m_oEndnoteSep = oNewTextItem;
			}
		}
		else if( nContinueId == nId )
		{
			TextItemContainerPtr oNewTextItem ( new TextItemContainer() );
			oTextItemReader.m_oTextItems = oNewTextItem;

			if( true == oTextItemReader.Parse( ooxFtnEdn->m_arrItems[i], oParam ) )
			{
				if( true == bFootnote )
					oParam.oRtf->m_oFootnoteCon = oNewTextItem;
				else
					oParam.oRtf->m_oEndnoteCon = oNewTextItem;
			}
		}
		else
		{
			TextItemContainerPtr oNewTextItem ( new TextItemContainer() );
			oTextItemReader.m_oTextItems = oNewTextItem;

			if( true == oTextItemReader.Parse( ooxFtnEdn->m_arrItems[i], oParam ) )
			{
				if( true == bFootnote )
					oParam.oReader->m_mapFootnotes[ nId] = oNewTextItem;
				else
					oParam.oReader->m_mapEndnotes[ nId] = oNewTextItem;
			}
		}
	}
	return true;
}
