
#include "TxtFile.h"

#include "../Common/Encoding.h"
#include "../../../../Common/DocxFormat/Source/SystemUtility/File.h"


static const std::string BadSymbols = "\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19";


static std::wstring convertUtf16ToWString(UTF16 * Data, int nLength)
{
    UTF32 *pStrUtf32 = new UTF32 [nLength + 1];
    memset ((void *) pStrUtf32, 0, sizeof (UTF32) * (nLength + 1));

    // this values will be modificated
    const UTF16 *pStrUtf16_Conv = Data;
    UTF32 *pStrUtf32_Conv = pStrUtf32;

    ConversionResult eUnicodeConversionResult =
            ConvertUTF16toUTF32 (&pStrUtf16_Conv,
                                 &Data[nLength]
            , &pStrUtf32_Conv
            , &pStrUtf32 [nLength]
            , strictConversion);

    if (conversionOK != eUnicodeConversionResult)
    {
        delete [] pStrUtf32;
		return std::wstring();
    }

    std::wstring wstr ((wchar_t *) pStrUtf32);

    delete [] pStrUtf32;
    return wstr;
}

TxtFile::TxtFile(const std::wstring & path) : m_path(path), m_linesCount(0)
{
}
const int TxtFile::getLinesCount()
{
	return m_linesCount;
}
const std::list<std::string> TxtFile::readAnsiOrCodePage() // == readUtf8withoutPref также
{
	std::list<std::string> result;
	CFile file_binary;

	if (file_binary.OpenFile(std_string2string(m_path)) != S_OK) return result;

	long file_size	= file_binary.GetFileSize();
	char *file_data	= new char[file_size];

	if (file_data == NULL) return result;

	file_binary.ReadFile((BYTE*)file_data, file_size);

	long start_pos = 0;

	for (long end_pos = 0; end_pos < file_size; end_pos++)
	{
		if (file_data[end_pos] == 0x0a)
		{
			//string from start_pos to end_pos
			std::string str(file_data + start_pos, file_data + end_pos);
			
			start_pos = end_pos + 1;

			result.push_back(str);
			m_linesCount++;
		}
	}//last
	std::string str(file_data + start_pos, file_data + file_size);
	result.push_back(str);
	m_linesCount++;
	return result;
}


const std::list<std::wstring> TxtFile::readUnicode()
{
	std::list<std::wstring> result;
	CFile file_binary;

	if (file_binary.OpenFile(std_string2string(m_path)) != S_OK) return result;

	long file_size	= file_binary.GetFileSize();
	char *file_data	= new char[file_size];

	if (file_data == NULL) return result;

	file_binary.ReadFile((BYTE*)file_data, file_size);

	long start_pos = 2;	// skip Header

	for (long end_pos = 2; end_pos < file_size; end_pos+=2)
	{
		if ((((UTF16*)(file_data+end_pos))[0] == 0x000a) &&
			((UTF16*)(file_data+end_pos))[1] == 0x000d)
		{
			if (sizeof(wchar_t) == 4)
			{
				result.push_back(convertUtf16ToWString((UTF16*)(file_data + start_pos), (end_pos-start_pos) /2));
			}
			else
			{
				std::wstring wstr((wchar_t*)(file_data + start_pos), (wchar_t*)(file_data + end_pos));
				result.push_back(wstr);
			}
			start_pos = end_pos + 4;
			m_linesCount++;

		}
	}
//last
	if (sizeof(wchar_t) == 4)
	{
		result.push_back(convertUtf16ToWString((UTF16*)(file_data + start_pos), (file_size-start_pos) /2));
	}
	else
	{
		std::wstring wstr(file_data + start_pos, file_data + file_size);
		result.push_back(wstr);
	}
	m_linesCount++;
	return result;
}

const std::list<std::wstring> TxtFile::readBigEndian()
{
	std::list<std::wstring> result;
	CFile file_binary;

	if (file_binary.OpenFile(std_string2string(m_path)) != S_OK) return result;

	long file_size	= file_binary.GetFileSize();
	char *file_data	= new char[file_size];

	if (file_data == NULL) return result;

	file_binary.ReadFile((BYTE*)file_data, file_size);

	long start_pos = 2;	// skip Header

	for (long end_pos = 2; end_pos < file_size; end_pos+=2)
	{
		if (((UTF16*)(file_data+end_pos))[0] == 0x000d &&
			((UTF16*)(file_data+end_pos))[1] == 0x000a)
		{
			//swap bytes
			for (long i = start_pos; i < end_pos; i+=2)
			{
				char v			= file_data[i];
				file_data[i]	= file_data[i+1];
				file_data[i+1]	= v;
			}

			if (sizeof(wchar_t) == 4)
			{
				result.push_back(convertUtf16ToWString((UTF16*)(file_data + start_pos), (end_pos-start_pos) /2));
			}
			else
			{
				std::wstring wstr((wchar_t*)(file_data + start_pos), (wchar_t*)(file_data + end_pos));
				result.push_back(wstr);
			}
			start_pos = end_pos + 4;
			m_linesCount++;

		}
	}
//last
	//swap bytes
	for (long i = start_pos; i < file_size; i+=2)
	{
		char v			= file_data[i];
		file_data[i]	= file_data[i+1];
		file_data[i+1]	= v;
	}
	if (sizeof(wchar_t) == 4)
	{
		result.push_back(convertUtf16ToWString((UTF16*)(file_data + start_pos), (file_size-start_pos) /2));
	}
	else
	{
		std::wstring wstr(file_data + start_pos, file_data + file_size);
		result.push_back(wstr);
	}
	m_linesCount++;
	return result;
}


