/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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
#ifndef OOX_TEXT_FILE_INCLUDE_H_
#define OOX_TEXT_FILE_INCLUDE_H_

#include "../CommonInclude.h"
#include "../../../../../DesktopEditor/common/StringExt.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		class CText : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CText)
			CText() {}
			virtual ~CText() {}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<t"));
				if(std::wstring::npos != m_sText.find(' ') || std::wstring::npos != m_sText.find('\n'))
					writer.WriteString(_T(" xml:space=\"preserve\""));
				writer.WriteString(_T(">"));
				writer.WriteEncodeXmlString(m_sText);
				writer.WriteString(_T("</t>"));
			}
			virtual void toXML2(NSStringUtils::CStringBuilder& writer, const wchar_t* name) const
			{
				writer.WriteString(_T("<"));
				writer.WriteString(name);
				if(std::wstring::npos != m_sText.find(' ') || std::wstring::npos != m_sText.find('\n'))
					writer.WriteString(_T(" xml:space=\"preserve\""));
				writer.WriteString(_T(">"));
				writer.WriteEncodeXmlString(m_sText);
				writer.WriteString(_T("</"));
				writer.WriteString(name);
				writer.WriteString(_T(">"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nDepth = oReader.GetDepth();
				XmlUtils::XmlNodeType eNodeType = XmlUtils::XmlNodeType_EndElement;
				while (oReader.Read(eNodeType) && oReader.GetDepth() >= nDepth && XmlUtils::XmlNodeType_EndElement != eNodeType)
				{
					if (eNodeType == XmlNodeType_Text || eNodeType == XmlUtils::XmlNodeType_Whitespace || eNodeType == XmlUtils::XmlNodeType_SIGNIFICANT_WHITESPACE)
					{
						std::string sTemp = oReader.GetText2A();
						wchar_t* pUnicodes = NULL;
						LONG lOutputCount = 0;
						NSFile::CUtf8Converter::GetUnescapedUnicodeStringFromUTF8((BYTE*)sTemp.c_str(), sTemp.length(), pUnicodes, lOutputCount);
						m_sText.append(pUnicodes);
						RELEASEARRAYOBJECTS(pUnicodes);
					}
				}

				NSStringExt::Replace(m_sText, L"\t", L"");
				if(!(m_oSpace.IsInit() && SimpleTypes::xmlspacePreserve == m_oSpace->GetValue()))
				{
					//trim ' ', '\r', '\n'
					int nLength		= (int)m_sText.length();
					int nStartIndex = 0;
					int nEndIndex	= nLength - 1;
					
					for(int i = nStartIndex; i < nLength; ++i)
					{
                        wchar_t cElem = m_sText[i];
						if(' ' == cElem || '\n' == cElem || '\r' == cElem)
							nStartIndex++;
						else
							break;
					}
					for(int i = nEndIndex; i > nStartIndex; --i)
					{
                        wchar_t cElem = m_sText[i];
						if(' ' == cElem || '\n' == cElem || '\r' == cElem)
							nEndIndex--;
						else
							break;
					}
					if(0 != nStartIndex || nLength - 1 != nEndIndex)
					{
						if (nStartIndex <= nEndIndex)
							m_sText = m_sText.substr(nStartIndex, nEndIndex - nStartIndex + 1);
						else
							m_sText.clear();
					}
				}
			}
			std::wstring ToString() const
			{
				return m_sText;
			}
			virtual EElementType getType() const
			{
				return et_x_t;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				std::wstring wsName = XmlUtils::GetNameNoNS(oReader.GetName());
				while( !wsName.empty() )
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
			std::wstring                             m_sText;

		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_TEXT_FILE_INCLUDE_H_
