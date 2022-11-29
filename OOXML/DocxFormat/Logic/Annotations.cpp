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
#include "Annotations.h"
#include "Bdo.h"
#include "Paragraph.h"
#include "Annotations.h"
#include "Run.h"
#include "FldSimple.h"
#include "Sdt.h"
#include "Hyperlink.h"
#include "SmartTag.h"
#include "Dir.h"
#include "../Math/oMathPara.h"
#include "../Math/OMath.h"

// TO DO: Нехватающие классы:
//        <w:customXml>
//        <w:subDoc>


namespace OOX
{
	namespace Logic
	{
		CCommentRangeEnd::CCommentRangeEnd(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CCommentRangeEnd::CCommentRangeEnd(CCommentRangeEnd *pOther) : WritingElement(NULL)
		{
			m_oDisplacedByCustomXml = pOther->m_oDisplacedByCustomXml;
			m_oId                   = pOther->m_oId;
		}
		CCommentRangeEnd::~CCommentRangeEnd()
		{
		}
		void CCommentRangeEnd::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml );
			XmlMacroReadAttributeBase( oNode, L"w:id",                   m_oId );
		}
		void CCommentRangeEnd::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CCommentRangeEnd::toXML() const
		{
			std::wstring sResult = L"<w:commentRangeEnd";

			ComplexTypes_WriteAttribute_( L" w:displacedbyCustomXml=\"", m_oDisplacedByCustomXml );
			ComplexTypes_WriteAttribute_( L" w:id=\"",                   m_oId );

			sResult += L"/>";

			return sResult;
		}
		EElementType CCommentRangeEnd::getType() const
		{
			return et_w_commentRangeEnd;
		}
		void CCommentRangeEnd::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",                   m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"aml:id",					m_oId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CCommentRangeStart::CCommentRangeStart(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CCommentRangeStart::~CCommentRangeStart()
		{
		}
		void CCommentRangeStart::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml );
			XmlMacroReadAttributeBase( oNode, L"w:id",                   m_oId );
		}
		void CCommentRangeStart::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CCommentRangeStart::toXML() const
		{
			std::wstring sResult = L"<w:commentRangeStart ";

			ComplexTypes_WriteAttribute_( L" w:displacedbyCustomXml=\"", m_oDisplacedByCustomXml );
			ComplexTypes_WriteAttribute_( L" w:id=\"",                   m_oId );

			sResult += L"/>";

			return sResult;
		}
		EElementType CCommentRangeStart::getType() const
		{
			return et_w_commentRangeStart;
		}
		void CCommentRangeStart::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",                   m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"aml:id",					m_oId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CCustomXmlDelRangeEnd::CCustomXmlDelRangeEnd(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CCustomXmlDelRangeEnd::~CCustomXmlDelRangeEnd()
		{
		}
		void CCustomXmlDelRangeEnd::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase(oNode, L"w:id", m_oId );
		}
		void CCustomXmlDelRangeEnd::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CCustomXmlDelRangeEnd::toXML() const
		{
			std::wstring sResult = L"<w:customXmlDelRangeEnd";

			ComplexTypes_WriteAttribute_( L" w:id=\"", m_oId );

			sResult += L"/>";

			return sResult;
		}
		EElementType CCustomXmlDelRangeEnd::getType() const
		{
			return et_w_customXmlDelRangeEnd;
		}
		void CCustomXmlDelRangeEnd::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, L"w:id", m_oId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CCustomXmlDelRangeStart::CCustomXmlDelRangeStart(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CCustomXmlDelRangeStart::~CCustomXmlDelRangeStart()
		{
		}
		void CCustomXmlDelRangeStart::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:author", m_sAuthor );
			XmlMacroReadAttributeBase( oNode, L"w:date",   m_oDate );
			XmlMacroReadAttributeBase( oNode, L"w:id",     m_oID );
			XmlMacroReadAttributeBase( oNode, L"oouserid", m_sUserId );
		}
		void CCustomXmlDelRangeStart::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CCustomXmlDelRangeStart::toXML() const
		{
			std::wstring sResult = L"<w:customXmlDelRangeStart ";

			if ( m_sAuthor.IsInit() )
			{
				sResult += L"w:author=\"";
				sResult += XmlUtils::EncodeXmlString(m_sAuthor.get2());
				sResult += L"\" ";
			}

			if ( m_oDate.IsInit() )
			{
				sResult += L"w:date=\"";
				sResult += m_oDate->ToString();
				sResult += L"\" ";
			}

			if ( m_oID.IsInit() )
			{
				sResult += L"w:id=\"";
				sResult += m_oID->ToString();
				sResult += L"\" ";
			}

			if ( m_sUserId.IsInit() )
			{
				sResult += L"oouserid=\"";
				sResult += m_sUserId.get2();
				sResult += L"\" ";
			}

			sResult += L"/>";

			return sResult;
		}
		EElementType CCustomXmlDelRangeStart::getType() const
		{
			return et_w_customXmlDelRangeStart;
		}
		void CCustomXmlDelRangeStart::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w:author", m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:date",   m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",     m_oID )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"oouserid", m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CCustomXmlInsRangeEnd::CCustomXmlInsRangeEnd(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CCustomXmlInsRangeEnd::~CCustomXmlInsRangeEnd()
		{
		}
		void CCustomXmlInsRangeEnd::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:id", m_oId );
		}
		void CCustomXmlInsRangeEnd::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CCustomXmlInsRangeEnd::toXML() const
		{
			std::wstring sResult = L"<w:customXmlInsRangeEnd";

			ComplexTypes_WriteAttribute_( L" w:id=\"", m_oId );

			sResult += L"/>";

			return sResult;
		}
		EElementType CCustomXmlInsRangeEnd::getType() const
		{
			return et_w_customXmlInsRangeEnd;
		}
		void CCustomXmlInsRangeEnd::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, L"w:id", m_oId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CCustomXmlInsRangeStart::CCustomXmlInsRangeStart(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CCustomXmlInsRangeStart::~CCustomXmlInsRangeStart()
		{
		}
		void CCustomXmlInsRangeStart::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:author", m_sAuthor );
			XmlMacroReadAttributeBase( oNode, L"w:date",   m_oDate );
			XmlMacroReadAttributeBase( oNode, L"w:id",     m_oID );
			XmlMacroReadAttributeBase( oNode, L"oouserid", m_sUserId );
		}
		void CCustomXmlInsRangeStart::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CCustomXmlInsRangeStart::toXML() const
		{
			std::wstring sResult = L"<w:customXmlInsRangeStart ";

			if ( m_sAuthor.IsInit() )
			{
				sResult += L"w:author=\"";
				sResult += XmlUtils::EncodeXmlString(m_sAuthor.get2());
				sResult += L"\" ";
			}

			if ( m_oDate.IsInit() )
			{
				sResult += L"w:date=\"";
				sResult += m_oDate->GetValue();
				sResult += L"\" ";
			}

			if ( m_oID.IsInit() )
			{
				sResult += L"w:id=\"";
				sResult += m_oID->ToString();
				sResult += L"\" ";
			}

			if ( m_sUserId.IsInit() )
			{
				sResult += L"oouserid=\"";
				sResult += m_sUserId.get2();
				sResult += L"\" ";
			}

			sResult += L"/>";

			return sResult;
		}
		EElementType CCustomXmlInsRangeStart::getType() const
		{
			return et_w_customXmlInsRangeStart;
		}
		void CCustomXmlInsRangeStart::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w:author", m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:date",   m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",     m_oID )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"oouserid", m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CCustomXmlMoveFromRangeEnd::CCustomXmlMoveFromRangeEnd(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CCustomXmlMoveFromRangeEnd::~CCustomXmlMoveFromRangeEnd()
		{
		}
		void CCustomXmlMoveFromRangeEnd::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:id", m_oId );
		}
		void CCustomXmlMoveFromRangeEnd::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CCustomXmlMoveFromRangeEnd::toXML() const
		{
			std::wstring sResult = L"<w:customXmlMoveFromRangeEnd";

			ComplexTypes_WriteAttribute_( L" w:id=\"", m_oId );

			sResult += L"/>";

			return sResult;
		}
		EElementType CCustomXmlMoveFromRangeEnd::getType() const
		{
			return et_w_customXmlMoveFromRangeEnd;
		}
		void CCustomXmlMoveFromRangeEnd::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, L"w:id", m_oId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CCustomXmlMoveFromRangeStart::CCustomXmlMoveFromRangeStart(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CCustomXmlMoveFromRangeStart::~CCustomXmlMoveFromRangeStart()
		{
		}
		void CCustomXmlMoveFromRangeStart::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:author", m_sAuthor );
			XmlMacroReadAttributeBase( oNode, L"w:date",   m_oDate );
			XmlMacroReadAttributeBase( oNode, L"w:id",     m_oID );
			XmlMacroReadAttributeBase( oNode, L"oouserid", m_sUserId );
		}
		void CCustomXmlMoveFromRangeStart::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CCustomXmlMoveFromRangeStart::toXML() const
		{
			std::wstring sResult = L"<w:customXmlMoveFromRangeStart ";

			if ( m_sAuthor.IsInit() )
			{
				sResult += L"w:author=\"";
				sResult += XmlUtils::EncodeXmlString(m_sAuthor.get2());
				sResult += L"\" ";
			}

			if ( m_oDate.IsInit() )
			{
				sResult += L"w:date=\"";
				sResult += m_oDate->ToString();
				sResult += L"\" ";
			}

			if ( m_oID.IsInit() )
			{
				sResult += L"w:id=\"";
				sResult += m_oID->ToString();
				sResult += L"\" ";
			}

			if ( m_sUserId.IsInit() )
			{
				sResult += L"oouserid=\"";
				sResult += m_sUserId.get2();
				sResult += L"\" ";
			}

			sResult += L"/>";

			return sResult;
		}
		EElementType CCustomXmlMoveFromRangeStart::getType() const
		{
			return et_w_customXmlMoveFromRangeStart;
		}
		void CCustomXmlMoveFromRangeStart::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w:author",    m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:date",      m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",        m_oID )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"oouserid",    m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CCustomXmlMoveToRangeEnd::CCustomXmlMoveToRangeEnd(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CCustomXmlMoveToRangeEnd::~CCustomXmlMoveToRangeEnd()
		{
		}
		void CCustomXmlMoveToRangeEnd::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:id", m_oId );
		}
		void CCustomXmlMoveToRangeEnd::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CCustomXmlMoveToRangeEnd::toXML() const
		{
			std::wstring sResult = L"<w:customXmlMoveToRangeEnd";

			ComplexTypes_WriteAttribute_( L" w:id=\"", m_oId );

			sResult += L"/>";

			return sResult;
		}
		EElementType CCustomXmlMoveToRangeEnd::getType() const
		{
			return et_w_customXmlMoveToRangeEnd;
		}
		void CCustomXmlMoveToRangeEnd::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, L"w:id", m_oId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CCustomXmlMoveToRangeStart::CCustomXmlMoveToRangeStart(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CCustomXmlMoveToRangeStart::~CCustomXmlMoveToRangeStart()
		{
		}
		void CCustomXmlMoveToRangeStart::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:author", m_sAuthor );
			XmlMacroReadAttributeBase( oNode, L"w:date",   m_oDate );
			XmlMacroReadAttributeBase( oNode, L"w:id",     m_oID );
			XmlMacroReadAttributeBase( oNode, L"oouserid", m_sUserId );
		}
		void CCustomXmlMoveToRangeStart::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CCustomXmlMoveToRangeStart::toXML() const
		{
			std::wstring sResult = L"<w:customXmlMoveToRangeStart ";

			if ( m_sAuthor.IsInit() )
			{
				sResult += L"w:author=\"";
				sResult += XmlUtils::EncodeXmlString(m_sAuthor.get2());
				sResult += L"\" ";
			}

			if ( m_oDate.IsInit() )
			{
				sResult += L"w:date=\"";
				sResult += m_oDate->ToString();
				sResult += L"\" ";
			}

			if ( m_oID.IsInit() )
			{
				sResult += L"w:id=\"";
				sResult += m_oID->ToString();
				sResult += L"\" ";
			}

			if ( m_sUserId.IsInit() )
			{
				sResult += L"oouserid=\"";
				sResult += m_sUserId.get2();
				sResult += L"\" ";
			}

			sResult += L"/>";

			return sResult;
		}
		EElementType CCustomXmlMoveToRangeStart::getType() const
		{
			return et_w_customXmlMoveToRangeStart;
		}
		void CCustomXmlMoveToRangeStart::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w:author", m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:date",   m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",     m_oID )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"oouserid", m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CMoveFromRangeEnd::CMoveFromRangeEnd(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CMoveFromRangeEnd::~CMoveFromRangeEnd()
		{
		}
		void CMoveFromRangeEnd::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml );
			XmlMacroReadAttributeBase( oNode, L"w:id",                   m_oId );
		}
		void CMoveFromRangeEnd::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CMoveFromRangeEnd::toXML() const
		{
			std::wstring sResult = L"<w:moveFromRangeEnd";

			ComplexTypes_WriteAttribute_( L" w:displacedbyCustomXml=\"", m_oDisplacedByCustomXml );
			ComplexTypes_WriteAttribute_( L" w:id=\"",                   m_oId );

			sResult += L"/>";

			return sResult;
		}
		EElementType CMoveFromRangeEnd::getType() const
		{
			return et_w_moveFromRangeEnd;
		}
		void CMoveFromRangeEnd::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",                   m_oId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CMoveFromRangeStart::CMoveFromRangeStart(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CMoveFromRangeStart::~CMoveFromRangeStart()
		{
		}
		void CMoveFromRangeStart::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:author",               m_sAuthor );
			XmlMacroReadAttributeBase( oNode, L"w:colFirst",             m_oColFirst );
			XmlMacroReadAttributeBase( oNode, L"w:colLast",              m_oColLast );
			XmlMacroReadAttributeBase( oNode, L"w:date",                 m_oDate );
			XmlMacroReadAttributeBase( oNode, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml );
			XmlMacroReadAttributeBase( oNode, L"w:id",                   m_oId );
			XmlMacroReadAttributeBase( oNode, L"w:name",                 m_sName );
			XmlMacroReadAttributeBase( oNode, L"oouserid",               m_sUserId );
		}
		void CMoveFromRangeStart::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CMoveFromRangeStart::toXML() const
		{
			std::wstring sResult = L"<w:moveFromRangeStart";

			if ( m_sAuthor.IsInit() )
			{
				sResult += L" w:author=\"";
				sResult += XmlUtils::EncodeXmlString(m_sAuthor.get2());
				sResult += L"\"";
			}

			ComplexTypes_WriteAttribute_( L" w:colFirst=\"",             m_oColFirst );
			ComplexTypes_WriteAttribute_( L" w:colLast=\"",              m_oColLast );
			ComplexTypes_WriteAttribute_( L" w:date=\"",                 m_oDate );
			ComplexTypes_WriteAttribute_( L" w:displacedbyCustomXml=\"", m_oDisplacedByCustomXml );
			ComplexTypes_WriteAttribute_( L" w:id=\"",                   m_oId );

			if ( m_sName.IsInit() )
			{
				sResult += L" w:name=\"";
				sResult += XmlUtils::EncodeXmlString(m_sName.get2());
				sResult += L"\"";
			}

			if ( m_sUserId.IsInit() )
			{
				sResult += L" oouserid=\"";
				sResult += XmlUtils::EncodeXmlString(m_sUserId.get2());
				sResult += L"\"";
			}

			sResult += L"/>";

			return sResult;
		}
		EElementType CMoveFromRangeStart::getType() const
		{
			return et_w_moveFromRangeStart;
		}
		void CMoveFromRangeStart::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w:author",               m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:colFirst",             m_oColFirst )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:colLast",              m_oColLast )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:date",                 m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",                   m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:name",                 m_sName )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"oouserid",              m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CMoveToRangeEnd::CMoveToRangeEnd(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CMoveToRangeEnd::~CMoveToRangeEnd()
		{
		}
		void CMoveToRangeEnd::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml );
			XmlMacroReadAttributeBase( oNode, L"w:id",                   m_oId );
		}
		void CMoveToRangeEnd::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CMoveToRangeEnd::toXML() const
		{
			std::wstring sResult = L"<w:moveToRangeEnd";

			ComplexTypes_WriteAttribute_( L" w:displacedbyCustomXml=\"", m_oDisplacedByCustomXml );
			ComplexTypes_WriteAttribute_( L" w:id=\"",                   m_oId );

			sResult += L"/>";

			return sResult;
		}
		EElementType CMoveToRangeEnd::getType() const
		{
			return et_w_moveToRangeEnd;
		}
		void CMoveToRangeEnd::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",                   m_oId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CMoveToRangeStart::CMoveToRangeStart(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CMoveToRangeStart::~CMoveToRangeStart()
		{
		}
		void CMoveToRangeStart::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:author",               m_sAuthor );
			XmlMacroReadAttributeBase( oNode, L"w:colFirst",             m_oColFirst );
			XmlMacroReadAttributeBase( oNode, L"w:colLast",              m_oColLast );
			XmlMacroReadAttributeBase( oNode, L"w:date",                 m_oDate );
			XmlMacroReadAttributeBase( oNode, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml );
			XmlMacroReadAttributeBase( oNode, L"w:id",                   m_oId );
			XmlMacroReadAttributeBase( oNode, L"w:name",                 m_sName );
			XmlMacroReadAttributeBase( oNode, L"oouserid",               m_sUserId );
		}
		void CMoveToRangeStart::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CMoveToRangeStart::toXML() const
		{
			std::wstring sResult = L"<w:moveToRangeStart";

			if ( m_sAuthor.IsInit() )
			{
				sResult += L" w:author=\"";
				sResult += XmlUtils::EncodeXmlString(m_sAuthor.get2());
				sResult += L"\" ";
			}

			ComplexTypes_WriteAttribute_( L" w:colFirst=\"",             m_oColFirst );
			ComplexTypes_WriteAttribute_( L" w:colLast=\"",              m_oColLast );
			ComplexTypes_WriteAttribute_( L" w:date=\"",                 m_oDate );
			ComplexTypes_WriteAttribute_( L" w:displacedbyCustomXml=\"", m_oDisplacedByCustomXml );
			ComplexTypes_WriteAttribute_( L" w:id=\"",                   m_oId );

			if ( m_sName.IsInit() )
			{
				sResult += L" w:name=\"";
				sResult += XmlUtils::EncodeXmlString(m_sName.get2());
				sResult += L"\"";
			}

			if ( m_sUserId.IsInit() )
			{
				sResult += L" oouserid=\"";
				sResult += XmlUtils::EncodeXmlString(m_sUserId.get2());
				sResult += L"\"";
			}

			sResult += L"/>";

			return sResult;
		}
		EElementType CMoveToRangeStart::getType() const
		{
			return et_w_moveToRangeStart;
		}
		void CMoveToRangeStart::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w:author",               m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:colFirst",             m_oColFirst )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:colLast",              m_oColLast )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:date",                 m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",                   m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:name",                 m_sName )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"oouserid",				m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CMoveFrom::CMoveFrom(OOX::Document *pMain) : WritingElementWithChilds<>(pMain)
		{
		}
		CMoveFrom::CMoveFrom(XmlUtils::CXmlNode &oNode)
		{
			fromXML( oNode );
		}
		CMoveFrom::CMoveFrom(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( oReader );
		}
		CMoveFrom::~CMoveFrom()
		{
		}
		void CMoveFrom::fromXML(XmlUtils::CXmlNode& oNode)
		{
			//todo
		}
		void CMoveFrom::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( _T("w:bdo") == sName )
					pItem = new CBdo( oReader );
				else if ( _T("w:bookmarkEnd") == sName )
					pItem = new CBookmarkEnd( oReader );
				else if ( _T("w:bookmarkStart") == sName )
					pItem = new CBookmarkStart( oReader );
				else if ( _T("w:commentRangeEnd") == sName )
					pItem = new CCommentRangeEnd( oReader );
				else if ( _T("w:commentRangeStart") == sName )
					pItem = new CCommentRangeStart( oReader );
				//else if ( _T("w:customXml") == sName )
				//	pItem = new CCustomXml( oReader );
				else if ( _T("w:customXmlDelRangeEnd") == sName )
					pItem = new CCustomXmlDelRangeEnd( oReader );
				else if ( _T("w:customXmlDelRangeStart") == sName )
					pItem = new CCustomXmlDelRangeStart( oReader );
				else if ( _T("w:customXmlInsRangeEnd") == sName )
					pItem = new CCustomXmlInsRangeEnd( oReader );
				else if ( _T("w:customXmlInsRangeStart") == sName )
					pItem = new CCustomXmlInsRangeStart( oReader );
				else if ( _T("w:customXmlMoveFromRangeEnd") == sName )
					pItem = new CCustomXmlMoveFromRangeEnd( oReader );
				else if ( _T("w:customXmlMoveFromRangeStart") == sName )
					pItem = new CCustomXmlMoveFromRangeStart( oReader );
				else if ( _T("w:customXmlMoveToRangeEnd") == sName )
					pItem = new CCustomXmlMoveToRangeEnd( oReader );
				else if ( _T("w:customXmlMoveToRangeStart") == sName )
					pItem = new CCustomXmlMoveToRangeStart( oReader );
				else if ( _T("w:del") == sName )
					pItem = new CDel( oReader );
				else if ( _T("w:dir") == sName )
					pItem = new CDir( oReader );
				else if ( _T("w:fldSimple") == sName )
					pItem = new CFldSimple( oReader );
				else if ( _T("w:hyperlink") == sName )
					pItem = new CHyperlink( oReader );
				else if ( _T("w:ins") == sName )
					pItem = new CIns( oReader );
				else if ( _T("w:moveFrom") == sName )
					pItem = new CMoveFrom( oReader );
				else if ( _T("w:moveFromRangeEnd") == sName )
					pItem = new CMoveFromRangeEnd( oReader );
				else if ( _T("w:moveFromRangeStart") == sName )
					pItem = new CMoveFromRangeStart( oReader );
				else if ( _T("w:moveTo") == sName )
					pItem = new CMoveTo( oReader );
				else if ( _T("w:moveToRangeEnd") == sName )
					pItem = new CMoveToRangeEnd( oReader );
				else if ( _T("w:moveToRangeStart") == sName )
					pItem = new CMoveToRangeStart( oReader );
				else if ( _T("m:oMath") == sName )
					pItem = new COMath( oReader );
				else if ( _T("m:oMathPara") == sName )
					pItem = new COMathPara( oReader );
				else if ( _T("w:permEnd") == sName )
					pItem = new CPermEnd( oReader );
				else if ( _T("w:permStart") == sName )
					pItem = new CPermStart( oReader );
				else if ( _T("w:proofErr") == sName )
					pItem = new CProofErr( oReader );
				else if ( _T("w:r") == sName )
					pItem = new CRun( oReader );
				else if ( _T("w:sdt") == sName )
					pItem = new CSdt( oReader );
				else if ( _T("w:smartTag") == sName )
					pItem = new CSmartTag( oReader );
				//else if ( _T("w:subDoc") == sName )
				//	pItem = new CSubDoc( oReader );

				if ( pItem )
					m_arrItems.push_back( pItem );
			}
		}
		std::wstring CMoveFrom::toXML() const
		{
			//todo
			std::wstring sResult = _T("");
			return sResult;
		}
		EElementType CMoveFrom::getType() const
		{
			return et_w_moveFrom;
		}
		void CMoveFrom::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if( oReader, L"w:author", m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:date", m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id", m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"oouserid", m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CMoveTo::CMoveTo(OOX::Document *pMain) : WritingElementWithChilds<>(pMain)
		{
		}
		CMoveTo::CMoveTo(XmlUtils::CXmlNode &oNode)
		{
			fromXML( oNode );
		}
		CMoveTo::CMoveTo(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( oReader );
		}
		CMoveTo::~CMoveTo()
		{
		}
		void CMoveTo::fromXML(XmlUtils::CXmlNode& oNode)
		{
			//todo
		}
		void CMoveTo::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( _T("w:bdo") == sName )
					pItem = new CBdo( oReader );
				else if ( _T("w:bookmarkEnd") == sName )
					pItem = new CBookmarkEnd( oReader );
				else if ( _T("w:bookmarkStart") == sName )
					pItem = new CBookmarkStart( oReader );
				else if ( _T("w:commentRangeEnd") == sName )
					pItem = new CCommentRangeEnd( oReader );
				else if ( _T("w:commentRangeStart") == sName )
					pItem = new CCommentRangeStart( oReader );
				//else if ( _T("w:customXml") == sName )
				//	pItem = new CCustomXml( oReader );
				else if ( _T("w:customXmlDelRangeEnd") == sName )
					pItem = new CCustomXmlDelRangeEnd( oReader );
				else if ( _T("w:customXmlDelRangeStart") == sName )
					pItem = new CCustomXmlDelRangeStart( oReader );
				else if ( _T("w:customXmlInsRangeEnd") == sName )
					pItem = new CCustomXmlInsRangeEnd( oReader );
				else if ( _T("w:customXmlInsRangeStart") == sName )
					pItem = new CCustomXmlInsRangeStart( oReader );
				else if ( _T("w:customXmlMoveFromRangeEnd") == sName )
					pItem = new CCustomXmlMoveFromRangeEnd( oReader );
				else if ( _T("w:customXmlMoveFromRangeStart") == sName )
					pItem = new CCustomXmlMoveFromRangeStart( oReader );
				else if ( _T("w:customXmlMoveToRangeEnd") == sName )
					pItem = new CCustomXmlMoveToRangeEnd( oReader );
				else if ( _T("w:customXmlMoveToRangeStart") == sName )
					pItem = new CCustomXmlMoveToRangeStart( oReader );
				else if ( _T("w:del") == sName )
					pItem = new CDel( oReader );
				else if ( _T("w:dir") == sName )
					pItem = new CDir( oReader );
				else if ( _T("w:fldSimple") == sName )
					pItem = new CFldSimple( oReader );
				else if ( _T("w:hyperlink") == sName )
					pItem = new CHyperlink( oReader );
				else if ( _T("w:ins") == sName )
					pItem = new CIns( oReader );
				else if ( _T("w:moveFrom") == sName )
					pItem = new CMoveFrom( oReader );
				else if ( _T("w:moveFromRangeEnd") == sName )
					pItem = new CMoveFromRangeEnd( oReader );
				else if ( _T("w:moveFromRangeStart") == sName )
					pItem = new CMoveFromRangeStart( oReader );
				else if ( _T("w:moveTo") == sName )
					pItem = new CMoveTo( oReader );
				else if ( _T("w:moveToRangeEnd") == sName )
					pItem = new CMoveToRangeEnd( oReader );
				else if ( _T("w:moveToRangeStart") == sName )
					pItem = new CMoveToRangeStart( oReader );
				else if ( _T("m:oMath") == sName )
					pItem = new COMath( oReader );
				else if ( _T("m:oMathPara") == sName )
					pItem = new COMathPara( oReader );
				else if ( _T("w:permEnd") == sName )
					pItem = new CPermEnd( oReader );
				else if ( _T("w:permStart") == sName )
					pItem = new CPermStart( oReader );
				else if ( _T("w:proofErr") == sName )
					pItem = new CProofErr( oReader );
				else if ( _T("w:r") == sName )
					pItem = new CRun( oReader );
				else if ( _T("w:sdt") == sName )
					pItem = new CSdt( oReader );
				else if ( _T("w:smartTag") == sName )
					pItem = new CSmartTag( oReader );
				//else if ( _T("w:subDoc") == sName )
				//	pItem = new CSubDoc( oReader );

				if ( pItem )
					m_arrItems.push_back( pItem );
			}
		}
		std::wstring CMoveTo::toXML() const
		{
			//todo
			std::wstring sResult = _T("");
			return sResult;
		}
		EElementType CMoveTo::getType() const
		{
			return et_w_moveTo;
		}
		void CMoveTo::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if( oReader, L"w:author", m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:date", m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id", m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"oouserid", m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CBookmarkEnd::CBookmarkEnd(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CBookmarkEnd::CBookmarkEnd(CBookmarkEnd* pOther)
		{
			m_oDisplacedByCustomXml = pOther->m_oDisplacedByCustomXml;
			m_oId                   = pOther->m_oId;
		}
		CBookmarkEnd::~CBookmarkEnd()
		{
		}
		void CBookmarkEnd::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase(oNode, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml );
			XmlMacroReadAttributeBase(oNode, L"w:id",                   m_oId );
		}
		void CBookmarkEnd::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CBookmarkEnd::toXML() const
		{
			std::wstring sResult = L"<w:bookmarkEnd";

			ComplexTypes_WriteAttribute_( L" w:displacedbyCustomXml=\"", m_oDisplacedByCustomXml );
			ComplexTypes_WriteAttribute_( L" w:id=\"",                   m_oId );

			sResult += L"/>";

			return sResult;
		}
		EElementType CBookmarkEnd::getType() const
		{
			return et_w_bookmarkEnd;
		}
		void CBookmarkEnd::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",                   m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"aml:id",					m_oId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CBookmarkStart::CBookmarkStart(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CBookmarkStart::CBookmarkStart(CBookmarkStart *pOther)
		{
			m_oColFirst             = pOther->m_oColFirst;
			m_oColLast              = pOther->m_oColLast;
			m_oDisplacedByCustomXml = pOther->m_oDisplacedByCustomXml;
			m_oId                   = pOther->m_oId;
			m_sName                 = pOther->m_sName;
		}
		CBookmarkStart::~CBookmarkStart()
		{
		}
		void CBookmarkStart::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase(oNode, L"w:colFirst",             m_oColFirst );
			XmlMacroReadAttributeBase(oNode, L"w:colLast",              m_oColLast );
			XmlMacroReadAttributeBase(oNode, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml );
			XmlMacroReadAttributeBase(oNode, L"w:id",                   m_oId );
			XmlMacroReadAttributeBase(oNode, L"w:name",                 m_sName );
		}
		void CBookmarkStart::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CBookmarkStart::toXML() const
		{
			std::wstring sResult = L"<w:bookmarkStart";

			ComplexTypes_WriteAttribute_( L" w:colFirst=\"",             m_oColFirst );
			ComplexTypes_WriteAttribute_( L" w:colLast=\"",              m_oColLast );
			ComplexTypes_WriteAttribute_( L" w:displacedbyCustomXml=\"", m_oDisplacedByCustomXml );
			ComplexTypes_WriteAttribute_( L" w:id=\"",                   m_oId );

			if ( m_sName.IsInit() )
			{
				sResult += L" w:name=\"" + XmlUtils::EncodeXmlString(m_sName.get2()) + L"\"";
			}

			sResult += L"/>";

			return sResult;
		}
		EElementType CBookmarkStart::getType() const
		{
			return et_w_bookmarkStart;
		}
		void CBookmarkStart::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w:colFirst",             m_oColFirst )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:colLast",              m_oColLast )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",                   m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"aml:id",					m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:name",                 m_sName )
			WritingElement_ReadAttributes_End( oReader )
		}

		CPermEnd::CPermEnd(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CPermEnd::~CPermEnd()
		{
		}
		void CPermEnd::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase(oNode, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml );
			XmlMacroReadAttributeBase(oNode, L"w:id",                   m_sId );
		}
		void CPermEnd::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CPermEnd::toXML() const
		{
			std::wstring sResult = L"<w:permEnd";

			ComplexTypes_WriteAttribute_( L" w:displacedbyCustomXml=\"", m_oDisplacedByCustomXml );

			if ( m_sId.IsInit() )
			{
				sResult += L" w:id=\"";
				sResult += m_sId.get2();
				sResult += L"\"";
			}

			sResult += L"/>";

			return sResult;
		}
		EElementType CPermEnd::getType() const
		{
			return et_w_permEnd;
		}
		void CPermEnd::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",                   m_sId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CPermStart::CPermStart(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CPermStart::~CPermStart()
		{
		}
		void CPermStart::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase(oNode, L"w:colFirst",             m_oColFirst );
			XmlMacroReadAttributeBase(oNode, L"w:colLast",              m_oColLast );
			XmlMacroReadAttributeBase(oNode, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml );
			XmlMacroReadAttributeBase(oNode, L"w:ed",                   m_sEd );
			XmlMacroReadAttributeBase(oNode, L"w:edGrp",                m_oEdGrp );
			XmlMacroReadAttributeBase(oNode, L"w:id",                   m_sId );
		}
		void CPermStart::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CPermStart::toXML() const
		{
			std::wstring sResult = L"<w:permStart";

			ComplexTypes_WriteAttribute_( L" w:colFirst=\"", m_oColFirst );
			ComplexTypes_WriteAttribute_( L" w:colLast=\"", m_oColLast );
			ComplexTypes_WriteAttribute_( L" w:displacedbyCustomXml=\"", m_oDisplacedByCustomXml );
			ComplexTypes_WriteAttribute2_(L" w:ed=\"", m_sEd);
			ComplexTypes_WriteAttribute_( L" w:edGrp=\"", m_oEdGrp );
			ComplexTypes_WriteAttribute2_(L" w:id=\"", m_sId);

			sResult += L"/>";

			return sResult;
		}
		EElementType CPermStart::getType() const
		{
			return et_w_permStart;
		}
		void CPermStart::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w:colFirst",             m_oColFirst )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:colLast",              m_oColLast )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:displacedbyCustomXml", m_oDisplacedByCustomXml )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:ed",                   m_sEd )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:edGrp",                m_oEdGrp )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",                   m_sId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CProofErr::CProofErr(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CProofErr::~CProofErr()
		{
		}
		void CProofErr::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase(oNode, L"w:type", m_oType );
		}
		void CProofErr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CProofErr::toXML() const
		{
			std::wstring sResult = L"<w:proofErr ";

			if ( m_oType.IsInit() )
			{
				sResult += L"w:type=\"";
				sResult += m_oType->ToString();
				sResult += L"\" ";
			}

			sResult += L"/>";

			return sResult;
		}
		EElementType CProofErr::getType() const
		{
			return et_w_proofErr;
		}
		void CProofErr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, L"w:type", m_oType )
			WritingElement_ReadAttributes_End( oReader )
		}

		CIns::CIns(OOX::Document *pMain) : WritingElementWithChilds<>(pMain)
		{
		}
		CIns::CIns(XmlUtils::CXmlNode &oNode)
		{
			fromXML( oNode );
		}
		CIns::CIns(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( oReader );
		}
		CIns::~CIns()
		{
		}
		void CIns::fromXML(XmlUtils::CXmlNode& oNode)
		{
			//todo
		}
		void CIns::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( _T("w:bdo") == sName )
					pItem = new CBdo( oReader );
				else if ( _T("w:bookmarkEnd") == sName )
					pItem = new CBookmarkEnd( oReader );
				else if ( _T("w:bookmarkStart") == sName )
					pItem = new CBookmarkStart( oReader );
				else if ( _T("w:commentRangeEnd") == sName )
					pItem = new CCommentRangeEnd( oReader );
				else if ( _T("w:commentRangeStart") == sName )
					pItem = new CCommentRangeStart( oReader );
				//else if ( _T("w:customXml") == sName )
				//	pItem = new CCustomXml( oReader );
				else if ( _T("w:customXmlDelRangeEnd") == sName )
					pItem = new CCustomXmlDelRangeEnd( oReader );
				else if ( _T("w:customXmlDelRangeStart") == sName )
					pItem = new CCustomXmlDelRangeStart( oReader );
				else if ( _T("w:customXmlInsRangeEnd") == sName )
					pItem = new CCustomXmlInsRangeEnd( oReader );
				else if ( _T("w:customXmlInsRangeStart") == sName )
					pItem = new CCustomXmlInsRangeStart( oReader );
				else if ( _T("w:customXmlMoveFromRangeEnd") == sName ) 
					pItem = new CCustomXmlMoveFromRangeEnd( oReader );
				else if ( _T("w:customXmlMoveFromRangeStart") == sName )
					pItem = new CCustomXmlMoveFromRangeStart( oReader );
				else if ( _T("w:customXmlMoveToRangeEnd") == sName ) 
					pItem = new CCustomXmlMoveToRangeEnd( oReader );
				else if ( _T("w:customXmlMoveToRangeStart") == sName )
					pItem = new CCustomXmlMoveToRangeStart( oReader );
				else if ( _T("w:del") == sName )
					pItem = new CDel( oReader );
				else if ( _T("w:dir") == sName )
					pItem = new CDir( oReader );
				else if ( _T("w:fldSimple") == sName )
					pItem = new CFldSimple( oReader );
				else if ( _T("w:hyperlink") == sName )
					pItem = new CHyperlink( oReader );
				else if ( _T("w:ins") == sName )
					pItem = new CIns( oReader );
				else if ( _T("w:moveFrom") == sName )
					pItem = new CMoveFrom( oReader );
				else if ( _T("w:moveFromRangeEnd") == sName )
					pItem = new CMoveFromRangeEnd( oReader );
				else if ( _T("w:moveFromRangeStart") == sName )
					pItem = new CMoveFromRangeStart( oReader );
				else if ( _T("w:moveTo") == sName )
					pItem = new CMoveTo( oReader );
				else if ( _T("w:moveToRangeEnd") == sName )
					pItem = new CMoveToRangeEnd( oReader );
				else if ( _T("w:moveToRangeStart") == sName )
					pItem = new CMoveToRangeStart( oReader );
				else if ( _T("m:oMath") == sName )
					pItem = new COMath( oReader );
				else if ( _T("m:oMathPara") == sName )
					pItem = new COMathPara( oReader );
				else if ( _T("m:r") == sName )
					pItem = new CMRun( oReader );
				else if ( _T("w:permEnd") == sName )
					pItem = new CPermEnd( oReader );
				else if ( _T("w:permStart") == sName )
					pItem = new CPermStart( oReader );
				else if ( _T("w:proofErr") == sName )
					pItem = new CProofErr( oReader );
				else if ( _T("w:r") == sName )
					pItem = new CRun( oReader );
				else if ( _T("w:sdt") == sName )
					pItem = new CSdt( oReader );
				else if ( _T("w:smartTag") == sName )
					pItem = new CSmartTag( oReader );
				//else if ( _T("w:subDoc") == sName )
				//	pItem = new CSubDoc( oReader );

				if ( pItem )
					m_arrItems.push_back( pItem );
			}
		}
		std::wstring CIns::toXML() const
		{
			//todo
			std::wstring sResult = _T("");
			return sResult;
		}
		EElementType CIns::getType() const
		{
			return et_w_ins;
		}
		void CIns::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if( oReader, L"w:author", m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:date", m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id", m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"oouserid", m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CDel::CDel(OOX::Document *pMain) : WritingElementWithChilds<>(pMain)
		{
		}
		CDel::CDel(XmlUtils::CXmlNode &oNode)
		{
			fromXML( oNode );
		}
		CDel::CDel(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( oReader );
		}
		CDel::~CDel()
		{
		}
		void CDel::fromXML(XmlUtils::CXmlNode& oNode)
		{
			//todo
		}
		void CDel::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( _T("w:bdo") == sName )
					pItem = new CBdo( oReader );
				else if ( _T("w:bookmarkEnd") == sName )
					pItem = new CBookmarkEnd( oReader );
				else if ( _T("w:bookmarkStart") == sName )
					pItem = new CBookmarkStart( oReader );
				else if ( _T("w:commentRangeEnd") == sName )
					pItem = new CCommentRangeEnd( oReader );
				else if ( _T("w:commentRangeStart") == sName )
					pItem = new CCommentRangeStart( oReader );
				//else if ( _T("w:customXml") == sName )
				//	pItem = new CCustomXml( oReader );
				else if ( _T("w:customXmlDelRangeEnd") == sName )
					pItem = new CCustomXmlDelRangeEnd( oReader );
				else if ( _T("w:customXmlDelRangeStart") == sName )
					pItem = new CCustomXmlDelRangeStart( oReader );
				else if ( _T("w:customXmlInsRangeEnd") == sName )
					pItem = new CCustomXmlInsRangeEnd( oReader );
				else if ( _T("w:customXmlInsRangeStart") == sName )
					pItem = new CCustomXmlInsRangeStart( oReader );
				else if ( _T("w:customXmlMoveFromRangeEnd") == sName ) 
					pItem = new CCustomXmlMoveFromRangeEnd( oReader );
				else if ( _T("w:customXmlMoveFromRangeStart") == sName )
					pItem = new CCustomXmlMoveFromRangeStart( oReader );
				else if ( _T("w:customXmlMoveToRangeEnd") == sName ) 
					pItem = new CCustomXmlMoveToRangeEnd( oReader );
				else if ( _T("w:customXmlMoveToRangeStart") == sName )
					pItem = new CCustomXmlMoveToRangeStart( oReader );
				else if ( _T("w:del") == sName )
					pItem = new CDel( oReader );
				else if ( _T("w:dir") == sName )
					pItem = new CDir( oReader );
				else if ( _T("w:fldSimple") == sName )
					pItem = new CFldSimple( oReader );
				else if ( _T("w:hyperlink") == sName )
					pItem = new CHyperlink( oReader );
				else if ( _T("w:ins") == sName )
					pItem = new CIns( oReader );
				else if ( _T("w:moveFrom") == sName )
					pItem = new CMoveFrom( oReader );
				else if ( _T("w:moveFromRangeEnd") == sName )
					pItem = new CMoveFromRangeEnd( oReader );
				else if ( _T("w:moveFromRangeStart") == sName )
					pItem = new CMoveFromRangeStart( oReader );
				else if ( _T("w:moveTo") == sName )
					pItem = new CMoveTo( oReader );
				else if ( _T("w:moveToRangeEnd") == sName )
					pItem = new CMoveToRangeEnd( oReader );
				else if ( _T("w:moveToRangeStart") == sName )
					pItem = new CMoveToRangeStart( oReader );
				else if ( _T("m:oMath") == sName )
					pItem = new COMath( oReader );
				else if ( _T("m:oMathPara") == sName )
					pItem = new COMathPara( oReader );
				else if ( _T("m:r") == sName )
					pItem = new CMRun( oReader );
				else if ( _T("w:permEnd") == sName )
					pItem = new CPermEnd( oReader );
				else if ( _T("w:permStart") == sName )
					pItem = new CPermStart( oReader );
				else if ( _T("w:proofErr") == sName )
					pItem = new CProofErr( oReader );
				else if ( _T("w:r") == sName )
					pItem = new CRun( oReader );
				else if ( _T("w:sdt") == sName )
					pItem = new CSdt( oReader );
				else if ( _T("w:smartTag") == sName )
					pItem = new CSmartTag( oReader );
				//else if ( _T("w:subDoc") == sName )
				//	pItem = new CSubDoc( oReader );

				if ( pItem )
					m_arrItems.push_back( pItem );
			}
		}
		std::wstring CDel::toXML() const
		{
			//todo
			std::wstring sResult = _T("");
			return sResult;
		}
		EElementType CDel::getType() const
		{
			return et_w_del;
		}
		void CDel::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if( oReader, L"w:author", m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:date", m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id", m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"oouserid", m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}

	} // namespace Logic
} // namespace OOX
