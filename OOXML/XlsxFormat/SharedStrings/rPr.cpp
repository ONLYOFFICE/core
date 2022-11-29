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

#include "rPr.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CColor::CColor()
		{
		}
		CColor::~CColor()
		{
		}
		void CColor::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		void CColor::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("auto"),	m_oAuto )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("indexed"),	m_oIndexed )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("rgb"),		m_oRgb )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("theme"),	m_oTheme )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("tint"),	m_oTint )
			WritingElement_ReadAttributes_End( oReader )
		}

		CRPr::CRPr()
		{
		}
		CRPr::~CRPr()
		{
		}
		void CRPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("b") == sName )
					m_oBold = oReader;
				else if ( _T("charset") == sName )
					;//pItem = new CText( oReader );
				else if ( _T("color") == sName )
					m_oColor = oReader;
				else if ( _T("condense") == sName )
					m_oCondense = oReader;
				else if ( _T("extend") == sName )
					m_oExtend = oReader;
				else if ( _T("family") == sName )
					;//m_oFamily = oReader;
				else if ( _T("i") == sName )
					m_oItalic = oReader;
				else if ( _T("outline") == sName )
					m_oOutline = oReader;
				else if ( _T("rFont") == sName )
					m_oRFont = oReader;
				else if ( _T("scheme") == sName )
					;//m_oScheme = oReader;
				else if ( _T("shadow") == sName )
					m_oShadow = oReader;
				else if ( _T("strike") == sName )
					m_oStrike = oReader;
				else if ( _T("sz") == sName )
					m_oSz = oReader;
				else if ( _T("u") == sName )
					m_oUnderline = oReader;
				else if ( _T("vertAlign") == sName )
					m_oVertAlign = oReader;

			}
		}
		EElementType CRPr::getType () const
		{
			return et_rPr;
		}
		void CRPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

	} //Spreadsheet
} // namespace OOX
