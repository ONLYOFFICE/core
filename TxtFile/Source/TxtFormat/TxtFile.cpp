/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "TxtFile.h"
#include "../../../OOXML/SystemUtility/File.h"
#include "../../../UnicodeConverter/UnicodeConverter.h"
#include "../../../UnicodeConverter/UnicodeConverter_Encodings.h"

static const std::string BadSymbols = "\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19";

TxtFile::TxtFile(const std::wstring & path) : m_path(path), m_linesCount(0)
{
}
const int TxtFile::getLinesCount()
{
	return m_linesCount;
}
const std::vector<std::string> TxtFile::readUtf8Lines(int IdxEncoding)
{
    std::vector<std::string> result;
    NSFile::CFileBinary file_binary;
    if (!file_binary.OpenFile(m_path))
        return result;
    DWORD file_size = file_binary.GetFileSize();
    if (file_size == 0)
        return result;
    std::unique_ptr<char[]> file_data(new char[file_size]);
    DWORD read_size = 0;
    file_binary.ReadFile((BYTE*)file_data.get(), file_size, read_size);
    if (read_size == 0)
        return result;
    bool isUtf8 = (read_size >= 3 && (BYTE)file_data[0] == 0xEF && (BYTE)file_data[1] == 0xBB && (BYTE)file_data[2] == 0xBF);
    bool isUtf16LE = (read_size >= 2 && (BYTE)file_data[0] == 0xFF && (BYTE)file_data[1] == 0xFE);
    bool isUtf16BE = (read_size >= 2 && (BYTE)file_data[0] == 0xFE && (BYTE)file_data[1] == 0xFF);

    NSUnicodeConverter::CUnicodeConverter conv;
    std::string utf8_content;

    if (isUtf8)
    {
        size_t content_start = 0;
        size_t content_size = read_size;

        if (read_size >= 3 && file_data[0] == '\xEF' && file_data[1] == '\xBB' && file_data[2] == '\xBF') {
            content_start = 3;
            content_size -= 3;
        }

        utf8_content.assign(file_data.get() + content_start, content_size);
    }
    else if (isUtf16LE)
    {
        std::wstring unicode_content = conv.toUnicode(file_data.get() + 2, read_size - 2, 1200);//UTF-16LE
        utf8_content = conv.fromUnicode(unicode_content, "UTF-8");
    }
    else if (isUtf16BE)
    {
        std::wstring unicode_content = conv.toUnicode(file_data.get() + 2, read_size - 2, 1201);//UTF-16BE
        utf8_content = conv.fromUnicode(unicode_content, "UTF-8");
    }
    else
    {
        std::wstring unicode_content;

        if (IdxEncoding >= 0 && IdxEncoding < UNICODE_CONVERTER_ENCODINGS_COUNT)
        {
            const char* encodingName = NSUnicodeConverter::Encodings[IdxEncoding].Name;
            unicode_content = conv.toUnicode(file_data.get(), read_size, encodingName);
        }
        else
        {
            unicode_content = conv.toUnicode(file_data.get(), read_size, 65001);
        }

        utf8_content = conv.fromUnicode(unicode_content, "UTF-8");
    }
    size_t lineCount = 0;
    if (!utf8_content.empty())
    {
        for (size_t i = 0; i < utf8_content.size(); ++i)
        {
            if (utf8_content[i] == '\n' || utf8_content[i] == '\r')
            {
                ++lineCount;
                if (utf8_content[i] == '\r' && i + 1 < utf8_content.size() && utf8_content[i + 1] == '\n')
                    ++i;
            }
        }
        if (!utf8_content.empty() && utf8_content.back() != '\n' && utf8_content.back() != '\r')
            ++lineCount;
    }

    result.reserve(lineCount);

    if (!utf8_content.empty())
    {
        size_t line_start = 0;
        size_t i = 0;

        while (i < utf8_content.size())
        {
            if (utf8_content[i] == '\n' || utf8_content[i] == '\r')
            {
                result.push_back(utf8_content.substr(line_start, i - line_start));
                if (utf8_content[i] == '\r' && i + 1 < utf8_content.size() && utf8_content[i + 1] == '\n') {
                    i += 2;
                } else {
                    i += 1;
                }

                line_start = i;
            }
            else
            {
                ++i;
            }
        }

        if (line_start < utf8_content.size()) {
            result.push_back(utf8_content.substr(line_start));
        }
    }

    m_linesCount = result.size();
    return result;
}

