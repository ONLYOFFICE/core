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

#include "VmlWord.h"

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
			// TO DO: Реализовать CBorder::fromXML(XmlUtils::CXmlNode& oNode)
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
			// TO DO: Реализовать CWrap::fromXML(XmlUtils::CXmlNode& oNode)
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
