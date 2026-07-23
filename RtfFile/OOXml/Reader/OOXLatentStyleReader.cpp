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

#include "OOXLatentStyleReader.h"

OOXLatentStyleReader::OOXLatentStyleReader()
{
}
bool OOXLatentStyleReader::Parse( ReaderParameter oParam )
{
	//XmlUtils::CXmlLiteReader oXmlReader;
	//if( TRUE ==  oXmlReader.OpenFromXmlNode(oParam.oNode) )
	//{
	//	std::wstring nCount = oXmlReader.ReadNodeAttribute( L"w:count" );
	//	if( !nCount.empty() )
	//		oParam.oRtf->m_oLatentStyleTable.m_nCount = Strings::ToInteger( nCount );
	//	std::wstring nLocked = oXmlReader.ReadNodeAttribute( L"w:defLockedState" );
	//	if( !nLocked.empty() )
	//		oParam.oRtf->m_oLatentStyleTable.m_nLocked= Strings::ToInteger( nLocked );
	//	std::wstring nSemiHid = oXmlReader.ReadNodeAttribute( L"w:defSemiHidden" );
	//	if( !nSemiHid.empty() )
	//		oParam.oRtf->m_oLatentStyleTable.m_nSemiHidden= Strings::ToInteger( nSemiHid );
	//	std::wstring nUNhidWU = oXmlReader.ReadNodeAttribute( L"w:defUnhideWhenUsed" );
	//	if( !nUNhidWU.empty() )
	//		oParam.oRtf->m_oLatentStyleTable.m_nUnHiddenWhenUse= Strings::ToInteger( nUNhidWU );
	//	std::wstring nQFormat = oXmlReader.ReadNodeAttribute( L"w:defQFormat" );
	//	if( !nQFormat.empty() )
	//		oParam.oRtf->m_oLatentStyleTable.m_nQFormat= Strings::ToInteger( nQFormat );
	//	std::wstring nUIPriority = oXmlReader.ReadNodeAttribute( L"w:defUIPriority" );
	//	if( !nUIPriority.empty() )
	//		oParam.oRtf->m_oLatentStyleTable.m_nPriority= Strings::ToInteger( nUIPriority );

	//	oXmlReader.ReadNodeList( L"w:lsdException" );
	//	for (size_t i = 0; i < oXmlReader.GetLengthList(); i++ )
	//	{
	//		RtfStyleException oNewException;
	//		oNewException.m_sName	= oXmlReader.ReadNodeAttribute( L"w:name", L"" );
	//		std::wstring nPriority		= oXmlReader.ReadNodeAttribute( L"w:uiPriority");

	//		if( !nPriority.empty() )
	//			oNewException.m_nPriority = Strings::ToInteger( nPriority );
	//		std::wstring nHiddenWhenUse = oXmlReader.ReadNodeAttribute( L"w:unhideWhenUsed");
	//		if( !nHiddenWhenUse.empty() )
	//			oNewException.m_nHiddenWhenUse = Strings::ToInteger( nHiddenWhenUse );
	//		std::wstring nQFormat = oXmlReader.ReadNodeAttribute( L"w:qFormat" );
	//		if( !nQFormat.empty() )
	//			oNewException.m_nQFormat = Strings::ToInteger( nQFormat );
	//		std::wstring nLocked = oXmlReader.ReadNodeAttribute( L"w:locked" );
	//		if( !nLocked.empty() )
	//			oNewException.m_nLocked = Strings::ToInteger( nLocked );
	//		std::wstring nSemiHidden = oXmlReader.ReadNodeAttribute( L"w:semiHidden");
	//		if( !nSemiHidden.empty() )
	//			oNewException.m_nSemiHidden = Strings::ToInteger( nSemiHidden );

	//		if( true == oNewException.IsValid() )
	//			oParam.oRtf->m_oLatentStyleTable.AddItem( oNewException );
	//	}

	//}
	return false;
}

