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

#include "RunContent.h"

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CBr 17.3.3.1 (Part 1)
		//--------------------------------------------------------------------------------

		CBr::CBr(OOX::Document *pMain) : WritingElement(pMain) {}
		CBr::~CBr() {}
		void CBr::fromXML(XmlUtils::CXmlNode &oNode)
		{
			m_oClear.SetValue(SimpleTypes::brclearNone);
			m_oType.SetValue(SimpleTypes::brtypeTextWrapping);

			XmlMacroReadAttributeBase( oNode, _T("w:type"),  m_oType  );
			XmlMacroReadAttributeBase( oNode, _T("w:clear"), m_oClear );
		}
		void CBr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CBr::toXML() const
		{
			std::wstring sResult = _T("<w:br ");

			if ( SimpleTypes::brtypeTextWrapping == m_oType.GetValue() )
			{
				sResult += _T("w:clear=\"");
				sResult += m_oClear.ToString();
				sResult += _T("\" ");
			}

			sResult += _T("w:type=\"");
			sResult += m_oType.ToString();
			sResult += _T("\" />");

			return sResult;
		}
		EElementType CBr::getType() const
		{
			return et_w_br;
		}
		void CBr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				m_oClear.SetValue(SimpleTypes::brclearNone);
				m_oType.SetValue(SimpleTypes::brtypeTextWrapping);

				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					if ( _T("w:type") == wsName )
						m_oType = oReader.GetText();
					else if ( _T("w:clear") == wsName )
						m_oClear = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}

				oReader.MoveToElement();
			}

		//--------------------------------------------------------------------------------
		// CContentPart 17.3.3.2 (Part 1)
		//--------------------------------------------------------------------------------

		CContentPart::CContentPart(OOX::Document *pMain) : WritingElement(pMain) {}
		CContentPart::~CContentPart() {}
		void CContentPart::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"r:id", m_oId );
			if (false == m_oId.IsInit())
			{
				XmlMacroReadAttributeBase( oNode, L"relationships:id", m_oId );
			}
		}
		void CContentPart::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"xfrm" == sName)
					m_oXfrm = oReader;
				//else if (L"nvContentPartPr" == sName)
				//	m_oNvContentPartPr = oReader;
			}
		}
		std::wstring CContentPart::toXML() const
		{
			std::wstring sResult = L"<w:contentPart ";

			if ( m_oId.IsInit() )
			{
				sResult += L"r:id=\"";
				sResult += m_oId->ToString();
				sResult += L"\" ";
			}

			sResult += L" />";

			return sResult;
		}
		EElementType CContentPart::getType() const
		{
			return et_w_contentPart;
		}
		void CContentPart::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start_No_NS( oReader )
					WritingElement_ReadAttributes_Read_if( oReader, L"id", m_oId)
				WritingElement_ReadAttributes_End_No_NS( oReader )
			}

		//--------------------------------------------------------------------------------
		// CCr 17.3.3.4 (Part 1)
		//--------------------------------------------------------------------------------

		CCr::CCr(OOX::Document *pMain) : WritingElement(pMain) {}
		CCr::~CCr() {}
		void CCr::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CCr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CCr::toXML() const
		{
			return _T("<w:cr />");
		}
		EElementType CCr::getType() const
			{
				return et_w_cr;
			}

		//--------------------------------------------------------------------------------
		// CDayLong 17.3.3.5 (Part 1)
		//--------------------------------------------------------------------------------

		CDayLong::CDayLong(OOX::Document *pMain) : WritingElement(pMain) {}
		CDayLong::~CDayLong() {}
		void CDayLong::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CDayLong::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CDayLong::toXML() const
		{
			return _T("<w:dayLong />");
		}
		EElementType CDayLong::getType() const
			{
				return et_w_dayLong;
			}

		//--------------------------------------------------------------------------------
		// CDayShort 17.3.3.6 (Part 1)
		//--------------------------------------------------------------------------------

		CDayShort::CDayShort(OOX::Document *pMain) : WritingElement(pMain) {}
		CDayShort::~CDayShort() {}
		void CDayShort::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CDayShort::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CDayShort::toXML() const
		{
			return _T("<w:dayShort />");
		}
		EElementType CDayShort::getType() const
			{
				return et_w_dayShort;
			}

		//--------------------------------------------------------------------------------
		// CDelText 17.3.3.7 (Part 1)
		//--------------------------------------------------------------------------------

		CDelText::CDelText(OOX::Document *pMain) : WritingElement(pMain) {}
		CDelText::~CDelText() {}
		void CDelText::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("xml:space"), m_oSpace );

			m_sText = oNode.GetText();
		}
		void CDelText::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			m_sText = oReader.GetText2();
		}
		std::wstring  CDelText::toXML() const
		{
			std::wstring sResult;

			if ( m_oSpace.IsInit() )
				sResult = _T("<w:delText xml:space=\"") + m_oSpace->ToString() + _T("\">");
			else
				sResult = _T("<w:delText>");

			sResult += m_sText;
			sResult += _T("</w:delText>");

			return sResult;
		}
		EElementType CDelText::getType() const
		{
			return et_w_delText;
		}
		void CDelText::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					if ( _T("xml:space") == wsName )
					{
						m_oSpace = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}

				oReader.MoveToElement();
			}

		//--------------------------------------------------------------------------------
		// CLastRenderedPageBreak 17.3.3.13 (Part 1)
		//--------------------------------------------------------------------------------

		CLastRenderedPageBreak::CLastRenderedPageBreak(OOX::Document *pMain) : WritingElement(pMain) {}
		CLastRenderedPageBreak::~CLastRenderedPageBreak() {}
		void CLastRenderedPageBreak::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CLastRenderedPageBreak::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CLastRenderedPageBreak::toXML() const
		{
			return _T("<w:lastRenderedPageBreak />");
		}
		EElementType CLastRenderedPageBreak::getType() const
		{
			return et_w_lastRenderedPageBreak;
		}

		//--------------------------------------------------------------------------------
		// CMonthLong 17.3.3.15 (Part 1)
		//--------------------------------------------------------------------------------

		CMonthLong::CMonthLong(OOX::Document *pMain) : WritingElement(pMain) {}
		CMonthLong::~CMonthLong() {}
		void CMonthLong::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CMonthLong::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CMonthLong::toXML() const
		{
			return _T("<w:monthLong />");
		}
		EElementType CMonthLong::getType() const
		{
			return et_w_monthLong;
		}

		//--------------------------------------------------------------------------------
		// CMonthShort 17.3.3.16 (Part 1)
		//--------------------------------------------------------------------------------

		CMonthShort::CMonthShort(OOX::Document *pMain) : WritingElement(pMain) {}
		CMonthShort::~CMonthShort() {}
		void CMonthShort::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CMonthShort::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CMonthShort::toXML() const
		{
			return _T("<w:monthShort />");
		}
		EElementType CMonthShort::getType() const
		{
			return et_w_monthShort;
		}

		//--------------------------------------------------------------------------------
		// CNoBreakHyphen 17.3.3.18 (Part 1)
		//--------------------------------------------------------------------------------

		CNoBreakHyphen::CNoBreakHyphen(OOX::Document *pMain) : WritingElement(pMain) {}
		CNoBreakHyphen::~CNoBreakHyphen() {}
		void CNoBreakHyphen::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CNoBreakHyphen::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CNoBreakHyphen::toXML() const
		{
			return _T("<w:noBreakHyphen />");
		}
		EElementType CNoBreakHyphen::getType() const
		{
			return et_w_nonBreakHyphen;
		}

		//--------------------------------------------------------------------------------
		// CPgNum 17.3.3.22 (Part 1)
		//--------------------------------------------------------------------------------

		CPgNum::CPgNum(OOX::Document *pMain) : WritingElement(pMain) {}
		CPgNum::~CPgNum() {}
		void CPgNum::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CPgNum::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CPgNum::toXML() const
		{
			return _T("<w:pgNum />");
		}
		EElementType CPgNum::getType() const
		{
			return et_w_pgNum;
		}

		//--------------------------------------------------------------------------------
		// CPTab 17.3.3.23 (Part 1)
		//--------------------------------------------------------------------------------

		CPTab::CPTab(OOX::Document *pMain) : WritingElement(pMain) {}
		CPTab::~CPTab() {}
		void CPTab::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("w:alignment"),  m_oAlignment );
			XmlMacroReadAttributeBase( oNode, _T("w:leader"),     m_oLeader );
			XmlMacroReadAttributeBase( oNode, _T("w:relativeTo"), m_oRelativeTo );
		}
		void CPTab::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CPTab::toXML() const
		{
			std::wstring sResult = _T("<w:ptab ");

			ComplexTypes_WriteAttribute( _T("w:alignment=\""),  m_oAlignment );
			ComplexTypes_WriteAttribute( _T("w:leader=\""),     m_oLeader );
			ComplexTypes_WriteAttribute( _T("w:relativeTo=\""), m_oRelativeTo );

			sResult += _T("/>");
			return sResult;
		}
		EElementType CPTab::getType() const
		{
			return et_w_ptab;
		}
		void CPTab::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					if ( _T("w:alignment") == wsName )
						m_oAlignment = oReader.GetText();
					else if ( _T("w:leader") == wsName )
						m_oLeader = oReader.GetText();
					else if ( _T("w:relativeTo") == wsName )
						m_oRelativeTo = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}

				oReader.MoveToElement();
			}

		//--------------------------------------------------------------------------------
		// CRuby 17.3.3.25 (Part 1)
		//--------------------------------------------------------------------------------

		CRuby::CRuby(OOX::Document *pMain) : WritingElement(pMain) {}
		CRuby::~CRuby() {}
		void CRuby::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CRuby::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CRuby::toXML() const
		{
			return _T("<w:ruby />");
		}
		EElementType CRuby::getType() const
		{
			return et_w_ruby;
		}

		//--------------------------------------------------------------------------------
		// CSoftHyphen 17.3.3.29 (Part 1)
		//--------------------------------------------------------------------------------

		CSoftHyphen::CSoftHyphen(OOX::Document *pMain) : WritingElement(pMain) {}
		CSoftHyphen::~CSoftHyphen() {}
		void CSoftHyphen::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CSoftHyphen::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CSoftHyphen::toXML() const
		{
			return _T("<w:softHyphen />");
		}
		EElementType CSoftHyphen::getType() const
		{
			return et_w_softHyphen;
		}

		//--------------------------------------------------------------------------------
		// CSym 17.3.3.30 (Part 1)
		//--------------------------------------------------------------------------------

		CSym::CSym(OOX::Document *pMain) : WritingElement(pMain) {}
		CSym::~CSym() {}
		void CSym::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("w:char"), m_oChar );
			XmlMacroReadAttributeBase( oNode, _T("w:font"), m_oFont );
		}
		void CSym::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CSym::toXML() const
		{
			std::wstring sResult = _T("<w:sym ");

			ComplexTypes_WriteAttribute( _T("w:char=\""), m_oChar );

			if ( m_oFont.IsInit() )
			{
				sResult += _T("w:font=\"");
				sResult += m_oFont.get2();
				sResult += _T("\" ");
			}

			sResult += _T("/>");
			return sResult;
		}
		EElementType CSym::getType() const
		{
			return et_w_sym;
		}
		void CSym::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					if ( _T("w:char") == wsName )
						m_oChar = oReader.GetText();
					else if ( _T("w:font") == wsName )
						m_oFont = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}

				oReader.MoveToElement();
			}

		//--------------------------------------------------------------------------------
		// CText 17.3.3.31 (Part 1)
		//--------------------------------------------------------------------------------

		CText::CText(OOX::Document *pMain) : WritingElement(pMain) {}
		CText::~CText() {}
		void CText::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("xml:space"), m_oSpace );

			m_sText = oNode.GetText();
		}
		void CText::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			m_sText = oReader.GetText2();
		}
		std::wstring CText::toXML() const
		{
			std::wstring sResult;

			if ( m_oSpace.IsInit() )
				sResult = _T("<w:t xml:space=\"") + m_oSpace->ToString() + _T("\">");
			else
				sResult = _T("<w:t>");

			sResult += XmlUtils::EncodeXmlString(m_sText);
			sResult += _T("</w:t>");

			return sResult;
		}
		EElementType CText::getType() const
		{
			return et_w_t;
		}
		void CText::ReadAttributes(XmlUtils::CXmlLiteReader& oReader, nullable<SimpleTypes::CXmlSpace>& oSpace)
		{
			if ( oReader.GetAttributesCount() <= 0 )
				return;

			if ( !oReader.MoveToFirstAttribute() )
				return;

			std::wstring wsName = oReader.GetName();
			while( !wsName.empty() )
			{
				if ( L"xml:space" == wsName )
				{
					oSpace = oReader.GetText();
					break;
				}

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}

			oReader.MoveToElement();
		}
		void CText::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader, m_oSpace);
			}

		//--------------------------------------------------------------------------------
		// CTab 17.3.3.32 (Part 1)
		//--------------------------------------------------------------------------------

		CTab::CTab(OOX::Document *pMain) : WritingElement(pMain) {}
		CTab::~CTab() {}
		void CTab::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CTab::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CTab::toXML() const
		{
			return _T("<w:tab />");
		}
		EElementType CTab::getType() const
		{
			return et_w_tab;
		}

		//--------------------------------------------------------------------------------
		// CYearLong 17.3.3.33 (Part 1)
		//--------------------------------------------------------------------------------

		CYearLong::CYearLong(OOX::Document *pMain) : WritingElement(pMain) {}
		CYearLong::~CYearLong() {}
		void CYearLong::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CYearLong::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CYearLong::toXML() const
		{
			return _T("<w:yearLong />");
		}
		EElementType CYearLong::getType() const
		{
			return et_w_yearLong;
		}

		//--------------------------------------------------------------------------------
		// CYearShort 17.3.3.32 (Part 1)
		//--------------------------------------------------------------------------------

		CYearShort::CYearShort(OOX::Document *pMain) : WritingElement(pMain) {}
		CYearShort::~CYearShort() {}
		void CYearShort::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CYearShort::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CYearShort::toXML() const
		{
			return _T("<w:yearShort />");
		}
		EElementType CYearShort::getType() const
		{
			return et_w_yearShort;
		}

		//--------------------------------------------------------------------------------
		// CAnnotationRef 17.13.4.1 (Part 1)
		//--------------------------------------------------------------------------------

		CAnnotationRef::CAnnotationRef(OOX::Document *pMain) : WritingElement(pMain) {}
		CAnnotationRef::~CAnnotationRef() {}
		void CAnnotationRef::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CAnnotationRef::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CAnnotationRef::toXML() const
		{
			return _T("<w:annotationRef />");
		}
		EElementType CAnnotationRef::getType() const
		{
			return et_w_annotationRef;
		}

		//--------------------------------------------------------------------------------
		// CCommentReference 17.13.4.5 (Part 1)
		//--------------------------------------------------------------------------------

		CCommentReference::CCommentReference(OOX::Document *pMain) : WritingElement(pMain) {}
		CCommentReference::~CCommentReference() {}
		void CCommentReference::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("w:id"), m_oId );
		}
		void CCommentReference::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CCommentReference::toXML() const
		{
			std::wstring sResult = _T("<w:commentReference ");

			ComplexTypes_WriteAttribute( _T("w:id=\""), m_oId );

			sResult += _T("/>");

			return sResult;
		}
		EElementType CCommentReference::getType() const
		{
			return et_w_commentReference;
		}
		void CCommentReference::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					if ( _T("w:id") == wsName )
					{
						m_oId = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}

				oReader.MoveToElement();
			}

		//--------------------------------------------------------------------------------
		// CContinuationSeparator 17.11.1 (Part 1)
		//--------------------------------------------------------------------------------

		CContinuationSeparator::CContinuationSeparator(OOX::Document *pMain) : WritingElement(pMain) {}
		CContinuationSeparator::~CContinuationSeparator() {}
		void CContinuationSeparator::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CContinuationSeparator::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CContinuationSeparator::toXML() const
		{
			return _T("<w:continuationSeparator />");
		}
		EElementType CContinuationSeparator::getType() const
		{
			return et_w_continuationSeparator;
		}

		//--------------------------------------------------------------------------------
		// CDelInstrText 17.16.13 (Part 1)
		//--------------------------------------------------------------------------------

		CDelInstrText::CDelInstrText(OOX::Document *pMain) : WritingElement(pMain) {}
		CDelInstrText::~CDelInstrText() {}
		void CDelInstrText::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("xml:space"), m_oSpace );

			m_sText = oNode.GetText();
		}
		void CDelInstrText::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			m_sText = oReader.GetText2();
		}
		std::wstring CDelInstrText::toXML() const
		{
			std::wstring sResult;

			if ( m_oSpace.IsInit() )
				sResult = _T("<w:delInstrText xml:space=\"") + m_oSpace->ToString() + _T("\">");
			else
				sResult = _T("<w:delInstrText>");

			sResult += m_sText;
			sResult += _T("</w:delInstrText>");

			return sResult;
		}
		EElementType CDelInstrText::getType() const
		{
			return et_w_delInstrText;
		}
		void CDelInstrText::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					if ( _T("xml:space") == wsName )
					{
						m_oSpace = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}

				oReader.MoveToElement();
			}

		//--------------------------------------------------------------------------------
		// CEndnoteRef 17.11.6 (Part 1)
		//--------------------------------------------------------------------------------

		CEndnoteRef::CEndnoteRef(OOX::Document *pMain) : WritingElement(pMain) {}
		CEndnoteRef::~CEndnoteRef() {}
		void CEndnoteRef::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CEndnoteRef::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CEndnoteRef::toXML() const
		{
			return _T("<w:endnoteRef />");
		}
		EElementType CEndnoteRef::getType() const
		{
			return et_w_endnoteRef;
		}

		//--------------------------------------------------------------------------------
		// CEndnoteReference 17.11.7 (Part 1)
		//--------------------------------------------------------------------------------

		CEndnoteReference::CEndnoteReference(OOX::Document *pMain) : WritingElement(pMain) {}
		CEndnoteReference::~CEndnoteReference() {}
		void CEndnoteReference::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("w:customMarkFollows"), m_oCustomMarkFollows );
			XmlMacroReadAttributeBase( oNode, _T("w:id"),                m_oId );
		}
		void CEndnoteReference::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CEndnoteReference::toXML() const
		{
			std::wstring sResult = _T("<w:endnoteReference ");

			if ( m_oCustomMarkFollows.IsInit() )
			{
				sResult += _T("w:customMarkFollows=\"");
				sResult += m_oCustomMarkFollows->ToString2(SimpleTypes::onofftostring1);
				sResult += _T("\" ");
			}
			ComplexTypes_WriteAttribute( _T("w:id=\""),                m_oId );

			sResult += _T("/>");

			return sResult;
		}
		EElementType CEndnoteReference::getType() const
		{
			return et_w_endnoteReference;
		}
		void CEndnoteReference::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					if ( _T("w:customMarkFollows") == wsName )
						m_oCustomMarkFollows = oReader.GetText();
					else if ( _T("w:id") == wsName )
						m_oId = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}

				oReader.MoveToElement();
			}

		//--------------------------------------------------------------------------------
		// CEndnoteRef 17.11.13 (Part 1)
		//--------------------------------------------------------------------------------

		CFootnoteRef::CFootnoteRef(OOX::Document *pMain) : WritingElement(pMain) {}
		CFootnoteRef::~CFootnoteRef() {}
		void CFootnoteRef::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CFootnoteRef::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CFootnoteRef::toXML() const
		{
			return _T("<w:footnoteRef />");
		}
		EElementType CFootnoteRef::getType() const
		{
			return et_w_footnoteRef;
		}

		//--------------------------------------------------------------------------------
		// CEndnoteReference 17.11.14 (Part 1)
		//--------------------------------------------------------------------------------

		CFootnoteReference::CFootnoteReference(OOX::Document *pMain) : WritingElement(pMain) {}
		CFootnoteReference::~CFootnoteReference() {}
		void CFootnoteReference::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("w:customMarkFollows"), m_oCustomMarkFollows );
			XmlMacroReadAttributeBase( oNode, _T("w:id"),                m_oId );
		}
		void CFootnoteReference::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CFootnoteReference::toXML() const
		{
			std::wstring sResult = _T("<w:footnoteReference ");

			if ( m_oCustomMarkFollows.IsInit() )
			{
				sResult += _T("w:customMarkFollows=\"");
				sResult += m_oCustomMarkFollows->ToString2(SimpleTypes::onofftostring1);
				sResult += _T("\" ");
			}
			ComplexTypes_WriteAttribute( _T("w:id=\""),                m_oId );

			sResult += _T("/>");

			return sResult;
		}
		EElementType CFootnoteReference::getType() const
		{
			return et_w_footnoteReference;
		}
		void CFootnoteReference::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					if ( _T("w:customMarkFollows") == wsName )
						m_oCustomMarkFollows = oReader.GetText();
					else if ( _T("w:id") == wsName )
						m_oId = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}

				oReader.MoveToElement();
			}

		//--------------------------------------------------------------------------------
		// CInstrText 17.16.23 (Part 1)
		//--------------------------------------------------------------------------------

		CInstrText::CInstrText(OOX::Document *pMain) : WritingElement(pMain) {}
		CInstrText::~CInstrText() {}
		void CInstrText::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("xml:space"), m_oSpace );

			m_sText = oNode.GetText();
		}
		void CInstrText::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			m_sText = oReader.GetText2();
		}
		std::wstring CInstrText::toXML() const
		{
			std::wstring sResult;

			if ( m_oSpace.IsInit() )
				sResult = _T("<w:instrText xml:space=\"") + m_oSpace->ToString() + _T("\">");
			else
				sResult = _T("<w:instrText>");

			sResult += m_sText;
			sResult += _T("</w:instrText>");

			return sResult;
		}
		EElementType CInstrText::getType() const
		{
			return et_w_instrText;
		}
		void CInstrText::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					if ( _T("xml:space") == wsName )
					{
						m_oSpace = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}

				oReader.MoveToElement();
			}

		//--------------------------------------------------------------------------------
		// CSeparator 17.11.23 (Part 1)
		//--------------------------------------------------------------------------------

		CSeparator::CSeparator(OOX::Document *pMain) : WritingElement(pMain) {}
		CSeparator::~CSeparator() {}
		void CSeparator::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CSeparator::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CSeparator::toXML() const
		{
			return _T("<w:separator />");
		}

		EElementType CSeparator::getType() const
		{
			return et_w_separator;
		}

	} // Logic
} // OOX
