#pragma once
#ifndef OOX_DEFINEDNAMES_FILE_INCLUDE_H_
#define OOX_DEFINEDNAMES_FILE_INCLUDE_H_

#include "../CommonInclude.h"


namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		//<extLst>
		class CDefinedName : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CDefinedName)
			CDefinedName()
			{
			}
			virtual ~CDefinedName()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<definedName"));
				if(m_oName.IsInit())
				{
					CString sVal;sVal.Format(_T(" name=\"%s\""), XmlUtils::EncodeXmlString(m_oName.get()));
					writer.WriteString(sVal);
				}
				if(m_oLocalSheetId.IsInit())
				{
					CString sVal;sVal.Format(_T(" localSheetId=\"%d\""), m_oLocalSheetId->GetValue());
					writer.WriteString(sVal);
				}
				writer.WriteString(_T(">"));
				if(m_oRef.IsInit())
					writer.WriteString(XmlUtils::EncodeXmlString(m_oRef.get()));
				writer.WriteString(_T("</definedName>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				m_oRef = oReader.GetText2().GetString();
			}

			virtual EElementType getType () const
			{
				return et_DefinedName;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("comment"),      m_oComment )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("customMenu"),      m_oCustomMenu )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("description"),      m_oDescription )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("function"),      m_oFunction )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("functionGroupId"),      m_oFunctionGroupId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("help"),      m_oHelp )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("hidden"),      m_oHidden )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("localSheetId"),      m_oLocalSheetId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("name"),      m_oName )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("publishToServer"),      m_oPublishToServer )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("shortcutKey "),      m_oShortcutKey  )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("statusBar "),      m_oStatusBar  )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("vbProcedure "),      m_oVbProcedure  )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("workbookParameter "),      m_oWorkbookParameter  )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("xlm "),      m_oXlm  )
					

					WritingElement_ReadAttributes_End( oReader )
			}

		public:
				nullable<CString>								m_oComment;
				nullable<CString>								m_oCustomMenu;
				nullable<CString>								m_oDescription;
				nullable<SimpleTypes::COnOff<>>					m_oFunction;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oFunctionGroupId;
				nullable<CString>								m_oHelp;
				nullable<SimpleTypes::COnOff<>>					m_oHidden;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oLocalSheetId;
				nullable<CString>								m_oName;
				nullable<SimpleTypes::COnOff<>>					m_oPublishToServer;
				nullable<CString>								m_oShortcutKey;
				nullable<CString>								m_oStatusBar;
				nullable<SimpleTypes::COnOff<>>					m_oVbProcedure;
				nullable<SimpleTypes::COnOff<>>					m_oWorkbookParameter;
				nullable<SimpleTypes::COnOff<>>					m_oXlm;

				nullable<CString>								m_oRef;
		};

		class CDefinedNames  : public WritingElementWithChilds<CDefinedName>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CDefinedNames)
			CDefinedNames()
			{
			}
			virtual ~CDefinedNames()
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
					writer.WriteString(_T("<definedNames>"));
					for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
						m_arrItems[i]->toXML(writer);
					writer.WriteString(_T("</definedNames>"));
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

					if ( _T("definedName") == sName )
						m_arrItems.push_back( new CDefinedName( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_BookViews;
			}
		
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_DEFINEDNAMES_FILE_INCLUDE_H_