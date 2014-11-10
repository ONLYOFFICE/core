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
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<t"));
				if(-1 != m_sText.Find(' ') || -1 != m_sText.Find('\n'))
					writer.WriteString(_T(" xml:space=\"preserve\""));
				writer.WriteString(_T(">"));
				writer.WriteString(XmlUtils::EncodeXmlString(m_sText));
				writer.WriteString(_T("</t>"));
			}
			virtual void toXML2(XmlUtils::CStringWriter& writer, CString name) const
			{
				writer.WriteString(_T("<"));
				writer.WriteString(name);
				if(-1 != m_sText.Find(' ') || -1 != m_sText.Find('\n'))
					writer.WriteString(_T(" xml:space=\"preserve\""));
				writer.WriteString(_T(">"));
				writer.WriteString(XmlUtils::EncodeXmlString(m_sText));
				writer.WriteString(_T("</"));
				writer.WriteString(name);
				writer.WriteString(_T(">"));
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

				CString wsName = XmlUtils::GetNameNoNS(oReader.GetName());
				while( !wsName.IsEmpty() )
				{
					if ( _T("space") == wsName )
					{
						m_oSpace = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = XmlUtils::GetNameNoNS(oReader.GetName());
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