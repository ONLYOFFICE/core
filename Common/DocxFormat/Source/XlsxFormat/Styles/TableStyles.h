#pragma once
#ifndef OOX_TABLESTYLES_FILE_INCLUDE_H_
#define OOX_TABLESTYLES_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "rPr.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CTableStyleElement : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CTableStyleElement)
			CTableStyleElement()
			{
			}
			virtual ~CTableStyleElement()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oType.IsInit() && m_oDxfId.IsInit())
				{
					CString sXml;
					if(m_oSize.IsInit())
						sXml.Format(_T("<tableStyleElement type=\"%s\" size=\"%d\" dxfId=\"%d\"/>"), m_oType->ToString(), m_oSize->GetValue(), m_oDxfId->GetValue());
					else
						sXml.Format(_T("<tableStyleElement type=\"%s\" dxfId=\"%d\"/>"), m_oType->ToString(), m_oDxfId->GetValue());
					writer.WriteString(sXml);
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_TableStyleElement;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
								// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("dxfId"),      m_oDxfId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("size"),      m_oSize )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("type"),      m_oType )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oDxfId;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oSize;
			nullable<SimpleTypes::Spreadsheet::CTableStyleType<>>	m_oType;
		};
		class CTableStyle : public WritingElementWithChilds<CTableStyleElement>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CTableStyle)
			CTableStyle()
			{
			}
			virtual ~CTableStyle()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oName.IsInit() && m_arrItems.GetSize() > 0)
				{
					writer.WriteString(_T("<tableStyle"));
					if(m_oName.IsInit())
					{
						CString sName;
						sName.Format(_T(" name=\"%s\""), XmlUtils::EncodeXmlString(m_oName.get2()));
						writer.WriteString(sName);
					}
					if(m_oPivot.IsInit() && true == m_oPivot->ToBool())
						writer.WriteString(_T(" table=\"0\""));
					else
						writer.WriteString(_T(" pivot=\"0\""));
					if(m_oCount.IsInit())
					{
						CString sCount;
						sCount.Format(_T(" count=\"%d\""), m_oCount->GetValue());
						writer.WriteString(sCount);
					}
					writer.WriteString(_T(">"));
					for(int i = 0, length = m_arrItems.GetSize(); i < length; ++i)
						m_arrItems[i]->toXML(writer);
					writer.WriteString(_T("</tableStyle>"));
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
					CWCharWrapper sName = oReader.GetName();

					if ( _T("tableStyleElement") == sName )
						m_arrItems.Add( new CTableStyleElement( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_TableStyle;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
								// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("name"),       m_oName )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("pivot"),      m_oPivot )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("table"),      m_oTable )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("displayName"),m_oDisplayName )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCount;
			nullable<CString>									m_oName;
			nullable<SimpleTypes::COnOff<>>						m_oPivot;
			nullable<SimpleTypes::COnOff<>>						m_oTable;
			nullable<CString>									m_oDisplayName; // Используется только для дефалтовых стилей
		};
		class CTableStyles : public WritingElementWithChilds<CTableStyle>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CTableStyles)
			CTableStyles()
			{
			}
			virtual ~CTableStyles()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<tableStyles"));
				if(m_oCount.IsInit())
				{
					CString sVal;sVal.Format(_T(" count=\"%d\""), m_oCount->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oDefaultTableStyle.IsInit())
				{
					CString sVal;sVal.Format(_T(" defaultTableStyle=\"%s\""), XmlUtils::EncodeXmlString(m_oDefaultTableStyle.get()));
					writer.WriteString(sVal);
				}
				if(m_oDefaultPivotStyle.IsInit())
				{
					CString sVal;sVal.Format(_T(" defaultPivotStyle=\"%s\""), XmlUtils::EncodeXmlString(m_oDefaultPivotStyle.get()));
					writer.WriteString(sVal);
				}
				if(m_arrItems.GetSize() >  0)
				{
					writer.WriteString(_T(">"));
					for(int i = 0, length = m_arrItems.GetSize(); i < length; ++i)
						m_arrItems[i]->toXML(writer);
					writer.WriteString(_T("</tableStyles>"));
				}
				else
					writer.WriteString(_T("/>"));
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

					if ( _T("tableStyle") == sName )
						m_arrItems.Add( new CTableStyle( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_TableStyles;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("defaultPivotStyle"),      m_oDefaultPivotStyle )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("defaultTableStyle"),      m_oDefaultTableStyle )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCount;
			nullable<CString>									m_oDefaultPivotStyle;
			nullable<CString>									m_oDefaultTableStyle;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_TABLESTYLES_FILE_INCLUDE_H_