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

#include "OOXpPrTabReader.h"

OOXpPrTabReader::OOXpPrTabReader( OOX::Logic::CTabs *ooxTabs)
{
	m_ooxTabs = ooxTabs;
}
bool OOXpPrTabReader::Parse( ReaderParameter oParam, RtfTabs& oOutputProperty)
{
	if (m_ooxTabs == NULL) return false;

	oOutputProperty.SetDefault();

	for (size_t i = 0; i < m_ooxTabs->m_arrTabs.size() ; i++ )
	{
		if (m_ooxTabs->m_arrTabs[i] == NULL) continue;

		RtfTab oCurTab;

		if (m_ooxTabs->m_arrTabs[i]->m_oLeader.IsInit())
		{
			switch(m_ooxTabs->m_arrTabs[i]->m_oLeader->GetValue())
			{
			case SimpleTypes::tabtlcDot        : oCurTab.m_eLeader = RtfTab::tl_dot;	break;
			case SimpleTypes::tabtlcHeavy      : break;
			case SimpleTypes::tabtlcHyphen     : oCurTab.m_eLeader = RtfTab::tl_hyph;	break;
			case SimpleTypes::tabtlcMiddleDot  : oCurTab.m_eLeader = RtfTab::tl_mdot;	break;
			case SimpleTypes::tabtlcNone       : break;
			case SimpleTypes::tabtlcUnderscore : oCurTab.m_eLeader = RtfTab::tl_ul;		break;
			default:
				break;
			}
		}
		if (m_ooxTabs->m_arrTabs[i]->m_oVal.IsInit())
		{
			switch(m_ooxTabs->m_arrTabs[i]->m_oVal->GetValue())
			{
			case SimpleTypes::tabjcBar     : oCurTab.m_eKind = RtfTab::tk_tqbar;	break;
			case SimpleTypes::tabjcCenter  : oCurTab.m_eKind = RtfTab::tk_tqc;		break;
			case SimpleTypes::tabjcClear   : break;
			case SimpleTypes::tabjcDecimal : oCurTab.m_eKind = RtfTab::tk_tqdec;	break;
			case SimpleTypes::tabjcEnd     : oCurTab.m_eKind = RtfTab::tk_tqr;		break;
			case SimpleTypes::tabjcNum     : oCurTab.m_eKind = RtfTab::tk_tqdec; 	break;//??
			case SimpleTypes::tabjcStart   : oCurTab.m_eKind = RtfTab::tk_tql;		break;
			case SimpleTypes::tabjcRight   : oCurTab.m_eKind = RtfTab::tk_tqr;		break;
			case SimpleTypes::tabjcLeft    : oCurTab.m_eKind = RtfTab::tk_tql;		break;
			default:
				break;
			}
		}
		if (m_ooxTabs->m_arrTabs[i]->m_oPos.IsInit())
			oCurTab.m_nTab = m_ooxTabs->m_arrTabs[i]->m_oPos->ToTwips();

		oOutputProperty.m_aTabs.push_back( oCurTab );
	}
	return true;
}
