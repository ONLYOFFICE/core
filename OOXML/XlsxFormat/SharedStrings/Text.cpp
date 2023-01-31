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
#include "Text.h"
#include "../../XlsbFormat/Biff12_structures/RichStr.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CStringXLSB::CStringXLSB(_UINT32 nSize)
		{
			m_nSize = nSize;
			m_sBuffer = new WCHAR[m_nSize];
			Clean();
		}
		CStringXLSB::~CStringXLSB()
		{
			RELEASEARRAYOBJECTS(m_sBuffer);
		}
		void CStringXLSB::Clean()
		{
			m_nLen = 0;
		}
		void CStringXLSB::fromXML(XmlUtils::CXmlLiteReader& oReader, bool bPreserve)
		{
			oReader.GetTextWithHHHH(bPreserve, m_sBuffer, m_nSize, m_nLen);
		}
		void CStringXLSB::fromStringA(const char* sVal)
		{
			LONG nLen = strlen((const char*)sVal);
			checkBufferSize(NSFile::CUtf8Converter::GetUnicodeStringFromUTF8BufferSize(nLen));
			NSFile::CUtf8Converter::GetUnicodeStringFromUTF8WithHHHH((const BYTE*)sVal, nLen, m_sBuffer, m_nLen);
		}
		void CStringXLSB::checkBufferSize(_UINT32 nRequired)
		{
			if(nRequired > m_nSize)
			{
				while(nRequired > m_nSize)
				{
					m_nSize *= 2;
				}
				RELEASEOBJECT(m_sBuffer);
				m_sBuffer = new WCHAR[m_nSize];
			}
		}
		CTextXLSB::CTextXLSB(_UINT32 nSize):m_oValue(nSize)
		{
			Clean();
		}
		void CTextXLSB::Clean()
		{
			m_bIsInit = false;
			m_oSpace.SetValue(SimpleTypes::xmlspaceDefault);
			m_dValue = 0;
			m_nValue = 0;
			m_oValue.Clean();
		}
		void CTextXLSB::fromXML(XmlUtils::CXmlLiteReader& oReader, SimpleTypes::Spreadsheet::ECellTypeType eType)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
			{
				m_bIsInit = SimpleTypes::Spreadsheet::celltypeStr == eType || SimpleTypes::Spreadsheet::celltypeInlineStr == eType;
				return;
			}

			if(SimpleTypes::Spreadsheet::celltypeStr == eType || SimpleTypes::Spreadsheet::celltypeInlineStr == eType)
			{
				m_bIsInit = true;
				m_oValue.fromXML(oReader, SimpleTypes::xmlspacePreserve == m_oSpace.GetValue());
			}
			else
			{
				int nDepth = oReader.GetDepth();
				XmlUtils::XmlNodeType eNodeType = XmlUtils::XmlNodeType_EndElement;
				while (oReader.Read(eNodeType) && oReader.GetDepth() >= nDepth && XmlUtils::XmlNodeType_EndElement != eNodeType)
				{
					if (eNodeType == XmlUtils::XmlNodeType_Text)
					{
						const char* pVal = oReader.GetTextChar();
						if(pVal[0] == '\0')
							continue;
						m_bIsInit = true;
						if(SimpleTypes::Spreadsheet::celltypeNumber == eType)
						{
							//todo RkNumber
							try
							{
								m_dValue = atof(pVal);
							}
							catch(...)
							{   //1.3912059045063478e-310
								//Lighting Load Calculation.xls
							}
						}
						else if(SimpleTypes::Spreadsheet::celltypeSharedString == eType)
						{
							try
							{
								m_nValue = atoi(pVal);
							}
							catch(...)
							{
							}
						}
						else if(SimpleTypes::Spreadsheet::celltypeError == eType)
						{
							if(strcmp("#NULL!", pVal) == 0)
							{
								m_nValue = 0x00;
							}
							else if(strcmp("#DIV/0!", pVal) == 0)
							{
								m_nValue = 0x07;
							}
							else if(strcmp("#VALUE!", pVal) == 0)
							{
								m_nValue = 0x0F;
							}
							else if(strcmp("#REF!", pVal) == 0)
							{
								m_nValue = 0x17;
							}
							else if(strcmp("#NAME?", pVal) == 0)
							{
								m_nValue = 0x1D;
							}
							else if(strcmp("#NUM!", pVal) == 0)
							{
								m_nValue = 0x24;
							}
							else if(strcmp("#N/A", pVal) == 0)
							{
								m_nValue = 0x2A;
							}
							else if(strcmp("#GETTING_DATA", pVal) == 0)
							{
								m_nValue = 0x2B;
							}
						}
						else if(SimpleTypes::Spreadsheet::celltypeBool == eType)
						{
							SimpleTypes::COnOff oOnOff;
							oOnOff.FromStringA(pVal);
							m_nValue = oOnOff.ToBool() ? 1 : 0;
						}
					}
				}
			}
		}
		void CTextXLSB::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_StartChar( oReader )

			if ( strcmp("xml:space", wsName) == 0 )
			{
				m_oSpace.FromStringA(oReader.GetTextChar());
				break;
			}

			WritingElement_ReadAttributes_EndChar( oReader )
		}

		CText::CText() {}
		CText::~CText() {}
		void CText::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CText::toXML() const
		{
			return _T("");
		}
		void CText::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<t"));
			if(std::wstring::npos != m_sText.find(' ') || std::wstring::npos != m_sText.find('\n'))
				writer.WriteString(_T(" xml:space=\"preserve\""));
			writer.WriteString(_T(">"));
			writer.WriteEncodeXmlStringHHHH(m_sText);
			writer.WriteString(_T("</t>"));
		}
		void CText::toXML2(NSStringUtils::CStringBuilder& writer, const wchar_t* name) const
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
		void CText::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
					NSFile::CUtf8Converter::GetUnicodeStringFromUTF8WithHHHH((BYTE*)sTemp.c_str(), (LONG)sTemp.length(), pUnicodes, lOutputCount);
					m_sText.append(pUnicodes);
					RELEASEARRAYOBJECTS(pUnicodes);
				}
			}

			trimString(m_sText, GetSpace());
		}
		void CText::fromBin(std::wstring& str)
		{
			m_sText = str;
			m_oSpace.Init();
			m_oSpace->SetValue(SimpleTypes::xmlspacePreserve);
		}
		std::wstring CText::ToString() const
		{
			return m_sText;
		}
		EElementType CText::getType() const
		{
			return et_x_t;
		}
		void CText::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
		void CText::trimString(std::wstring& sVal, SimpleTypes::EXmlSpace eSpace)
		{
			NSStringExt::Replace(sVal, L"\t", L"");
			if(SimpleTypes::xmlspacePreserve != eSpace)
			{
				//trim ' ', '\r', '\n'
				int nLength		= (int)sVal.length();
				int nStartIndex = 0;
				int nEndIndex	= nLength - 1;

				for(int i = nStartIndex; i < nLength; ++i)
				{
					wchar_t cElem = sVal[i];
					if(' ' == cElem || '\n' == cElem || '\r' == cElem)
						nStartIndex++;
					else
						break;
				} 
				for(int i = nEndIndex; i > nStartIndex; --i)
				{
					wchar_t cElem = sVal[i];
					if(' ' == cElem || '\n' == cElem || '\r' == cElem)
						nEndIndex--;
					else
						break;
				}
				if(0 != nStartIndex || nLength - 1 != nEndIndex)
				{
					if (nStartIndex <= nEndIndex)
						sVal = sVal.substr(nStartIndex, nEndIndex - nStartIndex + 1);
					else
						sVal.clear();
				}
			}
		}
		SimpleTypes::EXmlSpace CText::GetSpace() const
		{
			return m_oSpace.IsInit() ? m_oSpace->GetValue() : SimpleTypes::xmlspaceDefault;
		}

	} //Spreadsheet
} // OOX