const std::vector<std::wstring> TxtFile::readUnicodeLines(int CodePage)
{
    std::vector<std::wstring> result;
    NSFile::CFileBinary file_binary;
    if (!file_binary.OpenFile(m_path)) return result;
    DWORD file_size = file_binary.GetFileSize();
    if (file_size == 0) return result;
    char* file_data = new char[file_size];
    DWORD read_size = 0;
    file_binary.ReadFile((BYTE*)file_data, file_size, read_size);
    if (read_size == 0) {
        delete[] file_data;
        return result;
    }
    bool isUtf8 = (read_size >= 3 && (BYTE)file_data[0] == 0xEF && (BYTE)file_data[1] == 0xBB && (BYTE)file_data[2] == 0xBF);
    bool isUtf16LE = (read_size >= 2 && (BYTE)file_data[0] == 0xFF && (BYTE)file_data[1] == 0xFE);
    bool isUtf16BE = (read_size >= 2 && (BYTE)file_data[0] == 0xFE && (BYTE)file_data[1] == 0xFF);
    std::wstring content;
    if (isUtf8)
    {
        NSUnicodeConverter::CUnicodeConverter conv;
        content = conv.toUnicode(file_data + 3, read_size - 3, "UTF-8");
    }
    else if (isUtf16LE) {
        NSUnicodeConverter::CUnicodeConverter conv;
        content = conv.toUnicode(file_data + 2, read_size - 2, "UTF-16LE");
    }
    else if (isUtf16BE) {
        NSUnicodeConverter::CUnicodeConverter conv;
        content = conv.toUnicode(file_data + 2, read_size - 2, "UTF-16BE");
    }
    else
    {

        NSUnicodeConverter::CUnicodeConverter conv;
        if (CodePage >= 0 && CodePage < UNICODE_CONVERTER_ENCODINGS_COUNT)
        {
            const char* encodingName = NSUnicodeConverter::Encodings[CodePage].Name;
            content = conv.toUnicode(file_data, read_size, encodingName);
        }
        else
        {
            content = conv.toUnicode(file_data, read_size, 46);
        }
    }
    delete [] file_data;
    size_t lineCount = 0;
    for (size_t i = 0; i < content.size(); ++i)
    {
        if (content[i] == L'\n' || content[i] == L'\r')
        {
            ++lineCount;
            if (content[i] == L'\r' && i + 1 < content.size() && content[i + 1] == L'\n')
                ++i;
        }
    }
    if (!content.empty() && content.back() != L'\n' && content.back() != L'\r')
        ++lineCount;
    result.reserve(lineCount);
    std::wstring line;
    for (size_t i = 0; i < content.size(); ++i)
    {
        wchar_t wc = content[i];
        if (wc == L'\n' || wc == L'\r')
        {
            result.push_back(std::move(line));
            line.clear();
            if (wc == L'\r' && i + 1 < content.size() && content[i + 1] == L'\n')
                ++i;
        }
        else
        {
            line.push_back(wc);
        }
    }
    if (!line.empty())
        result.push_back(std::move(line));
    m_linesCount = result.size();
    return result;
}

const std::vector<std::string> TxtFile::readAnsiOrCodePage() // == readUtf8withoutPref также
{
    std::vector<std::string> result;
	NSFile::CFileBinary file_binary;

	if (file_binary.OpenFile(m_path) == false) return result;

	DWORD file_size	= file_binary.GetFileSize();
	char *file_data	= new char[file_size];

	if (file_data == NULL) return result;

	file_binary.ReadFile((BYTE*)file_data, file_size, file_size);

	DWORD start_pos = 0;

	for (DWORD end_pos = 0; end_pos < file_size; end_pos++)
	{
		BYTE cCurChar = file_data[end_pos];
		if (0x0a == cCurChar || 0x0d == cCurChar)
		{
			//string from start_pos to end_pos
			std::string str(file_data + start_pos, file_data + end_pos);
			
			start_pos = end_pos + 1;
			if(0x0d == cCurChar && start_pos < file_size && 0x0a == file_data[start_pos])
			{
				end_pos++;
				start_pos++;
			}

			result.push_back(str);
			m_linesCount++;
		}
	}//last
	std::string str(file_data + start_pos, file_data + file_size);
	result.push_back(str);
	m_linesCount++;
	return result;
}