const std::list<std::string> TxtFile::readUtf8()
{
	std::list<std::string> result;
	CFile file_binary;

	if (file_binary.OpenFile(std_string2string(m_path)) != S_OK) return result;

	long file_size	= file_binary.GetFileSize();
	char *file_data	= new char[file_size];

	if (file_data == NULL) return result;

	file_binary.ReadFile((BYTE*)file_data, file_size);

	long start_pos = 3; //skip header

	for (long end_pos = 3; end_pos < file_size; end_pos++)
	{
		if (file_data[end_pos] == 0x0a)
		{
			//string from start_pos to end_pos
			std::string str(file_data + start_pos, file_data + end_pos);
			
			start_pos = end_pos + 1;

			result.push_back(str);
			m_linesCount++;
		}
	}//last
	std::string str(file_data + start_pos, file_data + file_size);
	result.push_back(str);
	m_linesCount++;
	return result;
}

void TxtFile::writeAnsiOrCodePage(const std::list<std::string>& content) // === writeUtf8withoutPref также
{
	CFile file;
    if (file.CreateFile(std_string2string(m_path)) == S_OK)
	{
		BYTE endLine[2] = {0x0d, 0x0a};
		for (std::list<std::string>::const_iterator iter = content.begin(); iter != content.end(); ++iter)	
		{
			file.WriteFile((void*)(*iter).c_str(), (*iter).length());
			file.WriteFile(endLine, 2);

			m_linesCount++;
		}
	}
}

void TxtFile::writeUnicode(const std::list<std::wstring>& content)
{
	CFile file;
    if (file.CreateFile(std_string2string(m_path)) == S_OK)
	{
		BYTE Header[2]	= {0xff, 0xfe};
		BYTE EndLine[4] = {0x0d, 0x00, 0x0a, 0x00};
		
		file.WriteFile(Header,2);

		for (std::list<std::wstring>::const_iterator iter = content.begin(); iter != content.end(); ++iter)	
		{
			const wchar_t *	data = (*iter).c_str();
			int				size = (*iter).length();

			if(sizeof(wchar_t) == 2)
			{
				file.WriteFile((void*)data, size << 1);
			}
			else
			{
				//convert Utf 32 to Utf 16
			}

			file.WriteFile(EndLine, 4);
			m_linesCount++;
		}
	}	
}

void TxtFile::writeBigEndian(const std::list<std::wstring>& content)
{
	CFile file;
    if (file.CreateFile(std_string2string(m_path)) == S_OK)
	{
		BYTE Header[2]	= {0xfe,	0xff};
		BYTE EndLine[4] = {0x00, 0x0d, 0x00, 0x0a};
		
		file.WriteFile(Header,2);

		for (std::list<std::wstring>::const_iterator iter = content.begin(); iter != content.end(); ++iter)	
		{
			if(sizeof(wchar_t) == 2)
			{
				BYTE*	data = (BYTE*)(*iter).c_str();
				int		size = (*iter).length();
				//swap bytes
				for (long i = 0; i < size << 1; i+=2)
				{
					char v		= data[i];
					data[i]		= data[i+1];
					data[i+1]	= v;
				}
				file.WriteFile((void*)(*iter).c_str(), size << 1);
			}
			else
			{
				//convert Utf 32 to Utf 16
			}

			file.WriteFile(EndLine, 4);
			m_linesCount++;
		}
	}	
}

void TxtFile::writeUtf8(const std::list<std::string>& content)
{
	CFile file;
    if (file.CreateFile(std_string2string(m_path)) == S_OK)
	{
		BYTE Header[3]	= {0xef ,0xbb , 0xbf};
		BYTE EndLine[2]	= {0x0d ,0x0a};

		file.WriteFile(Header,3);

		for (std::list<std::string>::const_iterator iter = content.begin(); iter != content.end(); ++iter)	
		{
			file.WriteFile((void*)(*iter).c_str(), (*iter).length());
			file.WriteFile((void*)EndLine, 2);

			m_linesCount++;
		}
	}
}

const bool TxtFile::isUnicode()
{
	CFile file;

	if (file.OpenFile(std_string2string(m_path)) != S_OK) return false;

	BYTE data [2];
	file.ReadFile(data,2);
	file.CloseFile();

	if ((data [0] == 0xff) && (data [1] == 0xfe))return true;

	return false;
}

const bool TxtFile::isBigEndian()
{
	CFile file;

	if (file.OpenFile(std_string2string(m_path)) != S_OK) return false;

	BYTE data [2];
	file.ReadFile(data,2);
	file.CloseFile();

	if ((data [0] == 0xfe) && (data [1] == 0xff))return true;

	return false;
}

const bool TxtFile::isUtf8()
{
	CFile file;

	if (file.OpenFile(std_string2string(m_path)) != S_OK) return false;

	BYTE data [3];
	file.ReadFile(data,3);
	file.CloseFile();

	if ((data [0] == 0xef) && (data [1] == 0xbb) && (data [2] == 0xbf))return true;

	return false;
}
