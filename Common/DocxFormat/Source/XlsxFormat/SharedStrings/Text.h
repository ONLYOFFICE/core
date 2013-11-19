#pragma once
#ifndef OOX_TEXT_FILE_INCLUDE_H_
#define OOX_TEXT_FILE_INCLUDE_H_

#include "../CommonInclude.h"


namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		class CText : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CText)
			CText() {}
			virtual ~CText() {}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(CStringWriter& writer) const
			{
				writer.WriteStringC(_T("<t"));
				if(-1 != m_sText.Find(' ') || -1 != m_sText.Find('\n'))
					writer.WriteStringC(_T(" xml:space=\"preserve\""));
				writer.WriteStringC(_T(">"));
				writer.WriteStringC(XmlUtils::EncodeXmlString(m_sText));
				writer.WriteStringC(_T("</t>"));
			}
			virtual void toXML2(CStringWriter& writer, CString name) const
			{
				writer.WriteStringC(_T("<"));
				writer.WriteStringC(name);
				if(-1 != m_sText.Find(' ') || -1 != m_sText.Find('\n'))
					writer.WriteStringC(_T(" xml:space=\"preserve\""));
				writer.WriteStringC(_T(">"));
				writer.WriteStringC(XmlUtils::EncodeXmlString(m_sText));
				writer.WriteStringC(_T("</"));
				writer.WriteStringC(name);
				writer.WriteStringC(_T(">"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				m_sText = oReader.GetText2();
				m_sText.Replace(_T("\t"), _T(""));
				if(!(m_oSpace.IsInit() && SimpleTypes::xmlspacePreserve == m_oSpace->GetValue()))
				{
					//убираем пробелы и переносы строк в начале и в конце
					int nLength = m_sText.GetLength();
					int nStartIndex = 0;
					int nEndIndex = nLength - 1;
					for(int i = nStartIndex; i < nLength; ++i)
					{
						TCHAR cElem = m_sText[i];
						if(' ' == cElem || '\n' == cElem || '\r' == cElem)
							nStartIndex++;
						else
							break;
					}
					for(int i = nEndIndex; i > nStartIndex; --i)
					{
						TCHAR cElem = m_sText[i];
						if(' ' == cElem || '\n' == cElem || '\r' == cElem)
							nEndIndex--;
						else
							break;
					}
					if(0 != nStartIndex || nLength - 1 != nEndIndex)
					{
						if(nStartIndex <= nEndIndex)
							m_sText = m_sText.Mid(nStartIndex, nEndIndex - nStartIndex + 1);
						else
							m_sText.Empty();
					}
				}
			}
			CString ToString() const
			{
				return m_sText;
			}
			virtual EElementType getType() const
			{
				return et_t;
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
	} //Spreadsheet
} // namespace OOX

#endif // OOX_TEXT_FILE_INCLUDE_H_