const std::vector<std::wstring> TxtFile::readUnicodeFromBytes(char *file_data, long file_size)
{
    std::vector<std::wstring> result;
	if (file_size < 3) return result;

    long start_pos = 2;	// skip Header

    for (long end_pos = start_pos; end_pos + 1 < file_size; end_pos += 2)
    {
        BYTE cCurChar = file_data[end_pos];
        if (0x00 == file_data[end_pos + 1] && (0x0a == cCurChar || 0x0d == cCurChar))
        {
            result.push_back(NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)(file_data + start_pos), (end_pos - start_pos) / 2));
            start_pos = end_pos + 2;
            if(0x0d == cCurChar && start_pos + 1 < file_size && 0x00 == file_data[start_pos + 1] && 0x0a == file_data[start_pos])
            {
                end_pos += 2;
                start_pos += 2;
            }
            m_linesCount++;
        }
    }
    //last
    result.push_back(NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)(file_data + start_pos), (file_size - start_pos) / 2));
    m_linesCount++;
    return result;
}

const std::vector<std::wstring> TxtFile::readUnicode()
{
    std::vector<std::wstring> result;
	NSFile::CFileBinary file_binary;

	if (file_binary.OpenFile(m_path) == false ) return result;

	DWORD file_size	= file_binary.GetFileSize();
	char *file_data	= new char[file_size];

	if (file_data == NULL) return result;

	file_binary.ReadFile((BYTE*)file_data, file_size, file_size);

    return readUnicodeFromBytes(file_data, file_size);
}

const std::vector<std::wstring> TxtFile::readBigEndian()
{
    std::vector<std::wstring> result;
	NSFile::CFileBinary file_binary;

	if (file_binary.OpenFile(m_path) == false) return result;

	DWORD file_size	= file_binary.GetFileSize();
	char *file_data	= new char[file_size];

	if (file_data == NULL) return result;

	file_binary.ReadFile((BYTE*)file_data, file_size, file_size);

    //swap bytes
	DWORD file_size_round = (file_size / 2) * 2;
    for (DWORD i = 0; i < file_size_round; i+=2)
    {
        char v			= file_data[i];
        file_data[i]	= file_data[i+1];
        file_data[i+1]	= v;
    }
    return readUnicodeFromBytes(file_data, file_size);
}


const std::vector<std::string> TxtFile::readUtf8()
{
    std::vector<std::string> result;
	NSFile::CFileBinary file_binary;

	if (file_binary.OpenFile(m_path) == false) return result;

	DWORD file_size	= file_binary.GetFileSize();
	char *file_data	= new char[file_size];

	if (file_data == NULL) return result;

	file_binary.ReadFile((BYTE*)file_data, file_size, file_size);

	DWORD start_pos = 3; //skip header

    for (DWORD end_pos = start_pos; end_pos < file_size; end_pos++)
	{
		BYTE cCurChar = file_data[end_pos];
		if (0x0a == cCurChar || 0x0d == cCurChar)
		{
			//string from start_pos to end_pos
			std::string str(file_data + start_pos, file_data + end_pos);
			
			start_pos = end_pos + 1;
			if(0x0d == cCurChar && start_pos < file_size && 0x0a == file_data[start_pos])
			{
				end_pos++;
				start_pos++;
			}
			result.push_back(str);
			m_linesCount++;
		}
	}//last
	std::string str(file_data + start_pos, file_data + file_size);
	result.push_back(str);
	m_linesCount++;
	return result;
}

