#include "File.h"
#include "Utility.h"


namespace Txt
{

	File::File()
	{
	}


	void File::read(const boost::filesystem::wpath& filename)
	{
		if (filename.empty())
			return;

		TxtFile file(filename);
		//читаем юникод чтобы можно было выкинуть невалидные символы
		std::list<std::wstring> listContentUnicode;
		if (file.isUtf8())
			listContentUnicode = transform(file.readUtf8(), Encoding::utf82unicode);
		else if (file.isUnicode())
			listContentUnicode = file.readUnicode();
		else if (file.isBigEndian())
			listContentUnicode = file.readBigEndian();
		//проверка убрана, потому что она работает в редких случаюх: если в первой строке есть английские символы
		//далее не делается проверка BigEndian или LittleEndian
		//notepad++ открывает такие файлы как ansi и мы будем также.
		//else if (file.isUnicodeWithOutBOM())
		//	listContentUnicode = file.readUnicodeWithOutBOM();
		else
		{
			if(-1 == m_nEncoding)
				listContentUnicode = transform(file.readAnsi(), Encoding::ansi2unicode);
			else
				listContentUnicode = transform2(file.readAnsi(), m_nEncoding, Encoding::cp2unicode);
		}
		listContentUnicode = correctUnicode(listContentUnicode);
		//переводим в utf-8 чтобы не нарушать схему
		Content = transform(listContentUnicode, Encoding::unicode2utf8);
	}


	void File::write(const boost::filesystem::wpath& filename) const
	{
		TxtFile file(filename);
		file.writeUtf8(Content);
	}


	void File::writeUtf8(const boost::filesystem::wpath& filename) const
	{
		TxtFile file(filename);
		file.writeUtf8(Content);
	}


	void File::writeUnicode(const boost::filesystem::wpath& filename) const
	{
		TxtFile file(filename);
		file.writeUnicode(transform(*Content, Encoding::utf82unicode));
	}


	void File::writeBigEndian(const boost::filesystem::wpath& filename) const
	{
		TxtFile file(filename);
		file.writeBigEndian(transform(*Content, Encoding::utf82unicode));
	}


	void File::writeAnsi(const boost::filesystem::wpath& filename) const
	{
		TxtFile file(filename);
		file.writeAnsi(transform(*Content, Encoding::utf82ansi));
	}
		

	const bool File::isValid(const boost::filesystem::wpath& filename) const
	{
		if (filename.empty())
			return true;
		return boost::filesystem::exists(filename);
	}
	std::list<std::wstring> File::correctUnicode(std::list<std::wstring>& input)
	{
		std::list<std::wstring> output;
		for(std::list<std::wstring>::const_iterator iter = input.begin(); iter != input.end(); iter++)
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
			output.push_back(outputStr);
		}
		return output;
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