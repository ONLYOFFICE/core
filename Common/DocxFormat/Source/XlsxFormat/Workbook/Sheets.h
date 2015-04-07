#pragma once
#ifndef OOX_SHEETS_FILE_INCLUDE_H_
#define OOX_SHEETS_FILE_INCLUDE_H_

#include "../CommonInclude.h"


namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		//<extLst>
		class CSheet : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CSheet)
			CSheet()
			{
			}
			virtual ~CSheet()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<sheet"));
				if(m_oName.IsInit())
				{
					CString sVal;
					sVal.Append(_T(" name=\""));
					sVal.Append(XmlUtils::EncodeXmlString(m_oName.get()));
					sVal.Append(_T("\""));
					writer.WriteString(sVal);
				}
				if(m_oSheetId.IsInit())
				{
					CString sVal;sVal.Format(_T(" sheetId=\"%d\""), m_oSheetId->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oState.IsInit())
				{
					CString sVal;sVal.Format(_T(" state=\"%s\""), m_oState->ToString());
					writer.WriteString(sVal);
				}
				if(m_oRid.IsInit())
				{
					CString sVal;sVal.Format(_T(" r:id=\"%s\""), m_oRid->GetValue());
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
				return et_Sheet;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:id"),      m_oRid )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("name"),      m_oName )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("sheetId"),      m_oSheetId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("state"),      m_oState )

					WritingElement_ReadAttributes_End( oReader )
			}

		public:
				nullable<SimpleTypes::CRelationshipId>				m_oRid;
				nullable<CString>									m_oName;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oSheetId;
				nullable<SimpleTypes::Spreadsheet::CVisibleType<>>	m_oState;

		};

		class CSheets  : public WritingElementWithChilds<CSheet>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CSheets)
			CSheets()
			{
			}
			virtual ~CSheets()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<sheets>"));
				for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
					m_arrItems[i]->toXML(writer);
				writer.WriteString(_T("</sheets>"));
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

					if ( _T("sheet") == sName )
						m_arrItems.push_back( new CSheet( oReader ));

				}
			}

			virtual EElementType getType () const
			{
				return et_Sheets;
			}
		
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_SHEETS_FILE_INCLUDE_H_