bool TxtFile::writeAnsiOrCodePage(const std::vector<std::string>& content) // === writeUtf8withoutPref также
{
	NSFile::CFileBinary file;
	if (!file.CreateFileW(m_path)) return false;

	BYTE endLine[2] = {0x0d, 0x0a};
	for (std::vector<std::string>::const_iterator iter = content.begin(); iter != content.end(); ++iter)
	{
		file.WriteFile((BYTE*)(*iter).c_str(), (*iter).length());
		file.WriteFile(endLine, 2);

		m_linesCount++;
	}
	return true;
}

bool TxtFile::writeUnicode(const std::vector<std::wstring>& content)
{
	NSFile::CFileBinary file;
	if (!file.CreateFileW(m_path)) return false;

	BYTE Header[2]	= {0xff, 0xfe};
	BYTE EndLine[4] = { 0x0d, 0x00, 0x0a, 0x00 };

	file.WriteFile(Header, 2);

	for (std::vector<std::wstring>::const_iterator iter = content.begin(); iter != content.end(); ++iter)
	{
		const wchar_t *	data = (*iter).c_str();
		int				size = (*iter).length();

		if (sizeof(wchar_t) == 2)
		{
			file.WriteFile((BYTE*)data, size << 1);
		}
		else
		{
			//convert Utf 32 to Utf 16
		}

		file.WriteFile(EndLine, 4);
		m_linesCount++;
	}
	file.CloseFile();
	return true;
}

bool TxtFile::writeBigEndian(const std::vector<std::wstring>& content)
{
	NSFile::CFileBinary file;
	if (!file.CreateFileW(m_path)) return false;

	BYTE Header[2]	= {0xfe,	0xff};
	BYTE EndLine[4] = { 0x00, 0x0d, 0x00, 0x0a };

	file.WriteFile(Header, 2);

	for (std::vector<std::wstring>::const_iterator iter = content.begin(); iter != content.end(); ++iter)
	{
		if (sizeof(wchar_t) == 2)
		{
			BYTE*	data = (BYTE*)(*iter).c_str();
			int		size = (*iter).length();
			//swap bytes
			for (long i = 0; i < size << 1; i += 2)
			{
				char v = data[i];
				data[i] = data[i + 1];
				data[i + 1] = v;
			}
			file.WriteFile((BYTE*)(*iter).c_str(), size << 1);
		}
		else
		{
			//convert Utf 32 to Utf 16
		}

		file.WriteFile(EndLine, 4);
		m_linesCount++;
	}
	file.CloseFile();	
	return true;
}

bool TxtFile::writeUtf8(const std::vector<std::string>& content)
{
	NSFile::CFileBinary file;
	if (!file.CreateFileW(m_path)) return false;

	BYTE Header[3] = { 0xef ,0xbb , 0xbf };
	BYTE EndLine[2] = { 0x0d ,0x0a };

	file.WriteFile(Header, 3);

	for (std::vector<std::string>::const_iterator iter = content.begin(); iter != content.end(); ++iter)
	{
		file.WriteFile((BYTE*)(*iter).c_str(), (*iter).length());
        if (iter + 1 != content.end())
        {
            file.WriteFile((BYTE*)EndLine, 2);
        }

		m_linesCount++;
	}
	file.CloseFile();
	return true;
}

const bool TxtFile::isUnicode()
{
	NSFile::CFileBinary file;

	if (file.OpenFile(m_path) == false) return false;

	DWORD dwRead;
	BYTE data [2];
	
	file.ReadFile(data, 2, dwRead);
	file.CloseFile();

	if ((data [0] == 0xff) && (data [1] == 0xfe))return true;

	return false;
}

const bool TxtFile::isBigEndian()
{
	NSFile::CFileBinary file;

	if (file.OpenFile(m_path) == false) return false;

	DWORD dwRead;
	BYTE data [2];
	
	file.ReadFile(data, 2, dwRead);
	file.CloseFile();

	if ((data [0] == 0xfe) && (data [1] == 0xff))return true;

	return false;
}

const bool TxtFile::isUtf8()
{
	NSFile::CFileBinary file;

	if (file.OpenFile(m_path) == false) return false;

	DWORD dwRead;
	BYTE data [3];
	
	file.ReadFile(data, 3, dwRead);
	file.CloseFile();

	if ((data [0] == 0xef) && (data [1] == 0xbb) && (data [2] == 0xbf))return true;

	return false;
}
