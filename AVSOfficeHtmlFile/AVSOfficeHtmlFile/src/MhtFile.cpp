#include "stdafx.h"
#include "MhtFile.h"
#include "Utility.h"
#include "strings.h"
#include "path.h"
#include "CHtmlBitmap.h"
#include "ToString.h"
#include "utf8.h"
#include "gdiplus.h"

namespace Names
{
const std::wstring boundary_str				   = L"boundary=";
const std::wstring contentType_str			   = L"content-type:";
const std::wstring contentTransferEncoding_str = L"content-transfer-encoding:";
const std::wstring contentLocation_str		   = L"content-location:";

const std::wstring htmlFileType				   = L"text/html";
const std::wstring xmlFileType				   = L"text/xml";
const std::wstring cssFileType				   = L"text/css";
const std::wstring imageFileType			   = L"image/";

const std::wstring code_8bit				   = L"8bit";
const std::wstring code_QuotedPrintable		   = L"quoted-printable";
const std::wstring code_Base64				   = L"base64";
}

MhtFile::MhtFile()    
{

}


MhtFile::~MhtFile()
{

}


void MhtFile::ReadFile(const std::wstring& fileName)
{	
	TxtFile file(fileName);
	std::list<std::wstring> content;

	if (file.isUtf8())
		content = transform(file.readUtf8(), Encoding::utf82unicode);
	else if (file.isUnicode())
		content = file.readUnicode();
	else
		content = transform(file.readAnsi(), Encoding::ansi2unicode);

	content.push_back( std::wstring( L"" ) );
	content.push_back( std::wstring( L"1" ) );
	std::wstring boundary;
	std::wstring doc_location;

	int p = 0;
	//пробегаемся по строкам файла MHT
	for(std::list<std::wstring>::iterator i = content.begin(); i != content.end();) 
	{					
		//Ищем инициализацию boundary в шапке документа MHT(boundary - разделитель внутренних файлов) - обязательный параметр 
		if (ToLower(*i).find(L"boundary") != std::wstring::npos)
			boundary = L"--" + strings::remove_all(strings::substring_after( *i++, Names::boundary_str), L"\"");
		//Ищем инициализацию contentLocation в шапке(наименование главного внутренний файла) - может отсутствовать
		else if (ToLower(*i).find(Names::contentLocation_str) != std::wstring::npos)
				doc_location = getFileProperty(*i++, Names::contentLocation_str);
		//если встретили разделитель, то начинаем считывать новый внутренний файл
		else if (*i == boundary && boundary != L"")
		{
			std::wstring fileType;
			std::wstring location;
			std::wstring fileCode;

			//сначала считывается шапка внутреннего файла, которая отделяется от основного текста файлом как минимум одной пустой строкой 
			while ( i->length() != 0 )
			{
				std::wstring wsLower = ToLower(*i);

				// Проверяем, возможно разделитель поменялся с данного места
				if (wsLower.find(L"boundary") != std::wstring::npos)
					boundary = L"--" + strings::remove_all(strings::substring_after( *i++, Names::boundary_str), L"\"");

				//тип файла (image/, text/html, text/css)
				if (wsLower.find(Names::contentType_str) != std::wstring::npos)
					fileType = getFileProperty(*i, Names::contentType_str);
				//наименование файла
				else if (wsLower.find(Names::contentLocation_str) != std::wstring::npos)
					location = getFileProperty(*i, Names::contentLocation_str);
				//кодировка (base64, 8bit, quoted-printable)
				else if (wsLower.find(Names::contentTransferEncoding_str) != std::wstring::npos)
					fileCode = getFileProperty(*i, Names::contentTransferEncoding_str);	
				i++;
			}			
			while ( i->length() == 0)
				i++;

			std::vector<std::string> file;
			std::wstring new_file;

			//если кодировка base64 - смотрим на тип файла
			if (fileCode.find(Names::code_Base64) != std::wstring::npos)
			{
				if (fileType.find(Names::htmlFileType) != std::wstring::npos || fileType.find(Names::cssFileType) != std::wstring::npos)
				{
					std::wstring fileBase64;
					while (  i != content.end() && i->find(boundary) == std::wstring::npos )
						fileBase64 += *i++;
					new_file = base64To8bit(fileBase64);
				}
				else if (fileType.find(Names::imageFileType) != std::wstring::npos)
				{
					while (  i != content.end() && i->find(boundary) == std::wstring::npos )
						new_file += *i++;
				}
			}
			else
			{
				if (fileCode.find(Names::code_8bit) != std::wstring::npos)
				{
					while (  i != content.end() && i->find(boundary) == std::wstring::npos )				
						file.push_back(Encoding::unicode2ansi(*i++));
				}
				else if (fileCode.find(Names::code_QuotedPrintable) != std::wstring::npos)
				{
					while (  i != content.end() && i->find(boundary) == std::wstring::npos )
						file.push_back(encodingQuotedPrintable(Encoding::unicode2ansi(*i++)));
				}
				new_file = file2ansi(file);
			}

			if ( fileType.find(Names::htmlFileType) != std::wstring::npos || fileType.find(Names::xmlFileType) != std::wstring::npos )	
			{
				if (doc_location == location || doc_location == L"")
					m_content = new_file;
			}
			else if ( fileType.find(Names::cssFileType) != std::wstring::npos )
				m_styles.insert(std::make_pair(set_slashes(location), new_file));
			else if ( fileType.find(Names::imageFileType) != std::wstring::npos )
				m_pictures.insert(std::make_pair(set_slashes(location), new_file));				
		}
		else
			i++;
	}
}


