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

