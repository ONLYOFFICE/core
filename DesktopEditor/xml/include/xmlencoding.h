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
#ifndef _BUILD_XMLUTILS_ENCODINGS_H_
#define _BUILD_XMLUTILS_ENCODINGS_H_

#include "../../common/File.h"
#include "../../../UnicodeConverter/UnicodeConverter.h"
#define XML_HEADER_CHECKER_LENGHT 100

namespace XmlUtils
{
	// this function assumes that the input is definitely xml.
	// only need to determine encoding and return string in utf-8
	static std::string GetXmlContentAsUTF8(const std::wstring& sFile)
	{
		std::string sXmlSource;
		if (!NSFile::CFileBinary::ReadAllTextUtf8A(sFile, sXmlSource))
			return sXmlSource;

		std::string::size_type nCheckCount = sXmlSource.length();
		std::string sChecker = (nCheckCount > XML_HEADER_CHECKER_LENGHT) ? sXmlSource.substr(0, XML_HEADER_CHECKER_LENGHT) : sXmlSource;

		std::string::size_type posEncoding = sChecker.find("encoding=\"");
		if (std::string::npos == posEncoding)
			return sXmlSource;

		posEncoding += 10; // len(encoding=\")
		std::string::size_type posEnd = sChecker.find("\"", posEncoding);
		if (std::string::npos == posEnd)
			return sXmlSource;

		std::string sEncoding = sChecker.substr(posEncoding, posEnd - posEncoding);
		if (sEncoding == "utf-8" || sEncoding == "UTF-8")
			return sXmlSource;

		posEnd = sChecker.find(">", posEnd);
		if (std::string::npos == posEnd)
			return sXmlSource;

		sXmlSource = "<?xml version=\"1.0\" encoding=\"utf-8\"?>" + sXmlSource.substr(posEnd + 1);

		NSUnicodeConverter::CUnicodeConverter oConverter;
		std::wstring sUnicodeContent = oConverter.toUnicode(sXmlSource, sEncoding.c_str());
		return U_TO_UTF8(sUnicodeContent);
	}
}

#endif // _BUILD_XMLUTILS_ENCODINGS_H_