bool MhtFile::savePicture(const std::wstring& name, const std::wstring& path)const
{
	std::wstring picture = getPicture(name);
	if (picture != L"")
	{
		CString pic_cstr = picture.c_str();
		CString name_cstr = name.c_str();//TODO извлечь только имя
		CString path_cstr = path.c_str();

		CHtmlBitmap bitmap;
		bitmap.Create(pic_cstr, name_cstr, path_cstr);
		bitmap.WriteFile();
		return true;
	}
	return false;
}


const std::wstring MhtFile::getStyle(const std::wstring& name) const
{
	std::wstring new_name = set_slashes(ToLower(name));
	std::wstring file = L"";
	for(std::map<std::wstring, std::wstring>::const_iterator i = m_styles.begin(); i != m_styles.end(); ++i)	
		if ( i->first.find(new_name) != std::wstring::npos)
			return  file = i->second;
	return file;
}


const std::wstring MhtFile::getHtmlFile() const
{
	return m_content;
}


const std::wstring MhtFile::getPicture(const std::wstring& name) const
{
	std::wstring new_name = set_slashes(ToLower(name));
	std::wstring file = L"";
	for(std::map<std::wstring, std::wstring>::const_iterator i = m_pictures.begin(); i != m_pictures.end(); ++i)	
		if ( i->first.find(new_name) != std::wstring::npos)
			return  file = i->second;
	return file;
}


const std::wstring MhtFile::getFileProperty(const std::wstring& line, const std::wstring& propertyName) const
{
	std::wstring fileProperty;
	std::wstring lowerLine = ToLower(line);
	if (lowerLine.find(L";") != std::wstring::npos)
		fileProperty = strings::substring_between( lowerLine, propertyName, L";");
	else
		fileProperty = strings::substring_after( lowerLine, propertyName);

	return fileProperty;
}


const std::wstring MhtFile::base64To8bit(const std::wstring& fileBase64) const
{	
	BYTE* bytes = NULL;
	int size;
	CHtmlUtility::GetFileInBinary(fileBase64.c_str(), bytes, size);
	char * str = new char[size];
	::memcpy(str, bytes, size);
	std::wstring wstr = Encoding::ansi2unicode(str);
	delete []str;
	delete []bytes;
	return wstr;
}


const std::string MhtFile::encodingQuotedPrintable(const std::string& line) const 
{
	if (line.length() != 0)
	{
		char * new_line = new char[line.length() * 3];

		int j = 0;
		for (int i = 0; i < line.length(); i++)
		{
			if (line[i] != L'=')
			{
				new_line[j++] = line[i];
			}
			else
			{				
				std::wstring hex = strings::substring_between( Encoding::ansi2unicode(line), i + 1, i + 2);
				if (hex != L"")
					new_line[j++] = strings::strFromHex(hex);
				i += 2;
			}
		}
		new_line[j] = '\0';

		std::string result(new_line);
		if (new_line != 0)
			delete []new_line;

		return result;
	}

	std::string p;
	return p;
}


const std::wstring MhtFile::file2ansi(const std::vector<std::string>& file) const
{
	std::wstring ansiFile;

	std::string code;
	for(std::vector<std::string>::const_iterator i = file.begin(); i != file.end(); )
	{
		std::string lowerLine = ToLower(*i);
		std::string::size_type meta = lowerLine.find("<meta");
		std::string::size_type xmlEncode = lowerLine.find("<?xml");
		if (xmlEncode != std::string::npos )
		{
			std::string xmlTeg = lowerLine.substr(xmlEncode + 5, i->length() - xmlEncode - 5);
			std::string::size_type ugl = xmlTeg.find(">");
			while( ugl == std::string::npos)
			{
				xmlTeg += ToLower(*i++);
				ugl = xmlTeg.find(">");
			}
			xmlTeg = xmlTeg.substr(0, ugl - 1);
			std::string::size_type encoding = xmlTeg.find("encoding=");
			if (encoding != std::string::npos)
			{
				code = xmlTeg.substr(encoding + 10, xmlTeg.length() - encoding - 10);
				break;
			}
			i++;
		}
		else if (meta != std::string::npos)
		{
			std::string metaTeg = lowerLine.substr(meta + 5, i->length() - meta - 5);
			std::string::size_type ugl = metaTeg.find(">");
			while( ugl == std::string::npos)
			{
				metaTeg += ToLower(*i++);
				ugl = metaTeg.find(">");
			}
			metaTeg = metaTeg.substr(0, ugl - 1);
			std::string::size_type charset = metaTeg.find("charset=");
			if (charset != std::string::npos)
			{
				code = metaTeg.substr(charset + 8, metaTeg.length() - charset - 8);
			}
			break;
		}
		else
		{
			i++;
		}
	}


	
	if ( code.find("utf-8") != std::string::npos)
	{
		std::string str;
		str.push_back(strings::strFromHex("0xEF"));
		str.push_back(strings::strFromHex("0xBB"));
		str.push_back(strings::strFromHex("0xBF"));

		std::string all_text;
		BOOST_FOREACH(const std::string& line, file)
		{			
			all_text += line;
		}
		std::string text_without_pref = strings::remove(all_text, str);
		std::wstring new_text(text_without_pref.size(), 0);
		utf8_decode( text_without_pref.begin(), text_without_pref.end(), new_text.begin() );

		ansiFile = new_text;			
	}
	else
	{
		BOOST_FOREACH(const std::string& line, file)
		{
			ansiFile += Encoding::ansi2unicode(line);
		}
	}

	return ansiFile;
}