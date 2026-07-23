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

#include "VmlWord.h"
#include "../../Common/SimpleTypes_Vml.h"

namespace OOX
{
	namespace VmlWord
	{
		//--------------------------------------------------------------------------------
		// CAnchorLock 14.3.2.1 (Part 4)
		//--------------------------------------------------------------------------------

		CAnchorLock::CAnchorLock(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CAnchorLock::~CAnchorLock()
		{
		}
		void CAnchorLock::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CAnchorLock::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CAnchorLock::toXML() const
		{
			return _T("<wd:anchorLock/>");
		}
		EElementType CAnchorLock::getType() const
		{
			return OOX::et_wd_anchorLock;
		}

		//--------------------------------------------------------------------------------
		// CBorder 14.3.2.2;14.3.2.3;14.3.2.4;14.3.2.5 (Part 4)
		//--------------------------------------------------------------------------------

		CBorder::CBorder(OOX::Document *pMain) : WritingElement(pMain)
		{
			m_eType = et_Unknown;
		}
		CBorder::~CBorder()
		{
		}
		void CBorder::fromXML(XmlUtils::CXmlNode& oNode)
		{
			m_eType = et_Unknown;
			// TO DO: Implement CBorder::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CBorder::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_eType = et_Unknown;
			std::wstring sName = oReader.GetName();

			if ( _T("wd:borderbottom") == sName )
				m_eType = et_wd_borderbottom;
			else if ( _T("wd:borderleft") == sName )
				m_eType = et_wd_borderleft;
			else if ( _T("wd:borderright") == sName )
				m_eType = et_wd_borderright;
			else if ( _T("wd:bordertop") == sName )
				m_eType = et_wd_bordertop;
			else
				return;

			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CBorder::toXML() const
		{
			std::wstring sResult;
			switch ( m_eType )
			{
			case et_wd_borderbottom: sResult = _T("<wd:borderbottom "); break;
			case et_wd_borderleft:   sResult = _T("<wd:borderleft "); break;
			case et_wd_borderright:  sResult = _T("<wd:borderright "); break;
			case et_wd_bordertop:    sResult = _T("<wd:bordertop "); break;
			default: return _T("");
			}

			if ( m_oType.IsInit() )
				sResult += _T("wd:type=\"") + m_oType->ToString() + _T("\" ");

			if ( m_oWidth.IsInit() )
				sResult += _T("wd:width=\"") + m_oWidth->ToString() + _T("\" ");

			if ( m_oShadow.IsInit() )
				sResult += _T("wd:shadow=\"") + m_oShadow->ToString() + _T("\" ");

			sResult += _T("/>");

			return sResult;
		}
		EElementType CBorder::getType() const
		{
			return m_eType;
		}
		void CBorder::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("wd:shadow"), m_oShadow )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("wd:type"),   m_oType )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("wd:width"),  m_oWidth )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// CWrap 14.3.2.6 (Part 4)
		//--------------------------------------------------------------------------------

		CWrap::CWrap(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CWrap::~CWrap()
		{
		}
		void CWrap::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Implement CWrap::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CWrap::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CWrap::toXML() const
		{
			std::wstring sResult = _T("<w10:wrap ");

			if ( m_oType.IsInit() )
				sResult += _T("type=\"") + m_oType->ToString() + _T("\" ");

			if ( m_oSide.IsInit() )
				sResult += _T("side=\"") + m_oSide->ToString() + _T("\" ");

			if ( m_oAnchorX.IsInit() )
				sResult += _T("anchorx=\"") + m_oAnchorX->ToString() + _T("\" ");

			if ( m_oAnchorY.IsInit() )
				sResult += _T("anchory=\"") + m_oAnchorY->ToString() + _T("\" ");

			sResult += _T("/>");

			return sResult;
		}
		EElementType CWrap::getType() const
		{
			return OOX::et_wd_wrap;
		}
		void CWrap::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("anchorx"), m_oAnchorX )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("anchory"), m_oAnchorY )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("side"),    m_oSide )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("type"),    m_oType )
				WritingElement_ReadAttributes_End( oReader )
			}

	} // namespace Vml
} // namespace OOX
