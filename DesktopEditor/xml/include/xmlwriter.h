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
#ifndef _BUILD_XMLWRITER_CROSSPLATFORM_H_
#define _BUILD_XMLWRITER_CROSSPLATFORM_H_

#include <vector>
#include <list>
#include <map>
#include <memory>

#ifdef _MSC_VER
#pragma warning (disable: 4100 4189)
#endif

#include "../../common/StringBuilder.h"

#ifndef XML_UNUSED
#define XML_UNUSED( arg )  ( (arg) = (arg) )
#endif

namespace XmlUtils
{
	class KERNEL_DECL CXmlWriter
	{
	private:
		class Impl;
		std::shared_ptr<Impl> impl_;

	public:

		CXmlWriter();
		~CXmlWriter();
		
		std::wstring GetXmlString();
		void SetXmlString(const std::wstring& strValue);
		
		void WriteEncodeXmlString(const std::string& sString);
		bool SaveToFile(const std::wstring& strFilePath/*, bool bEncodingToUTF8 = false*/);
		void WriteString(const std::wstring& strValue);
		void WriteInteger(int Value);
		void WriteDouble(double Value);
		void WriteBoolean(bool Value);
		void WriteNodeBegin(const std::wstring& strNodeName, bool bAttributed = false);
		void WriteNodeEnd(const std::wstring& strNodeName, bool bEmptyNode = false, bool bEndNode = true);
		void WriteNode(const std::wstring& strNodeName, const std::wstring& strNodeValue);
		void WriteNode(const std::wstring& strNodeName, int nValue, const std::wstring& strTextBeforeValue = L"", const std::wstring& strTextAfterValue = L"");
		void WriteNode(const std::wstring& strNodeName, double dValue);
		void WriteAttribute(const std::wstring& strAttributeName, const std::wstring& strAttributeValue);
		void WriteAttribute(const std::wstring& strAttributeName, const std::string& strAttributeValue);
		void WriteAttribute(const std::wstring& strAttributeName, int nValue, const std::wstring& strTextBeforeValue = L"", const std::wstring& strTextAfterValue = (L""));
		void WriteAttribute(const std::wstring& strAttributeName, double dValue);
	};
}

#endif // _BUILD_XMLWRITER_CROSSPLATFORM_H_
