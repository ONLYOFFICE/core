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
		class CStringXLSB
		{
		public:
			CStringXLSB(_UINT32 nSize);
			~CStringXLSB();
			void Clean();
			void fromXML(XmlUtils::CXmlLiteReader& oReader, bool bPreserve);
			void fromStringA(const char* sVal);
		public:
			WCHAR* m_sBuffer;
			LONG m_nSize;
			LONG m_nLen;
		protected:
			void checkBufferSize(_UINT32 nRequired);
		};
		class CTextXLSB
		{
		public:
			CTextXLSB(_UINT32 nSize);
			void Clean();
			void fromXML(XmlUtils::CXmlLiteReader& oReader, SimpleTypes::Spreadsheet::ECellTypeType eType);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			bool m_bIsInit;
			SimpleTypes::CXmlSpace<> m_oSpace;

			double m_dValue;
			_UINT32 m_nValue;
			CStringXLSB m_oValue;
		};
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
				writer.WriteEncodeXmlStringHHHH(m_sText);
				writer.WriteString(_T("</t>"));
			}
			virtual void toXML2(NSStringUtils::CStringBuilder& writer, const wchar_t* name) const
			{
				writer.WriteString(_T("<"));
				writer.WriteString(name);
				if(std::wstring::npos != m_sText.find(' ') || std::wstring::npos != m_sText.find('\n'))
					writer.WriteString(_T(" xml:space=\"preserve\""));
				writer.WriteString(_T(">"));
				writer.WriteEncodeXmlStringHHHH(m_sText);
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
					if (eNodeType == XmlUtils::XmlNodeType_Text || eNodeType == XmlUtils::XmlNodeType_Whitespace || eNodeType == XmlUtils::XmlNodeType_SIGNIFICANT_WHITESPACE)
					{
						std::string sTemp = oReader.GetTextA();
						wchar_t* pUnicodes = NULL;
						LONG lOutputCount = 0;
						NSFile::CUtf8Converter::GetUnicodeStringFromUTF8WithHHHH((BYTE*)sTemp.c_str(), sTemp.length(), pUnicodes, lOutputCount);
						m_sText.append(pUnicodes);
						RELEASEARRAYOBJECTS(pUnicodes);
					}
				}

				trimString(m_sText, GetSpace());
			}
			static void trimString(std::wstring& sVal, SimpleTypes::EXmlSpace eSpace);
			std::wstring ToString() const
			{
				return m_sText;
			}
			SimpleTypes::EXmlSpace GetSpace() const;
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
