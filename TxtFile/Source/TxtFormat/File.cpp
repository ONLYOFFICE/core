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
	void File::read(const std::wstring& filename, int code_page) // насильственное чтение в кодировке
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
		
		//читаем юникод чтобы можно было выкинуть невалидные символы

		if (file.isUtf8())
		{
			m_listContent = NSEncoding::transformToUnicode(file.readUtf8(), 46); //65001 Unicode (UTF-8)
		}
		else if (file.isUnicode())
		{
			m_listContent = file.readUnicode();
		}
		else if (file.isBigEndian())
		{
			m_listContent = file.readBigEndian();
		}
		//проверка убрана, потому что она работает в редких случаюх: если в первой строке есть английские символы
		//далее не делается проверка BigEndian или LittleEndian
		//notepad++ открывает такие файлы как ansi и мы будем также.
		//else if (file.isUnicodeWithOutBOM())
		//	listContentUnicode = file.readUnicodeWithOutBOM();
		else
		{
			int nCodePage = m_nEncoding;
			if (-1 == nCodePage) nCodePage = 46;
			else if (1000 == nCodePage) nCodePage = -1;

			m_listContent = NSEncoding::transformToUnicode(file.readAnsiOrCodePage(), nCodePage);
		}
		
		m_listContentSize = file.getLinesCount();
	}

	void File::write(const std::wstring& filename) const
	{
		TxtFile file(filename);
		file.writeUtf8(NSEncoding::transformFromUnicode(m_listContent, 46));
	}

	void File::writeCodePage(const std::wstring& filename, int code_page) const
	{
		TxtFile file(filename);
		file.writeAnsiOrCodePage(NSEncoding::transformFromUnicode(m_listContent, code_page));
	}

	void File::writeUtf8(const std::wstring& filename) const
	{
		TxtFile file(filename);
		file.writeUtf8(NSEncoding::transformFromUnicode(m_listContent, 46));
	}

	void File::writeUnicode(const std::wstring& filename) const
	{
		TxtFile file(filename);
		file.writeUnicode(m_listContent);
	}

	void File::writeBigEndian(const std::wstring& filename) const
	{
		TxtFile file(filename);
		file.writeBigEndian(m_listContent);
	}

	void File::writeAnsi(const std::wstring& filename) const
	{
		TxtFile file(filename);
		file.writeAnsiOrCodePage(NSEncoding::transformFromUnicode(m_listContent, -1));
	}

	const bool File::isValid(const std::wstring& filename) const
	{
		if (filename.empty())
			return true;
		return NSFile::CFileBinary::Exists(filename);
	}
} // namespace Txt
