#pragma once
#ifndef OOX_LOGIC_RUN_INCLUDE_H_
#define OOX_LOGIC_RUN_INCLUDE_H_

#include "RunProperty.h"
#include "RunContent.h"
#include "FldChar.h"
#include "FldSimple.h"

#include "../Drawing/Drawing.h"
#include "Pict.h"
#include "Annotations.h"
#include "AlternateContent.h"

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CRun 17.3.2.25 (Part 1)
		//--------------------------------------------------------------------------------	
		class CRun : public WritingElementWithChilds<>
		{
		public:
			CRun()
			{
				m_oRunProperty = NULL;
			}
			CRun(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CRun(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CRun()
			{
				Clear();
			}

		public:

			const CRun &operator =(const XmlUtils::CXmlNode& oNode)
			{
				Clear();
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			const CRun &operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				Clear();
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}

			void Clear()
			{
				m_oRsidDel.reset();
				m_oRsidR.reset();
				m_oRsidRPr.reset();
				m_oRunProperty = NULL;

				for (unsigned int nIndex = 0; nIndex < m_arrSpreadsheetItems.size(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )delete m_arrSpreadsheetItems[nIndex];
					m_arrSpreadsheetItems[nIndex] = NULL;
				}
				m_arrSpreadsheetItems.clear();				

				for (unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )delete m_arrItems[nIndex];
					m_arrItems[nIndex] = NULL;
				}
				m_arrItems.clear();
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_oRunProperty = NULL;
				
				oNode.ReadAttributeBase( _T("w:rsidDel"), m_oRsidDel );
				oNode.ReadAttributeBase( _T("w:rsidR"),   m_oRsidR );
				oNode.ReadAttributeBase( _T("w:rsidRPr"), m_oRsidRPr );

				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							CString sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("w:annotationRef") == sName )
								pItem = new CAnnotationRef( oItem );
							else if ( _T("w:br") == sName )
								pItem = new CBr( oItem );
							else if ( _T("w:commentReference") == sName )
								pItem = new CCommentReference( oItem );
							else if ( _T("w:contentPart") == sName )
								pItem = new CContentPart( oItem );
							else if ( _T("w:continuationSeparator") == sName )
								pItem = new CContinuationSeparator( oItem );
							else if ( _T("w:cr") == sName )
								pItem = new CCr( oItem );
							else if ( _T("w:dayLong") == sName )
								pItem = new CDayLong( oItem );
							else if ( _T("w:dayShort") == sName )
								pItem = new CDayShort( oItem );
							else if ( _T("w:delInstrText") == sName )
								pItem = new CDelInstrText( oItem );
							else if ( _T("w:delText") == sName )
								pItem = new CDelText( oItem );
							else if ( _T("w:drawing") == sName ) 
								pItem = new CDrawing( oItem );
							else if ( _T("w:endnoteRef") == sName )
								pItem = new CEndnoteRef( oItem );
							else if ( _T("w:endnoteReference") == sName )
								pItem = new CEndnoteReference( oItem );
							else if ( _T("w:fldChar") == sName )
								pItem = new CFldChar( oItem );
							else if ( _T("w:footnoteRef") == sName )
								pItem = new CFootnoteRef( oItem );
							else if ( _T("w:footnoteReference") == sName )
								pItem = new CFootnoteReference( oItem );
							else if ( _T("w:instrText") == sName )
								pItem = new CInstrText( oItem );
							else if ( _T("w:lastRenderedPageBreak") == sName )
								pItem = new CLastRenderedPageBreak( oItem );
							else if ( _T("w:monthLong") == sName )
								pItem = new CMonthLong( oItem );
							else if ( _T("w:monthShort") == sName )
								pItem = new CMonthShort( oItem );
							else if ( _T("w:noBreakHyphen") == sName )
								pItem = new CNoBreakHyphen( oItem );
							else if ( _T("w:object") == sName )
								pItem = new CObject( oItem );
							else if ( _T("w:pgNum") == sName )
								pItem = new CPgNum( oItem );
							else if ( _T("w:pict") == sName )
								pItem = new CPicture( oItem );
							else if ( _T("w:ptab") == sName )
								pItem = new CPTab( oItem );
							else if ( _T("w:rPr") == sName )
							{
								pItem = m_oRunProperty = new CRunProperty( oItem );
							}
							else if ( _T("w:ruby") == sName )
								pItem = new CRuby( oItem );
							else if ( _T("w:separator") == sName )
								pItem = new CSeparator( oItem );
							else if ( _T("w:softHyphen") == sName )
								pItem = new CSoftHyphen( oItem );
							else if ( _T("w:sym") == sName )
								pItem = new CSym( oItem );
							else if ( _T("w:t") == sName )
								pItem = new CText( oItem );
							else if ( _T("w:tab") == sName )
								pItem = new CTab( oItem );
							else if ( _T("w:yearLong") == sName )
								pItem = new CYearLong( oItem );
							else if ( _T("w:yearShort") == sName )
								pItem = new CYearShort( oItem );

							if ( pItem )
								m_arrItems.push_back( pItem );
						}
					}
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_oRunProperty = NULL;

				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();
					WritingElement *pItem = NULL;
					OOX::Spreadsheet::WritingElement *pSpreadsheetItem = NULL;

					if ( _T("mc:AlternateContent") == sName )
						pItem = new CAlternateContent( oReader );
					else if ( _T("w:annotationRef") == sName )
						pItem = new CAnnotationRef( oReader );
					else if ( _T("w:br") == sName )
						pItem = new CBr( oReader );
					else if ( _T("w:commentReference") == sName )
						pItem = new CCommentReference( oReader );
					else if ( _T("w:contentPart") == sName )
						pItem = new CContentPart( oReader );
					else if ( _T("w:continuationSeparator") == sName )
						pItem = new CContinuationSeparator( oReader );
					else if ( _T("w:cr") == sName )
						pItem = new CCr( oReader );
					else if ( _T("w:dayLong") == sName )
						pItem = new CDayLong( oReader );
					else if ( _T("w:dayShort") == sName )
						pItem = new CDayShort( oReader );
					else if ( _T("w:delInstrText") == sName )
						pItem = new CDelInstrText( oReader );
					else if ( _T("w:delText") == sName )
						pItem = new CDelText( oReader );
					else if ( _T("w:drawing") == sName ) 
						pItem = new CDrawing( oReader );
					else if ( _T("w:endnoteRef") == sName )
						pItem = new CEndnoteRef( oReader );
					else if ( _T("w:endnoteReference") == sName )
						pItem = new CEndnoteReference( oReader );
					else if ( _T("w:fldChar") == sName )
						pItem = new CFldChar( oReader );
					else if ( _T("w:footnoteRef") == sName )
						pItem = new CFootnoteRef( oReader );
					else if ( _T("w:footnoteReference") == sName )
						pItem = new CFootnoteReference( oReader );
					else if ( _T("w:instrText") == sName )
						pItem = new CInstrText( oReader );
					else if ( _T("w:lastRenderedPageBreak") == sName )
						pItem = new CLastRenderedPageBreak( oReader );
					else if ( _T("w:monthLong") == sName )
						pItem = new CMonthLong( oReader );
					else if ( _T("w:monthShort") == sName )
						pItem = new CMonthShort( oReader );
					else if ( _T("w:noBreakHyphen") == sName )
						pItem = new CNoBreakHyphen( oReader );
					else if ( _T("w:object") == sName )
						pItem = new CObject( oReader );
					else if ( _T("w:pgNum") == sName )
						pItem = new CPgNum( oReader );
					else if ( _T("w:pict") == sName )
						pItem = new CPicture( oReader );
					else if ( _T("w:ptab") == sName )
						pItem = new CPTab( oReader );
					else if ( _T("w:rPr") == sName )
					{
						pItem = m_oRunProperty = new CRunProperty( oReader );
					}
					else if ( _T("w:ruby") == sName )
						pItem = new CRuby( oReader );
					else if ( _T("w:separator") == sName )
						pItem = new CSeparator( oReader );
					else if ( _T("w:softHyphen") == sName )
						pItem = new CSoftHyphen( oReader );
					else if ( _T("w:sym") == sName )
						pItem = new CSym( oReader );
					else if ( _T("w:t") == sName )
						pItem = new CText( oReader );
					else if ( _T("w:tab") == sName )
						pItem = new CTab( oReader );
					else if ( _T("w:yearLong") == sName )
						pItem = new CYearLong( oReader );

					if ( pItem )
						m_arrItems.push_back( pItem );
					if ( pSpreadsheetItem )
						m_arrSpreadsheetItems.push_back( pSpreadsheetItem );
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<w:r ");

				ComplexTypes_WriteAttribute( _T("w:rsidDel=\""), m_oRsidDel );
				ComplexTypes_WriteAttribute( _T("w:rsidR=\""),   m_oRsidR );
				ComplexTypes_WriteAttribute( _T("w:rsidRPr=\""), m_oRsidRPr );

				sResult += _T(">");

				for (unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
					{
						sResult += m_arrItems[nIndex]->toXML();
					}
				}
				for (unsigned int nIndex = 0; nIndex < m_arrSpreadsheetItems.size(); nIndex++ )
				{
					if ( m_arrSpreadsheetItems[nIndex] )
					{
						sResult += m_arrSpreadsheetItems[nIndex]->toXML();
					}
				}
				

				sResult += _T("</w:r>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_w_r;
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
					if ( _T("w:rsidDel") == wsName )
						m_oRsidDel = oReader.GetText();
					else if ( _T("w:rsidR") == wsName )
						m_oRsidR = oReader.GetText();
					else if ( _T("w:rsidRPr") == wsName )
						m_oRsidRPr = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}

				oReader.MoveToElement();
			}

		public:

			// Attributes
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidDel;
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidR;
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidRPr;

			// Childs
			std::vector<OOX::Spreadsheet::WritingElement *> m_arrSpreadsheetItems;

			OOX::Logic::CRunProperty						*m_oRunProperty;	// копия того что в m_arrItems...  - для быстрого доступа/анализа
			// по идее нужно сделать как в Drawing::Run - то есть единственные подобъекты вынести отдельно
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_RUN_INCLUDE_H_