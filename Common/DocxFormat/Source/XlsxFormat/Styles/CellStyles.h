#pragma once
#ifndef OOX_CELLSTYLES_FILE_INCLUDE_H_
#define OOX_CELLSTYLES_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//нереализован:
		//<extLst>
		class CCellStyle : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CCellStyle)
			CCellStyle()
			{
			}
			virtual ~CCellStyle()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<cellStyle"));
				if(m_oName.IsInit())
				{
					CString sVal;sVal.Format(_T(" name=\"%ls\""), XmlUtils::EncodeXmlString(m_oName.get()));
					writer.WriteString(sVal);
				}
				if(m_oXfId.IsInit())
				{
					CString sVal;sVal.Format(_T(" xfId=\"%d\""), m_oXfId->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oBuiltinId.IsInit())
				{
					CString sVal;sVal.Format(_T(" builtinId=\"%d\""), m_oBuiltinId->GetValue());
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
				return et_CellStyle;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("builtinId"),      m_oBuiltinId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("customBuiltin"),      m_oCustomBuiltin )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("hidden"),      m_oHidden )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("iLevel"),      m_oILevel )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("name"),      m_oName )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("xfId"),      m_oXfId )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oBuiltinId;
			nullable<SimpleTypes::COnOff<>>					m_oCustomBuiltin;
			nullable<SimpleTypes::COnOff<>>					m_oHidden;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oILevel;
			nullable<CString>								m_oName;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oXfId;
		};

		class CCellStyles : public WritingElementWithChilds<CCellStyle>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CCellStyles)
			CCellStyles()
			{
			}
			virtual ~CCellStyles()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<cellStyles"));
				if(m_oCount.IsInit())
				{
					CString sVal;sVal.Format(_T(" count=\"%d\""), m_oCount->GetValue());
					writer.WriteString(sVal);
				}
				writer.WriteString(_T(">"));
				for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
					m_arrItems[i]->toXML(writer);
				writer.WriteString(_T("</cellStyles>"));
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

					if ( _T("cellStyle") == sName )
						m_arrItems.push_back( new CCellStyle( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_CellStyles;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_ReadSingle ( oReader, _T("count"),      m_oCount )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCount;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_CELLSTYLES_FILE_INCLUDE_H_