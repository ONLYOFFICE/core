#pragma once
#ifndef OOX_COLS_FILE_INCLUDE_H_
#define OOX_COLS_FILE_INCLUDE_H_

#include "../CommonInclude.h"


namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		//<extLst>
		class CCol : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CCol)
			CCol()
			{
			}
			virtual ~CCol()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<col"));
				if(m_oBestFit.IsInit())
				{
					CString sVal; sVal.Format(_T(" bestFit=\"%ls\""), m_oBestFit->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}
				if(m_oCollapsed.IsInit())
				{
					CString sVal; sVal.Format(_T(" collapsed=\"%ls\""), m_oCollapsed->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}
				if(m_oCustomWidth.IsInit())
				{
					CString sVal; sVal.Format(_T(" customWidth=\"%ls\""), m_oCustomWidth->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}
				if(m_oHidden.IsInit())
				{
					CString sVal; sVal.Format(_T(" hidden=\"%ls\""), m_oHidden->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}
				if(m_oMin.IsInit())
				{
					CString sVal; sVal.Format(_T(" min=\"%d\""), m_oMin->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oMax.IsInit())
				{
					CString sVal; sVal.Format(_T(" max=\"%d\""), m_oMax->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oOutlineLevel.IsInit())
				{
					CString sVal; sVal.Format(_T(" outlineLevel=\"%d\""), m_oOutlineLevel->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oPhonetic.IsInit())
				{
					CString sVal; sVal.Format(_T(" phonetic=\"%ls\""), m_oPhonetic->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}
				if(m_oStyle.IsInit())
				{
					CString sVal; sVal.Format(_T(" style=\"%d\""), m_oStyle->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oWidth.IsInit())
				{
					CString sVal; sVal.Format(_T(" width=\"%ls\""), SpreadsheetCommon::WriteDouble(m_oWidth->GetValue()));
					writer.WriteString(sVal);
				}
				writer.WriteString(_T("/>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_Col;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("bestFit"),      m_oBestFit)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("collapsed"),      m_oCollapsed )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("customWidth"),      m_oCustomWidth )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("hidden"),      m_oHidden )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("max"),      m_oMax )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("min"),      m_oMin )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("outlineLevel"),      m_oOutlineLevel )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("phonetic"),      m_oPhonetic )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("style"),      m_oStyle )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("width"),      m_oWidth )

					WritingElement_ReadAttributes_End( oReader )
			}

		public:
				nullable<SimpleTypes::COnOff<>>					m_oBestFit;
				nullable<SimpleTypes::COnOff<>>					m_oCollapsed;
				nullable<SimpleTypes::COnOff<>>					m_oCustomWidth;
				nullable<SimpleTypes::COnOff<>>					m_oHidden;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMax;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMin;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oOutlineLevel;
				nullable<SimpleTypes::COnOff<>>					m_oPhonetic;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oStyle;
				nullable<SimpleTypes::CDouble>					m_oWidth;
		};

		class CCols  : public WritingElementWithChilds<CCol>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CCols)
			CCols()
			{
			}
			virtual ~CCols()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_arrItems.size() > 0)
				{
					writer.WriteString(_T("<cols>"));
					for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
						m_arrItems[i]->toXML(writer);
					writer.WriteString(_T("</cols>"));
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("col") == sName )
						m_arrItems.push_back( new CCol( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_Cols;
			}
		
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_COLS_FILE_INCLUDE_H_