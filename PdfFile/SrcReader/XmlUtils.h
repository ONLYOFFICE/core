/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#ifndef _PDF_READER_XML_UTILS_H
#define _PDF_READER_XML_UTILS_H

#include <string>
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Base64.h"
#include <map>

namespace PdfReader
{
	class CXmlWriter
	{
	public:

		CXmlWriter()
		{
			m_wsXml.clear();
		}

		const wchar_t* GetXmlString()
		{
			return m_wsXml.c_str();
		}
		void SetXmlString(const std::wstring& wsXml)
		{
			m_wsXml = wsXml;
		}
		bool SaveToFile(const std::wstring& wsFilePath, bool bEncodingToUTF8 = false)
		{
			return NSFile::CFileBinary::SaveToFile(wsFilePath, m_wsXml);
		}
		void WriteString(const std::wstring& wsString)
		{
			m_wsXml += wsString;
		}
		void WriteInteger(int nValue, int Base = 10)
		{
			m_wsXml += std::to_wstring(nValue);
		}
		void WriteDouble(double dValue)
		{
			m_wsXml += std::to_wstring(dValue);
		}
		void WriteBoolean(bool bValue)
		{
			if (bValue)
				m_wsXml += L"true";
			else
				m_wsXml += L"false";
		}
		void WriteNodeBegin(const std::wstring& wsNodeName, bool bAttributed = false)
		{
			m_wsXml += L"<" + wsNodeName;

			if (!bAttributed)
				m_wsXml += L">";
		}
		void WriteNodeEnd(const std::wstring& wsNodeName, bool bEmptyNode = false, bool bEndNode = true)
		{
			if (bEmptyNode)
			{
				if (bEndNode)
					m_wsXml += L" />";
				else
					m_wsXml += L">";
			}
			else
				m_wsXml += L"</" + wsNodeName + L">";
		}
		void WriteNode(const std::wstring& wsNodeName, const std::wstring& wsNodeValue)
		{
			if (0 == wsNodeValue.length())
				m_wsXml += L"<" + wsNodeName + L"/>";
			else
				m_wsXml += L"<" + wsNodeName + L">" + wsNodeValue + L"</" + wsNodeName + L">";
		}
		void WriteNode(const std::wstring& wsNodeName, int nValue, int nBase = 10, const std::wstring& wsTextBeforeValue = L"", const std::wstring& wsTextAfterValue = L"")
		{
			WriteNodeBegin(wsNodeName);
			WriteString(wsTextBeforeValue);
			WriteInteger(nValue, nBase);
			WriteString(wsTextAfterValue);
			WriteNodeEnd(wsNodeName);
		}
		void WriteNode(const std::wstring& wsNodeName, double dValue)
		{
			WriteNodeBegin(wsNodeName);
			WriteDouble(dValue);
			WriteNodeEnd(wsNodeName);
		}
		void WriteAttribute(const std::wstring& wsAttributeName, const std::wstring& wsAttributeValue)
		{
			m_wsXml += L" " + wsAttributeName + L"=\"" + wsAttributeValue + L"\"";
		}
		void WriteAttribute(const std::wstring& wsAttributeName, int nValue, int nBase = 10, const std::wstring& wsTextBeforeValue = L"", const std::wstring& wsTextAfterValue = L"")
		{
			WriteString(L" " + wsAttributeName + L"=");
			WriteString(L"\"");
			WriteString(wsTextBeforeValue);
			WriteInteger(nValue, nBase);
			WriteString(wsTextAfterValue);
			WriteString(L"\"");
		}
		void WriteAttribute(const std::wstring& wsAttributeName, double dValue)
		{
			WriteString(L" " + wsAttributeName + L"=");
			WriteString(L"\"");
			WriteDouble(dValue);
			WriteString(L"\"");
		}
	public:
		static void ReplaceSpecialCharacters(std::wstring& wsString)
		{
			ReplaceAll(wsString, L"&", L"&amp;");
			ReplaceAll(wsString, L"<", L"&lt;");
			ReplaceAll(wsString, L">", L"&gt;");
			ReplaceAll(wsString, L"\"", L"&quot;");
			ReplaceAll(wsString, L"'", L"&apos;");
		}
		static void ReplaceAll(std::wstring& wsSrc, const std::wstring& wsFrom, const std::wstring& wsTo)
		{
			if (wsFrom.empty())
				return;

			int nFromLen = wsFrom.length();
			int nToLen = wsTo.length();

			size_t nStartPos = 0;
			while (std::string::npos != (nStartPos = wsSrc.find(wsFrom, nStartPos)))
			{
				wsSrc.replace(nStartPos, nFromLen, wsTo);
				nStartPos += nToLen;
			}
		}

	private:

		std::wstring m_wsXml;

	};
	
	class CBase64
	{
	public:

		void Encode(unsigned char* pSrc, int nSrcLen)
		{
			m_sBuffer.clear();
			int nRequiredLen = NSBase64::Base64EncodeGetRequiredLength(nSrcLen);
			if (0 == nRequiredLen)
				return;

			unsigned char* pDst = new unsigned char[nRequiredLen + 1];
			if (!pDst)
				return;

			pDst[nRequiredLen] = 0x00;

			int nDstLen;
			NSBase64::Base64Encode(pSrc, nSrcLen, pDst, &nDstLen);
			m_sBuffer.append((char*)pDst);
			delete[] pDst;
		}
		std::string& GetString()
		{
			return m_sBuffer;
		}
		const char* GetCString()
		{
			return m_sBuffer.c_str();
		}

	private:

		std::string m_sBuffer;
	};
}

#endif // _PDF_READER_XML_UTILS_H