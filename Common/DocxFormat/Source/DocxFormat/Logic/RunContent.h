/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once

#include "../WritingElement.h"
#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Word.h"
#include "../../Common/ComplexTypes.h"

//        2. Класс CRuby   17.3.3.25

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CBr 17.3.3.1 (Part 1)
		//--------------------------------------------------------------------------------	
		class CBr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBr)
			CBr() {}
			virtual ~CBr() {}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode &oNode)
			{
				oNode.ReadAttributeBase( _T("w:type"),  m_oType  );
				oNode.ReadAttributeBase( _T("w:clear"), m_oClear );
			}

			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}

			virtual CString      toXML() const
			{
				CString sResult = _T("<w:br ");

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

			virtual EElementType getType() const
			{
				return et_w_br;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				CWCharWrapper wsName = oReader.GetName();
				while( !wsName.IsNull() )
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

		public:

			SimpleTypes::CBrClear<SimpleTypes::brclearNone      > m_oClear;
			SimpleTypes::CBrType<SimpleTypes::brtypeTextWrapping> m_oType;

		};


		//--------------------------------------------------------------------------------
		// CContentPart 17.3.3.2 (Part 1)
		//--------------------------------------------------------------------------------	
		class CContentPart : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CContentPart)
			CContentPart() {}
			virtual ~CContentPart() {}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				oNode.ReadAttributeBase( _T("r:id"), m_oId );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}

			virtual CString      toXML() const
			{
				CString sResult = _T("<w:contentPart ");

				if ( m_oId.IsInit() )
				{
					sResult += _T("r:id=\"");
					sResult += m_oId->ToString();
					sResult += _T("\" ");
				}

				sResult += _T(" />");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_contentPart;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				CWCharWrapper wsName = oReader.GetName();
				while( !wsName.IsNull() )
				{
					if ( _T("r:id") == wsName )
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

		public:

			nullable<SimpleTypes::CRelationshipId > m_oId;

		};


		//--------------------------------------------------------------------------------
		// CCr 17.3.3.4 (Part 1)
		//--------------------------------------------------------------------------------	
		class CCr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCr)
			CCr() {}
			virtual ~CCr() {}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				return _T("<w:cr />");
			}

			virtual EElementType getType() const
			{
				return et_w_cr;
			}
		};


		//--------------------------------------------------------------------------------
		// CDayLong 17.3.3.5 (Part 1)
		//--------------------------------------------------------------------------------	
		class CDayLong : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDayLong)
			CDayLong() {}
			virtual ~CDayLong() {}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				return _T("<w:dayLong />");
			}

			virtual EElementType getType() const
			{
				return et_w_dayLong;
			}
		};


		//--------------------------------------------------------------------------------
		// CDayShort 17.3.3.6 (Part 1)
		//--------------------------------------------------------------------------------	
		class CDayShort : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDayShort)
			CDayShort() {}
			virtual ~CDayShort() {}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				return _T("<w:dayShort />");
			}

			virtual EElementType getType() const
			{
				return et_w_dayShort;
			}
		};


		//--------------------------------------------------------------------------------
		// CDelText 17.3.3.7 (Part 1)
		//--------------------------------------------------------------------------------	
		class CDelText : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDelText)
			CDelText() {}
			virtual ~CDelText() {}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				oNode.ReadAttributeBase( _T("xml:space"), m_oSpace );

				m_sText = oNode.GetText();
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				m_sText = oReader.GetText2();
			}
			virtual CString      toXML() const
			{
				CString sResult;
				
				if ( m_oSpace.IsInit() )
					sResult.Format( _T("<w:delText xml:space=\"%ls\">"), m_oSpace->ToString() );
				else
					sResult = _T("<w:delText>");

				sResult += m_sText;
				sResult += _T("</w:delText>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_delText;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				CWCharWrapper wsName = oReader.GetName();
				while( !wsName.IsNull() )
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

		public:

			// Attributes
			nullable<SimpleTypes::CXmlSpace<> > m_oSpace;

			// Value
			CString                             m_sText;

		};


		//--------------------------------------------------------------------------------
		// CLastRenderedPageBreak 17.3.3.13 (Part 1)
		//--------------------------------------------------------------------------------	
		class CLastRenderedPageBreak : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLastRenderedPageBreak)
			CLastRenderedPageBreak() {}
			virtual ~CLastRenderedPageBreak() {}
			
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				return _T("<w:lastRenderedPageBreak />");
			}

			virtual EElementType getType() const
			{
				return et_w_lastRenderedPageBreak;
			}
		};


		//--------------------------------------------------------------------------------
		// CMonthLong 17.3.3.15 (Part 1)
		//--------------------------------------------------------------------------------	
		class CMonthLong : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMonthLong)
			CMonthLong() {}
			virtual ~CMonthLong() {}
			
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				return _T("<w:monthLong />");
			}

			virtual EElementType getType() const
			{
				return et_w_monthLong;
			}
		};


		//--------------------------------------------------------------------------------
		// CMonthShort 17.3.3.16 (Part 1)
		//--------------------------------------------------------------------------------	
		class CMonthShort : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMonthShort)
			CMonthShort() {}
			virtual ~CMonthShort() {}
			
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				return _T("<w:monthShort />");
			}

			virtual EElementType getType() const
			{
				return et_w_monthShort;
			}
		};


		//--------------------------------------------------------------------------------
		// CNoBreakHyphen 17.3.3.18 (Part 1)
		//--------------------------------------------------------------------------------	
		class CNoBreakHyphen : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CNoBreakHyphen)
			CNoBreakHyphen() {}
			virtual ~CNoBreakHyphen() {}
			
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				return _T("<w:noBreakHyphen />");
			}

			virtual EElementType getType() const
			{
				return et_w_nonBreakHyphen;
			}
		};


		//--------------------------------------------------------------------------------
		// CPgNum 17.3.3.22 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPgNum : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPgNum)
			CPgNum() {}
			virtual ~CPgNum() {}
			
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				return _T("<w:pgNum />");
			}

			virtual EElementType getType() const
			{
				return et_w_pgNum;
			}
		};
		//--------------------------------------------------------------------------------
		// CPTab 17.3.3.23 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPTab : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPTab)
			CPTab() {}
			virtual ~CPTab() {}
			
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				oNode.ReadAttributeBase( _T("w:alignment"),  m_oAlignment );
				oNode.ReadAttributeBase( _T("w:leader"),     m_oLeader );
				oNode.ReadAttributeBase( _T("w:relativeTo"), m_oRelativeTo );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<w:ptab ");

				ComplexTypes_WriteAttribute( _T("w:alignment=\""),  m_oAlignment );
				ComplexTypes_WriteAttribute( _T("w:leader=\""),     m_oLeader );
				ComplexTypes_WriteAttribute( _T("w:relativeTo=\""), m_oRelativeTo );

				sResult += _T("/>");
				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_ptab;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				CWCharWrapper wsName = oReader.GetName();
				while( !wsName.IsNull() )
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

		public:

			nullable<SimpleTypes::CPTabAlignment<> > m_oAlignment;
			nullable<SimpleTypes::CPTabLeader<>    > m_oLeader;
			nullable<SimpleTypes::CPTabRelativeTo<>> m_oRelativeTo;

		};
		//--------------------------------------------------------------------------------
		// CRuby 17.3.3.25 (Part 1)
		//--------------------------------------------------------------------------------	
		class CRuby : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRuby)
			CRuby() {}
			virtual ~CRuby() {}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				return _T("<w:ruby />");
			}

			virtual EElementType getType() const
			{
				return et_w_ruby;
			}
		};


		//--------------------------------------------------------------------------------
		// CSoftHyphen 17.3.3.29 (Part 1)
		//--------------------------------------------------------------------------------	
		class CSoftHyphen : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSoftHyphen)
			CSoftHyphen() {}
			virtual ~CSoftHyphen() {}
			
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				return _T("<w:softHyphen />");
			}

			virtual EElementType getType() const
			{
				return et_w_softHyphen;
			}
		};


		//--------------------------------------------------------------------------------
		// CSym 17.3.3.30 (Part 1)
		//--------------------------------------------------------------------------------	
		class CSym : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSym)
			CSym() {}
			virtual ~CSym() {}
			
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				oNode.ReadAttributeBase( _T("w:char"), m_oChar );
				oNode.ReadAttributeBase( _T("w:font"), m_oFont );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<w:sym ");

				ComplexTypes_WriteAttribute( _T("w:char=\""), m_oChar );
				
				if ( m_oFont.IsInit() )
				{
					sResult += _T("w:font=\"");
					sResult += m_oFont->GetString();
					sResult += _T("\" ");
				}

				sResult += _T("/>");
				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_sym;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				CWCharWrapper wsName = oReader.GetName();
				while( !wsName.IsNull() )
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

		public:

			nullable<SimpleTypes::CShortHexNumber<> > m_oChar;
			nullable<CString                        > m_oFont;

		};


		//--------------------------------------------------------------------------------
		// CText 17.3.3.31 (Part 1)
		//--------------------------------------------------------------------------------	
		class CText : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CText)
			CText() {}
			virtual ~CText() {}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				oNode.ReadAttributeBase( _T("xml:space"), m_oSpace );

				m_sText = oNode.GetText();
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				m_sText = oReader.GetText2();
			}
			virtual CString      toXML() const
			{
				CString sResult;
				
				if ( m_oSpace.IsInit() )
					sResult.Format( _T("<w:t xml:space=\"%ls\">"), m_oSpace->ToString() );
				else
					sResult = _T("<w:t>");

				sResult += m_sText;
				sResult += _T("</w:t>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_t;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				CWCharWrapper wsName = oReader.GetName();
				while( !wsName.IsNull() )
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

		public:

			// Attributes
			nullable<SimpleTypes::CXmlSpace<> > m_oSpace;

			// Value
			CString                             m_sText;

		};

		//--------------------------------------------------------------------------------
		// CTab 17.3.3.32 (Part 1)
		//--------------------------------------------------------------------------------	
		class CTab : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTab)
			CTab() {}
			virtual ~CTab() {}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				return _T("<w:tab />");
			}

			virtual EElementType getType() const
			{
				return et_w_tab;
			}
		};


		//--------------------------------------------------------------------------------
		// CYearLong 17.3.3.33 (Part 1)
		//--------------------------------------------------------------------------------	
		class CYearLong : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CYearLong)
			CYearLong() {}
			virtual ~CYearLong() {}
			
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				return _T("<w:yearLong />");
			}

			virtual EElementType getType() const
			{
				return et_w_yearLong;
			}
		};


		//--------------------------------------------------------------------------------
		// CYearShort 17.3.3.32 (Part 1)
		//--------------------------------------------------------------------------------	
		class CYearShort : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CYearShort)
			CYearShort() {}
			virtual ~CYearShort() {}
			
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				return _T("<w:yearShort />");
			}

			virtual EElementType getType() const
			{
				return et_w_yearShort;
			}
		};


	} // Logic
} // OOX

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CAnnotationRef 17.13.4.1 (Part 1)
		//--------------------------------------------------------------------------------
		class CAnnotationRef : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAnnotationRef)
			CAnnotationRef() {}
			virtual ~CAnnotationRef() {}
			
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				return _T("<w:annotationRef />");
			}

			virtual EElementType getType() const
			{
				return et_w_annotationRef;
			}
		};


		//--------------------------------------------------------------------------------
		// CCommentReference 17.13.4.5 (Part 1)
		//--------------------------------------------------------------------------------	
		class CCommentReference : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCommentReference)
			CCommentReference() {}
			virtual ~CCommentReference() {}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				oNode.ReadAttributeBase( _T("w:id"), m_oId );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<w:commentReference ");

				ComplexTypes_WriteAttribute( _T("w:id=\""), m_oId );

				sResult += _T("/>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_commentReference;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				CWCharWrapper wsName = oReader.GetName();
				while( !wsName.IsNull() )
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

		public:

			nullable<SimpleTypes::CDecimalNumber<> > m_oId;
		};


		//--------------------------------------------------------------------------------
		// CContinuationSeparator 17.11.1 (Part 1)
		//--------------------------------------------------------------------------------	
		class CContinuationSeparator : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CContinuationSeparator)
			CContinuationSeparator() {}
			virtual ~CContinuationSeparator() {}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				return _T("<w:continuationSeparator />");
			}

			virtual EElementType getType() const
			{
				return et_w_continuationSeparator;
			}
		};


		//--------------------------------------------------------------------------------
		// CDelInstrText 17.16.13 (Part 1)
		//--------------------------------------------------------------------------------	
		class CDelInstrText : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDelInstrText)
			CDelInstrText() {}
			virtual ~CDelInstrText() {}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				oNode.ReadAttributeBase( _T("xml:space"), m_oSpace );

				m_sText = oNode.GetText();
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				m_sText = oReader.GetText2();
			}
			virtual CString      toXML() const
			{
				CString sResult;
				
				if ( m_oSpace.IsInit() )
					sResult.Format( _T("<w:delInstrText xml:space=\"%ls\">"), m_oSpace->ToString() );
				else
					sResult = _T("<w:delInstrText>");

				sResult += m_sText;
				sResult += _T("</w:delInstrText>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_delInstrText;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				CWCharWrapper wsName = oReader.GetName();
				while( !wsName.IsNull() )
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


		public:

			// Attributes
			nullable<SimpleTypes::CXmlSpace<> > m_oSpace;

			// Value
			CString                             m_sText;

		};
		//--------------------------------------------------------------------------------
		// CEndnoteRef 17.11.6 (Part 1)
		//--------------------------------------------------------------------------------	
		class CEndnoteRef : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CEndnoteRef)
			CEndnoteRef() {}
			virtual ~CEndnoteRef() {}
			
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				return _T("<w:endnoteRef />");
			}

			virtual EElementType getType() const
			{
				return et_w_endnoteRef;
			}
		};


		//--------------------------------------------------------------------------------
		// CEndnoteReference 17.11.7 (Part 1)
		//--------------------------------------------------------------------------------	
		class CEndnoteReference : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CEndnoteReference)
			CEndnoteReference() {}
			virtual ~CEndnoteReference() {}
			
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				oNode.ReadAttributeBase( _T("w:customMarkFollows"), m_oCustomMarkFollows );
				oNode.ReadAttributeBase( _T("w:id"),                m_oId );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<w:endnoteReference ");

				ComplexTypes_WriteAttribute( _T("w:customMarkFollows=\""), m_oCustomMarkFollows );
				ComplexTypes_WriteAttribute( _T("w:id=\""),                m_oId );

				sResult += _T("/>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_endnoteReference;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				CWCharWrapper wsName = oReader.GetName();
				while( !wsName.IsNull() )
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


		public:

			nullable<SimpleTypes::COnOff<>        > m_oCustomMarkFollows;
			nullable<SimpleTypes::CDecimalNumber<>> m_oId;

		};


		//--------------------------------------------------------------------------------
		// CEndnoteRef 17.11.13 (Part 1)
		//--------------------------------------------------------------------------------	
		class CFootnoteRef : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFootnoteRef)
			CFootnoteRef() {}
			virtual ~CFootnoteRef() {}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				return _T("<w:footnoteRef />");
			}

			virtual EElementType getType() const
			{
				return et_w_footnoteRef;
			}
		};


		//--------------------------------------------------------------------------------
		// CEndnoteReference 17.11.14 (Part 1)
		//--------------------------------------------------------------------------------	
		class CFootnoteReference : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFootnoteReference)
			CFootnoteReference() {}
			virtual ~CFootnoteReference() {}
		
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				oNode.ReadAttributeBase( _T("w:customMarkFollows"), m_oCustomMarkFollows );
				oNode.ReadAttributeBase( _T("w:id"),                m_oId );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<w:footnoteReference ");

				ComplexTypes_WriteAttribute( _T("w:customMarkFollows=\""), m_oCustomMarkFollows );
				ComplexTypes_WriteAttribute( _T("w:id=\""),                m_oId );

				sResult += _T("/>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_footnoteReference;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				CWCharWrapper wsName = oReader.GetName();
				while( !wsName.IsNull() )
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

		public:

			nullable<SimpleTypes::COnOff<>        > m_oCustomMarkFollows;
			nullable<SimpleTypes::CDecimalNumber<>> m_oId;

		};


		//--------------------------------------------------------------------------------
		// CInstrText 17.16.23 (Part 1)
		//--------------------------------------------------------------------------------	
		class CInstrText : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CInstrText)
			CInstrText() {}
			virtual ~CInstrText() {}
			
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				oNode.ReadAttributeBase( _T("xml:space"), m_oSpace );

				m_sText = oNode.GetText();
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				m_sText = oReader.GetText2();
			}
			virtual CString      toXML() const
			{
				CString sResult;
				
				if ( m_oSpace.IsInit() )
					sResult.Format( _T("<w:instrText xml:space=\"%ls\">"), m_oSpace->ToString() );
				else
					sResult = _T("<w:instrText>");

				sResult += m_sText;
				sResult += _T("</w:instrText>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_instrText;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				CWCharWrapper wsName = oReader.GetName();
				while( !wsName.IsNull() )
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


		public:

			// Attributes
			nullable<SimpleTypes::CXmlSpace<> > m_oSpace;

			// Value
			CString                             m_sText;

		};
		//--------------------------------------------------------------------------------
		// CSeparator 17.11.23 (Part 1)
		//--------------------------------------------------------------------------------	
		class CSeparator : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSeparator)
			CSeparator() {}
			virtual ~CSeparator() {}
			
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}
			virtual CString      toXML() const
			{
				return _T("<w:separator />");
			}

			virtual EElementType getType() const
			{
				return et_w_separator;
			}
		};


	} // Logic
} // OOX
