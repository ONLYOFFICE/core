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

#include "OOXLevelReader.h"
#include "OOXpPrReader.h"

OOXLevelReader::OOXLevelReader(OOX::Numbering::CLvl* ooxLvlNum)
{
	m_ooxLvlNum = ooxLvlNum;
}
bool OOXLevelReader::Parse( ReaderParameter oParam, RtfListLevelProperty& oLevelProperty )
{
	if (m_ooxLvlNum == NULL) return false;

	if (m_ooxLvlNum->m_oIlvl.IsInit())
		oLevelProperty.m_nLevel = *m_ooxLvlNum->m_oIlvl;

	if (m_ooxLvlNum->m_oTentative.IsInit())
		oLevelProperty.m_bTentative = m_ooxLvlNum->m_oTentative->ToBool();

	if (m_ooxLvlNum->m_oLvlJc.IsInit() && m_ooxLvlNum->m_oLvlJc->m_oVal.IsInit())
	{
		switch(m_ooxLvlNum->m_oLvlJc->m_oVal->GetValue())
		{
		case SimpleTypes::jcCenter          : oLevelProperty.m_nJustification = 1; break;
		case SimpleTypes::jcEnd             : oLevelProperty.m_nJustification = 2; break;
		case SimpleTypes::jcStart           : oLevelProperty.m_nJustification = 0; break;
		case SimpleTypes::jcLeft            : oLevelProperty.m_nJustification = 0; break;
		case SimpleTypes::jcRight           : oLevelProperty.m_nJustification = 2; break;
		}
	}
	if (m_ooxLvlNum->m_oLvlRestart.IsInit() && m_ooxLvlNum->m_oLvlRestart->m_oVal.IsInit())
	{
		int nVal = *m_ooxLvlNum->m_oLvlRestart->m_oVal;
		if( 0 == nVal )
			oLevelProperty.m_nNoRestart = 0;
		//todooo unclear what happens if list restart is not "0"
	}
	if (m_ooxLvlNum->m_oIsLgl.IsInit() )
		oLevelProperty.m_nLegal = m_ooxLvlNum->m_oIsLgl->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxLvlNum->m_oLvlText.IsInit() && m_ooxLvlNum->m_oLvlText->m_sVal.IsInit())
	{
		oLevelProperty.SetLevelTextOOX( *m_ooxLvlNum->m_oLvlText->m_sVal);
		//null???? todooo
	}
	if (m_ooxLvlNum->m_oNumFmt.IsInit() && m_ooxLvlNum->m_oNumFmt->m_oVal.IsInit())
	{
		oLevelProperty.m_nNumberType = oLevelProperty.GetFormat( m_ooxLvlNum->m_oNumFmt->m_oVal->ToString() );
	}

	if (m_ooxLvlNum->m_oLvlPicBulletId.IsInit() && m_ooxLvlNum->m_oLvlPicBulletId->m_oVal.IsInit())
	{
		int nID = *m_ooxLvlNum->m_oLvlPicBulletId->m_oVal;
		std::map<int, int>::iterator poPair =  oParam.oReader->m_mapPictureBullet.find( nID );

		if( oParam.oReader->m_mapPictureBullet.end() != poPair )
		{
			oLevelProperty.m_nPictureIndex = poPair->second;
		}
	}

	if (m_ooxLvlNum->m_oStart.IsInit() && m_ooxLvlNum->m_oStart->m_oVal.IsInit())
		oLevelProperty.m_nStart = *m_ooxLvlNum->m_oStart->m_oVal;

	if (m_ooxLvlNum->m_oSuffix.IsInit() && m_ooxLvlNum->m_oSuffix->m_oVal.IsInit())
	{
		switch(m_ooxLvlNum->m_oSuffix->m_oVal->GetValue())
		{
		case SimpleTypes::levelsuffixSpace   : oLevelProperty.m_nFollow = 1; break;
		case SimpleTypes::levelsuffixTab     : oLevelProperty.m_nFollow = 0; break;
		case SimpleTypes::levelsuffixNothing : oLevelProperty.m_nFollow = 2; break;
		}
	}

	if (m_ooxLvlNum->m_oRPr.IsInit())
	{
		RtfCharProperty oNewProperty;
		oNewProperty.SetDefaultOOX();

		OOXrPrReader orPrReader(m_ooxLvlNum->m_oRPr.GetPointer());
		orPrReader.m_bDefStyle = false;

		if( true == orPrReader.Parse( oParam, oNewProperty ) )
			oLevelProperty.m_oCharProp = oNewProperty;
	}

	if (m_ooxLvlNum->m_oPPr.IsInit())
	{
		RtfParagraphProperty oNewProperty;
		OOXpPrReader opPrReader(m_ooxLvlNum->m_oPPr.GetPointer());

		CcnfStyle style;
		if( true == opPrReader.Parse( oParam, oNewProperty, style) )
		{
			oLevelProperty.m_nFirstIndent	= oNewProperty.m_nIndFirstLine;
			oLevelProperty.m_nIndent		= oNewProperty.m_nIndLeft;
			oLevelProperty.m_nIndentStart	= oNewProperty.m_nIndStart;
			oLevelProperty.m_oTabs			= oNewProperty.m_oTabs;
		}
	}
	return true;
}
