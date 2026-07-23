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
#include "File.h"
#include "TxtFile.h"

#include "../../../DesktopEditor/common/File.h"
#include <locale>

#include "../../../UnicodeConverter/UnicodeConverter.h"
#include "../../../UnicodeConverter/UnicodeConverter_Encodings.h"

namespace NSEncoding
{
	std::wstring ansi2unicode(const std::string& line)
	{
		if (line.empty())
			return std::wstring();

		std::locale loc("");
		std::ctype<wchar_t> const &facet = std::use_facet<std::ctype<wchar_t> >(loc);

		std::wstring result;
		result.resize(line.size());

		facet.widen(line.c_str(), line.c_str() + line.size(), &result[0]);
		return result;
	}

	std::string unicode2ansi(const std::wstring& line)
	{
		if (line.empty())
			return std::string();

		std::locale loc("");
		std::ctype<wchar_t> const &facet = std::use_facet<std::ctype<wchar_t> >(loc);

		std::string result;
		result.resize(line.size());

		facet.narrow(line.c_str(), line.c_str() + line.size(), '?', &result[0]);
		return result;
	}

	std::vector<std::wstring> transformToUnicode(const std::vector<std::string>& lines, int code_page)
	{
		std::vector<std::wstring> result;

		if (-1 == code_page)
		{
			for (std::vector<std::string>::const_iterator iter = lines.begin(); iter != lines.end(); iter++)
			{
				result.push_back(ansi2unicode(*iter));
			}
		}
		else
		{
			const NSUnicodeConverter::EncodindId& oEncodindId = NSUnicodeConverter::Encodings[code_page];
			NSUnicodeConverter::CUnicodeConverter oUnicodeConverter;

			for (std::vector<std::string>::const_iterator iter = lines.begin(); iter != lines.end(); iter++)
			{
				result.push_back(oUnicodeConverter.toUnicode(*iter, oEncodindId.Name));
			}
		}
		return result;
	}
	std::vector<std::string> transformFromUnicode(const std::vector<std::wstring>& lines, int code_page)
	{
		std::vector<std::string> result;

		if (-1 == code_page)
		{
			for (std::vector<std::wstring>::const_iterator iter = lines.begin(); iter != lines.end(); iter++)
			{
				result.push_back(unicode2ansi(*iter));
			}
		}
		else
		{
			const NSUnicodeConverter::EncodindId& oEncodindId = NSUnicodeConverter::Encodings[code_page];
			NSUnicodeConverter::CUnicodeConverter oUnicodeConverter;

			for (std::vector<std::wstring>::const_iterator iter = lines.begin(); iter != lines.end(); iter++)
			{
				result.push_back(oUnicodeConverter.fromUnicode(*iter, oEncodindId.Name));
			}
		}
		return result;
	}
}

namespace Txt
{

	File::File()
	{
	}
	File::~File()
	{
		m_listContent.clear();
	}
	void File::read(const std::wstring& filename, int code_page) // forced reading with encoding
	{
		m_listContent.clear();

		if (filename.empty())
			return;

		TxtFile file(filename);

		std::vector<std::string> codePageContent = file.readAnsiOrCodePage();
		m_listContentSize = file.getLinesCount();

		const NSUnicodeConverter::EncodindId& oEncodindId = NSUnicodeConverter::Encodings[code_page];
		NSUnicodeConverter::CUnicodeConverter oUnicodeConverter;

		for (std::vector<std::string>::const_iterator iter = codePageContent.begin(); iter != codePageContent.end(); ++iter)
		{
			m_listContent.push_back(oUnicodeConverter.toUnicode(*iter, oEncodindId.Name));
		}
		codePageContent.clear();
	}
	void File::read(const std::wstring& filename)
	{
		m_listContent.clear();

		if (filename.empty())
			return;

		TxtFile file(filename);

        //m_listContent = file.readUnicodeLines(m_nEncoding);
        m_listContentutf8 = file.readUtf8Lines(m_nEncoding);
		m_listContentSize = file.getLinesCount();
	}

	bool File::write(const std::wstring& filename) const
	{
		TxtFile file(filename);
		return file.writeUtf8(NSEncoding::transformFromUnicode(m_listContent, 46));
	}

	bool File::writeCodePage(const std::wstring& filename, int code_page) const
	{
		TxtFile file(filename);
		return file.writeAnsiOrCodePage(NSEncoding::transformFromUnicode(m_listContent, code_page));
	}

	bool File::writeUtf8(const std::wstring& filename) const
	{
		TxtFile file(filename);
		return file.writeUtf8(NSEncoding::transformFromUnicode(m_listContent, 46));
	}

	bool File::writeUnicode(const std::wstring& filename) const
	{
		TxtFile file(filename);
		return file.writeUnicode(m_listContent);
	}

	bool File::writeBigEndian(const std::wstring& filename) const
	{
		TxtFile file(filename);
		return file.writeBigEndian(m_listContent);
	}

	bool File::writeAnsi(const std::wstring& filename) const
	{
		TxtFile file(filename);
		return file.writeAnsiOrCodePage(NSEncoding::transformFromUnicode(m_listContent, -1));
	}

	const bool File::isValid(const std::wstring& filename) const
	{
		if (filename.empty())
			return true;
		return NSFile::CFileBinary::Exists(filename);
	}
} // namespace Txt
