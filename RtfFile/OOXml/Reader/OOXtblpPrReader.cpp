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

#include "OOXtblpPrReader.h"

OOXtblpPrReader::OOXtblpPrReader(ComplexTypes::Word::CTblPPr * ooxTblPr)
{
	m_ooxTblPr = ooxTblPr;
}
bool OOXtblpPrReader::Parse( ReaderParameter oParam , RtfTableProperty& oOutputProperty)
{
	if (m_ooxTblPr == NULL) return false;

	if(m_ooxTblPr->m_oLeftFromText.IsInit() )
		oOutputProperty.m_nWrapLeft = m_ooxTblPr->m_oLeftFromText->ToTwips();
	if(m_ooxTblPr->m_oRightFromText.IsInit() )
		oOutputProperty.m_nWrapRight = m_ooxTblPr->m_oRightFromText->ToTwips();
	if( m_ooxTblPr->m_oTopFromText.IsInit() )
		oOutputProperty.m_nWrapTop = m_ooxTblPr->m_oTopFromText->ToTwips();
	if( m_ooxTblPr->m_oBottomFromText.IsInit() )
		oOutputProperty.m_nWrapBottom = m_ooxTblPr->m_oBottomFromText->ToTwips();

	if( m_ooxTblPr->m_oTblpX.IsInit())
		oOutputProperty.m_nHPos = m_ooxTblPr->m_oTblpX->ToTwips();
	if( m_ooxTblPr->m_oTblpY.IsInit())
		oOutputProperty.m_nVPos = m_ooxTblPr->m_oTblpY->ToTwips();

	if (m_ooxTblPr->m_oHorzAnchor.IsInit())
	{
		switch(m_ooxTblPr->m_oHorzAnchor->GetValue())
		{
		case SimpleTypes::hanchorMargin : oOutputProperty.m_eHRef = RtfTableProperty::hr_phmrg; break;
		case SimpleTypes::hanchorPage   : oOutputProperty.m_eHRef = RtfTableProperty::hr_phpg; break;
		case SimpleTypes::hanchorText   : oOutputProperty.m_eHRef = RtfTableProperty::hr_phcol; break;
		}
	}
	if (m_ooxTblPr->m_oTblpXSpec.IsInit())
	{
		switch(m_ooxTblPr->m_oTblpXSpec->GetValue())
		{
		case SimpleTypes::xalignCenter  : oOutputProperty.m_eHPos = RtfTableProperty::hp_posxc; break;
		case SimpleTypes::xalignInside  : oOutputProperty.m_eHPos = RtfTableProperty::hp_posxi; break;
		case SimpleTypes::xalignLeft    : oOutputProperty.m_eHPos = RtfTableProperty::hp_posxl; break;
		case SimpleTypes::xalignOutside : oOutputProperty.m_eHPos = RtfTableProperty::hp_posxo; break;
		case SimpleTypes::xalignRight   : oOutputProperty.m_eHPos = RtfTableProperty::hp_posxr; break;
		}
	}

	if (m_ooxTblPr->m_oVertAnchor.IsInit())
	{
		switch(m_ooxTblPr->m_oVertAnchor->GetValue())
		{
		case SimpleTypes::vanchorMargin : oOutputProperty.m_eVRef = RtfTableProperty::vr_pvmrg; break;
		case SimpleTypes::vanchorPage   : oOutputProperty.m_eVRef = RtfTableProperty::vr_pvpg; break;
		case SimpleTypes::vanchorText   : oOutputProperty.m_eVRef = RtfTableProperty::vr_pvpara; break;
		}
	}
	if (m_ooxTblPr->m_oTblpYSpec.IsInit())
	{
		switch(m_ooxTblPr->m_oTblpYSpec->GetValue())
		{
		case SimpleTypes::yalignBottom  : oOutputProperty.m_eVPos = RtfTableProperty::vp_posyb; break;
		case SimpleTypes::yalignCenter  : oOutputProperty.m_eVPos = RtfTableProperty::vp_posyc; break;
		case SimpleTypes::yalignInline  : break;
		case SimpleTypes::yalignInside  : oOutputProperty.m_eVPos = RtfTableProperty::vp_posyin; break;
		case SimpleTypes::yalignOutside : oOutputProperty.m_eVPos = RtfTableProperty::vp_posyout; break;
		case SimpleTypes::yalignTop     : oOutputProperty.m_eVPos = RtfTableProperty::vp_posyt; break;
		}
	}
	return true;
}
