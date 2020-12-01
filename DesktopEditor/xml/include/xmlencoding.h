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
#ifndef _BUILD_XMLUTILS_ENCODINGS_H_
#define _BUILD_XMLUTILS_ENCODINGS_H_

#include "../../common/File.h"
#include "../../../UnicodeConverter/UnicodeConverter.h"
#define XML_HEADER_CHECKER_LENGHT 100

namespace XmlUtils
{
	// эта функция считает, что на вход пришла точно хмл.
	// надо лишь определить кодировку и отдать строку в utf-8
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
