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

#include "Font.h"

namespace OOX
{
	CFont::CFont()
	{
		m_sName = _T("Arial");
	}
	CFont::~CFont() {}
	void CFont::fromXML(XmlUtils::CXmlNode& oNode)
	{
		Reset();
		if ( _T("w:font") == oNode.GetName() )
		{
			XmlMacroReadAttributeBase( oNode, _T("w:name"), m_sName );

			XmlUtils::CXmlNode oChild;

			if ( oNode.GetNode( _T("w:altName"), oChild ) )
				XmlMacroReadAttributeBase( oChild, _T("w:val"), m_oAltName );

			if ( oNode.GetNode( _T("w:charset"), oChild ) )
				XmlMacroReadAttributeBase( oChild, _T("w:val"), m_oCharset );

			if ( oNode.GetNode( _T("w:family"), oChild ) )
				XmlMacroReadAttributeBase( oChild, _T("w:val"), m_oFamily );

			if ( oNode.GetNode( _T("w:notTrueType"), oChild ) )
				XmlMacroReadAttributeBase( oChild, _T("w:val"), m_oNotTrueType );

			if ( oNode.GetNode( _T("w:panose1"), oChild ) )
				XmlMacroReadAttributeBase( oChild, _T("w:val"), m_oPanose );

			if ( oNode.GetNode( _T("w:pitch"), oChild ) )
				XmlMacroReadAttributeBase( oChild, _T("w:val"), m_oPitch );

			if ( oNode.GetNode( _T("w:sig"), oChild ) )
			{
				XmlMacroReadAttributeBase( oChild, _T("w:csb0"), m_oCsb0 );
				XmlMacroReadAttributeBase( oChild, _T("w:csb1"), m_oCsb1 );

				XmlMacroReadAttributeBase( oChild, _T("w:usb0"), m_oUsb0 );
				XmlMacroReadAttributeBase( oChild, _T("w:usb1"), m_oUsb1 );
				XmlMacroReadAttributeBase( oChild, _T("w:usb2"), m_oUsb2 );
				XmlMacroReadAttributeBase( oChild, _T("w:usb3"), m_oUsb3 );
			}

		}
	}
	void CFont::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		Reset();
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() ) return;

		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = oReader.GetName();

			if ( sName == L"w:altName")
			{
				WritingElement_ReadAttributes_ReadSingle2  ( oReader, L"w:val", m_oAltName )
			}
			if ( sName == L"w:charset")
			{
				WritingElement_ReadAttributes_ReadSingle2  ( oReader, L"w:val", m_oCharset )
			}
			if ( sName == L"w:family")
			{
				WritingElement_ReadAttributes_ReadSingle2  ( oReader, L"w:val", m_oFamily )
			}

			if ( sName == L"w:notTrueType")
			{
				WritingElement_ReadAttributes_ReadSingle2  ( oReader, L"w:val", m_oNotTrueType )
			}
			if ( sName == L"w:panose1")
			{
				WritingElement_ReadAttributes_ReadSingle2  ( oReader, L"w:val", m_oPanose )
			}
			if ( sName == L"w:pitch")
			{
				WritingElement_ReadAttributes_ReadSingle2  ( oReader, L"w:val", m_oPitch )
			}
			if ( sName == L"w:sig")
			{
				ReadAttributes2( oReader );
			}

		}
	}
	std::wstring CFont::toXML () const
	{
		std::wstring sResult = L"<w:font w:name=\"" + m_sName + L"\">";

		if ( m_oAltName.IsInit() )
		{
			sResult += _T("<w:altName w:val=\"");
			sResult += m_oAltName.get();
			sResult += _T("\"/>");
		}

		if(m_oCharset.IsInit())
		{
			sResult += _T("<w:charset w:val=\"");
			sResult += m_oCharset->ToString();
			sResult += _T("\"/>");
		}
		if(m_oFamily.IsInit())
		{
			sResult += _T("<w:family w:val=\"");
			sResult += m_oFamily->ToString();
			sResult += _T("\"/>");
		}
		if ( m_oNotTrueType.IsInit() )
		{
			sResult += _T("<w:notTrueType w:val=\"");
			sResult += m_oNotTrueType->ToString();
			sResult += _T("\"/>");
		}
		if ( m_oPanose.IsInit() )
		{
			sResult += _T("<w:panose1 w:val=\"");
			sResult += m_oPanose->ToString();
			sResult += _T("\"/>");
		}
		if(m_oPitch.IsInit())
		{
			sResult += _T("<w:pitch w:val=\"");
			sResult += m_oPitch->ToString();
			sResult += _T("\"/>");
		}

		if ( m_oCsb0.IsInit() && m_oCsb1.IsInit() && m_oUsb0.IsInit() && m_oUsb1.IsInit() && m_oUsb2.IsInit() && m_oUsb3.IsInit() )
		{
			sResult += _T("<w:sig w:usb0=\"");
			sResult += m_oUsb0->ToString();
			sResult += _T("\" w:usb1=\"");
			sResult += m_oUsb1->ToString();
			sResult += _T("\" w:usb2=\"");
			sResult += m_oUsb2->ToString();
			sResult += _T("\" w:usb3=\"");
			sResult += m_oUsb3->ToString();
			sResult += _T("\" w:csb0=\"");
			sResult += m_oCsb0->ToString();
			sResult += _T("\" w:csb1=\"");
			sResult += m_oCsb1->ToString();
			sResult += _T("\"/>");
		}
		sResult += L"</w:font>";
		return sResult;
	}
	EElementType CFont::getType() const
	{
		return et_w_font;
	}
	void CFont::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle  ( oReader, L"w:name", m_sName )
		WritingElement_ReadAttributes_End( oReader )
	}
	void CFont::ReadAttributes2(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:csb0"),	m_oCsb0 )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:csb1"),	m_oCsb1 )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:usb0"),	m_oUsb0 )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:usb1"),	m_oUsb1 )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:usb2"),	m_oUsb2 )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:usb3"),	m_oUsb3 )
		WritingElement_ReadAttributes_End( oReader )
	}
	void CFont::Reset()
		{
			m_sName = L"Arial";
			m_oCharset.reset();
			m_oFamily.reset();
			m_oPitch.reset();
			m_oAltName.reset();
			m_oNotTrueType.reset();
			m_oPanose.reset();
			m_oCsb0.reset();
			m_oCsb1.reset();
			m_oUsb0.reset();
			m_oUsb1.reset();
			m_oUsb2.reset();
			m_oUsb3.reset();
		}
}
