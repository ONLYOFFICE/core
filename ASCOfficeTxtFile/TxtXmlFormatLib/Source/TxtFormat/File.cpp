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
#include "../Common/Utility.h"
#include "TxtFile.h"

#include "../../../DesktopEditor/common/File.h"


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

        std::vector<std::string> codePageContent	= file.readAnsiOrCodePage();
		m_listContentSize						= file.getLinesCount();

        for (std::vector<std::string>::const_iterator iter = codePageContent.begin(); iter != codePageContent.end(); ++iter)
		{
			m_listContent.push_back(Encoding::cp2unicode(*iter, code_page));
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
			m_listContent = _transform(file.readUtf8(), Encoding::utf82unicode);
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
            if (-1 == m_nEncoding)
                m_listContent = _transform(file.readAnsiOrCodePage(), Encoding::utf82unicode);
			else if (1000 == m_nEncoding)
                m_listContent = _transform(file.readAnsiOrCodePage(), Encoding::ansi2unicode);
			else
                m_listContent = _transform2(file.readAnsiOrCodePage(), m_nEncoding, Encoding::cp2unicode);
		}
		
		m_listContentSize = file.getLinesCount();
		
		//correctUnicode(listContentUnicode); - ВЫТИРАЕТ ПРОБЕЛЫ  в конце строки (
	}


    void File::write(const std::wstring& filename) const
	{
        TxtFile file(filename);
		file.writeUtf8(_transform(m_listContent, Encoding::unicode2utf8));
	}

    void File::writeCodePage(const std::wstring& filename, int code_page) const
	{
        TxtFile file(filename);
		
        std::vector<std::string> result;
        for (std::vector<std::wstring>::const_iterator iter = m_listContent.begin(); iter != m_listContent.end(); ++iter)
		{
			result.push_back(Encoding::unicode2cp(*iter,code_page));
		}

		file.writeAnsiOrCodePage(result);
	} 

    void File::writeUtf8(const std::wstring& filename) const
	{
        TxtFile file(filename);
		file.writeUtf8(_transform(m_listContent, Encoding::unicode2utf8));
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
		file.writeAnsiOrCodePage(_transform(m_listContent, Encoding::unicode2ansi));
	}
		

    const bool File::isValid(const std::wstring& filename) const
	{
		if (filename.empty())
			return true;
        return NSFile::CFileBinary::Exists(filename);
	}
    void File::correctUnicode(std::vector<std::wstring>& input)
	{
        for(std::vector<std::wstring>::iterator iter = input.begin(); iter != input.end(); iter++)
		{
			const std::wstring& inputStr = *iter;
			std::wstring outputStr;
			outputStr.reserve(inputStr.length());
			
			for(int i = 0, length = inputStr.length(); i < length; ++i)
			{
				wchar_t inputChr = inputStr[i];
				if(IsUnicodeSymbol(inputChr))
					outputStr.push_back(inputChr);
			}
			*iter = outputStr;
		}
	}
	bool File::IsUnicodeSymbol( wchar_t symbol )
	{
		bool result = false;

		if ( ( 0x0009 == symbol ) || ( 0x000A == symbol ) || ( 0x000D == symbol ) ||
			( ( 0x0020 <= symbol ) && ( 0xD7FF >= symbol ) ) || ( ( 0xE000 <= symbol ) && ( symbol <= 0xFFFD ) ) ||
			( ( 0x10000 <= symbol ) && symbol ) )
		{
			result = true;
		}

		return result;		  
	}
} // namespace Txt
