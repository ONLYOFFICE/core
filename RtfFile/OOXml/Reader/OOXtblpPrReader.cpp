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
