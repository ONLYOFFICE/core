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

namespace OOX
{
	namespace Spreadsheet
	{
		void CText::fromXMLToXLSB(XmlUtils::CXmlLiteReader& oReader, SimpleTypes::Spreadsheet::ECellTypeType eType, _UINT16& nType, double& dValue, unsigned int& nValue, BYTE& bValue, std::wstring** psValue, bool& bForceFormula)
		{
			SimpleTypes::EXmlSpace eSpace = SimpleTypes::xmlspaceDefault;
			ReadAttributesToXLSB( oReader, eSpace );

			if ( oReader.IsEmptyNode() )
				return;

			if(SimpleTypes::Spreadsheet::celltypeStr == eType || SimpleTypes::Spreadsheet::celltypeInlineStr == eType)
			{
				std::string sVal = oReader.GetText2A();
				fromXMLToXLSB(sVal.c_str(), eSpace, eType, nType, dValue, nValue, bValue, psValue, bForceFormula);
			}
			else
			{
				int nDepth = oReader.GetDepth();
				XmlUtils::XmlNodeType eNodeType = XmlUtils::XmlNodeType_EndElement;
				while (oReader.Read(eNodeType) && oReader.GetDepth() >= nDepth && XmlUtils::XmlNodeType_EndElement != eNodeType)
				{
					if (eNodeType == XmlUtils::XmlNodeType_Text)
					{
						fromXMLToXLSB(oReader.GetTextChar(), eSpace, eType, nType, dValue, nValue, bValue, psValue, bForceFormula);
					}
				}
			}
		}
		void CText::fromXMLToXLSB(const char* pVal, SimpleTypes::EXmlSpace eSpace, SimpleTypes::Spreadsheet::ECellTypeType eType, _UINT16& nType, double& dValue, unsigned int& nValue, BYTE& bValue, std::wstring** psValue, bool& bForceFormula)
		{
			if(pVal[0] == '\0')
				return;
			if(SimpleTypes::Spreadsheet::celltypeNumber == eType)
			{
				//todo RkNumber
				nType = XLSB::rt_CELL_REAL;
				try
				{
					dValue = atof(pVal);
				}
				catch(...)
				{   //1.3912059045063478e-310
					//Lighting Load Calculation.xls
				}
			}
			else if(SimpleTypes::Spreadsheet::celltypeSharedString == eType)
			{
				nType = XLSB::rt_CELL_ISST;
				try
				{
					nValue = atoi(pVal);
				}
				catch(...)
				{
				}
			}
			else if(SimpleTypes::Spreadsheet::celltypeError == eType)
			{
				nType = XLSB::rt_CELL_ERROR;
				if(strcmp("#NULL!", pVal) == 0)
				{
					bValue = 0x00;
				}
				else if(strcmp("#DIV/0!", pVal) == 0)
				{
					bValue = 0x07;
				}
				else if(strcmp("#VALUE!", pVal) == 0)
				{
					bValue = 0x0F;
				}
				else if(strcmp("#REF!", pVal) == 0)
				{
					bValue = 0x17;
				}
				else if(strcmp("#NAME?", pVal) == 0)
				{
					bValue = 0x1D;
				}
				else if(strcmp("#NUM!", pVal) == 0)
				{
					bValue = 0x24;
				}
				else if(strcmp("#N/A", pVal) == 0)
				{
					bValue = 0x2A;
				}
				else if(strcmp("#GETTING_DATA", pVal) == 0)
				{
					bValue = 0x2B;
				}
			}
			else if(SimpleTypes::Spreadsheet::celltypeBool == eType)
			{
				nType = XLSB::rt_CELL_BOOL;
				SimpleTypes::COnOff<> oOnOff;
				oOnOff.FromStringA(pVal);
				bValue = oOnOff.ToBool() ? 1 : 0;
			}
			else if(SimpleTypes::Spreadsheet::celltypeInlineStr == eType)
			{
				nType = XLSB::rt_CELL_ST;
				if(NULL == (*psValue))
				{
					(*psValue) = new std::wstring();
				}
				(*psValue)->append(NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pVal, (LONG)strlen(pVal)));
				trimString(**psValue, eSpace);
			}
			else if(SimpleTypes::Spreadsheet::celltypeStr == eType)
			{
				bForceFormula = true;
				nType = XLSB::rt_CELL_ST;
				if(NULL == (*psValue))
				{
					(*psValue) = new std::wstring();
				}
				(*psValue)->append(NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pVal, (LONG)strlen(pVal)));
				trimString(**psValue, eSpace);
			}
		}
		void CText::ReadAttributesToXLSB(XmlUtils::CXmlLiteReader& oReader, SimpleTypes::EXmlSpace& eSpace)
		{
			WritingElement_ReadAttributes_StartChar( oReader )
				if ( strcmp("space", wsName) == 0 )
				{
					SimpleTypes::CXmlSpace<> oSpace;
					oSpace.FromStringA(oReader.GetTextChar());
					eSpace = oSpace.GetValue();
					break;
				}

			WritingElement_ReadAttributes_EndChar( oReader )
		}
		void CText::trimString(std::wstring& sVal, SimpleTypes::EXmlSpace eSpace)
		{
			NSStringExt::Replace(sVal, L"\t", L"");
			if(SimpleTypes::xmlspacePreserve == eSpace)
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
