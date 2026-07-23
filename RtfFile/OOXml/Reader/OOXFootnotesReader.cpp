/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
