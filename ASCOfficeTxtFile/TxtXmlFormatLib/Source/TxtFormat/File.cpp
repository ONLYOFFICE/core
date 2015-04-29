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

        TxtFile file(std_string2string(filename));

		std::list<std::string> codePageContent	= file.readAnsiOrCodePage();	
		m_listContentSize						= file.getLinesCount();

		for (std::list<std::string>::const_iterator iter = codePageContent.begin(); iter != codePageContent.end(); ++iter)
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

        TxtFile file(std_string2string(filename));
		
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
			m_listContent = _transform(file.readAnsiOrCodePage(), Encoding::utf82unicode);
		}
		
		m_listContentSize = file.getLinesCount();
		
		//correctUnicode(listContentUnicode); - ВЫТИРАЕТ ПРОБЕЛЫ  в конце строки (
	}


    void File::write(const std::wstring& filename) const
	{
        TxtFile file(std_string2string(filename));
		file.writeUtf8(_transform(m_listContent, Encoding::unicode2utf8));
	}

    void File::writeCodePage(const std::wstring& filename, int code_page) const
	{
        TxtFile file(std_string2string(filename));
		
		std::list<std::string> result;
		for (std::list<std::wstring>::const_iterator iter = m_listContent.begin(); iter != m_listContent.end(); ++iter)
		{
			result.push_back(Encoding::unicode2cp(*iter,code_page));
		}

		file.writeAnsiOrCodePage(result);
	} 

    void File::writeUtf8(const std::wstring& filename) const
	{
        TxtFile file(std_string2string(filename));
		file.writeUtf8(_transform(m_listContent, Encoding::unicode2utf8));
	}


    void File::writeUnicode(const std::wstring& filename) const
	{
        TxtFile file(std_string2string(filename));
		file.writeUnicode(m_listContent);
	}


    void File::writeBigEndian(const std::wstring& filename) const
	{
        OOX::CPath path (filename);
        TxtFile file(path);
		file.writeBigEndian(m_listContent);
	}


    void File::writeAnsi(const std::wstring& filename) const
	{
        TxtFile file(std_string2string(filename));
		file.writeAnsiOrCodePage(_transform(m_listContent, Encoding::unicode2ansi));
	}
		

    const bool File::isValid(const std::wstring& filename) const
	{
		if (filename.empty())
			return true;
        return NSFile::CFileBinary::Exists(filename);
	}
	void File::correctUnicode(std::list<std::wstring>& input)
	{
		for(std::list<std::wstring>::iterator iter = input.begin(); iter != input.end(); iter++)
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
