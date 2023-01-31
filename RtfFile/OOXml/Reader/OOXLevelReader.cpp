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
		//todooo непонятка что будет если рестарт списка с не "0"
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
