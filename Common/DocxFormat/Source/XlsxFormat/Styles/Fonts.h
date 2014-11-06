#pragma once
#ifndef OOX_FONTS_FILE_INCLUDE_H_
#define OOX_FONTS_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "rPr.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CFont : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CFont)
			CFont()
			{
			}
			virtual ~CFont()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<font>"));
				if(m_oRFont.IsInit() && m_oRFont->m_sVal.IsInit())
				{
					CString sVal;sVal.Format(_T("<name val=\"%ls\"/>"), XmlUtils::EncodeXmlString(m_oRFont->m_sVal.get()));
					writer.WriteString(sVal);
				}			
				if(m_oCharset.IsInit() && m_oCharset->m_oCharset.IsInit())
				{
					CString sVal;sVal.Format(_T("<charset val=\"%ls\"/>"), m_oCharset->m_oCharset->ToString());
					writer.WriteString(sVal);
				}
				if(m_oFamily.IsInit() && m_oFamily->m_oFontFamily.IsInit())
				{
					CString sVal;sVal.Format(_T("<family val=\"%ls\"/>"), m_oFamily->m_oFontFamily->ToString());
					writer.WriteString(sVal);
				}
				if(m_oBold.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oBold->m_oVal.GetValue())
						writer.WriteString(_T("<b/>"));
					else
						writer.WriteString(_T("<b val=\"0\"/>"));
				}
				if(m_oItalic.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oItalic->m_oVal.GetValue())
						writer.WriteString(_T("<i/>"));
					else
						writer.WriteString(_T("<i val=\"0\"/>"));
				}
				if(m_oStrike.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oStrike->m_oVal.GetValue())
						writer.WriteString(_T("<strike/>"));
					else
						writer.WriteString(_T("<strike val=\"0\"/>"));
				}
				if(m_oOutline.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oOutline->m_oVal.GetValue())
						writer.WriteString(_T("<outline/>"));
					else
						writer.WriteString(_T("<outline val=\"0\"/>"));
				}
				if(m_oShadow.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oShadow->m_oVal.GetValue())
						writer.WriteString(_T("<shadow/>"));
					else
						writer.WriteString(_T("<shadow val=\"0\"/>"));
				}
				if(m_oCondense.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oCondense->m_oVal.GetValue())
						writer.WriteString(_T("<condense/>"));
					else
						writer.WriteString(_T("<condense val=\"0\"/>"));
				}
				if(m_oExtend.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oExtend->m_oVal.GetValue())
						writer.WriteString(_T("<extend/>"));
					else
						writer.WriteString(_T("<extend val=\"0\"/>"));
				}
				if(m_oColor.IsInit())
					m_oColor->toXML2(writer, _T("color"));
				if(m_oSz.IsInit() && m_oSz->m_oVal.IsInit())
				{
					CString sVal;sVal.Format(_T("<sz val=\"%ls\"/>"), SpreadsheetCommon::WriteDouble(m_oSz->m_oVal->GetValue()));
					writer.WriteString(sVal);
				}
				if(m_oUnderline.IsInit() && m_oUnderline->m_oUnderline.IsInit())
				{
					CString sVal;
					if( SimpleTypes::underlineSingle != m_oUnderline->m_oUnderline->GetValue())
						sVal.Format(_T("<u val=\"%ls\"/>"), m_oUnderline->m_oUnderline->ToString());
					else
						sVal.Format(_T("<u/>"), m_oUnderline->m_oUnderline->ToString());
					writer.WriteString(sVal);
				}
				if(m_oVertAlign.IsInit() && m_oVertAlign->m_oVerticalAlign.IsInit())
				{
					CString sVal;sVal.Format(_T("<vertAlign val=\"%ls\"/>"), m_oVertAlign->m_oVerticalAlign->ToString());
					writer.WriteString(sVal);
				}
				if(m_oScheme.IsInit() && m_oScheme->m_oFontScheme.IsInit())
				{
					CString sVal;sVal.Format(_T("<scheme val=\"%ls\"/>"), m_oScheme->m_oFontScheme->ToString());
					writer.WriteString(sVal);
				}
				writer.WriteString(_T("</font>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if ( _T("b") == sName )
						m_oBold = oReader;
					else if ( _T("charset") == sName )
						m_oCharset = oReader;
					else if ( _T("color") == sName )
						m_oColor = oReader;
					else if ( _T("condense") == sName )
						m_oCondense = oReader;
					else if ( _T("extend") == sName )
						m_oExtend = oReader;
					else if ( _T("family") == sName )
						m_oFamily = oReader;
					else if ( _T("i") == sName )
						m_oItalic = oReader;
					else if ( _T("name") == sName )
						m_oRFont = oReader;
					else if ( _T("outline") == sName )
						m_oOutline = oReader;
					else if ( _T("scheme") == sName )
						m_oScheme = oReader;
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

			virtual EElementType getType () const
			{
				return et_Font;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oBold;
			nullable<CCharset>												m_oCharset;
			nullable<CColor>												m_oColor;
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oCondense;
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oExtend;
			nullable<CFontFamily >											m_oFamily;
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oItalic;
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oOutline;
			nullable<ComplexTypes::Spreadsheet::CString_>							m_oRFont;
			nullable<CFontScheme>											m_oScheme;
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oShadow;
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oStrike;
			nullable<ComplexTypes::Spreadsheet::CDouble>							m_oSz;
			nullable<CUnderline>											m_oUnderline;
			nullable<CVerticalAlign>										m_oVertAlign;
		};
		class CFonts : public WritingElementWithChilds<CFont>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CFonts)
			CFonts()
			{
			}
			virtual ~CFonts()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<fonts"));
				if(m_oCount.IsInit())
				{
					CString sVal;sVal.Format(_T(" count=\"%d\""), m_oCount->GetValue());
					writer.WriteString(sVal);
				}
				writer.WriteString(_T(">"));
				for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
					m_arrItems[i]->toXML(writer);
				writer.WriteString(_T("</fonts>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if ( _T("font") == sName )
						m_arrItems.push_back( new CFont( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_Fonts;
			}
			void AddFont (CFont* pFont)
			{
				m_arrItems.push_back(pFont);
				if(false == m_oCount.IsInit())
					m_oCount.Init();
				m_oCount->SetValue(m_arrItems.size());
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCount;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_FONTS_FILE_INCLUDE_H_