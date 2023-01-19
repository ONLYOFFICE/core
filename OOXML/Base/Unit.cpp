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
#include "Unit.h"
#include <cwchar>


double Cm_To_Mm     (const double &dValue)
{
	return dValue * 10;
}

double Cm_To_Pt     (const double &dValue)
{
	return dValue * 72 / 2.54;
}

double Cm_To_Px     (const double &dValue)
{
	return dValue * 72 * 4 / 3 / 2.54;
}

double Cm_To_Inch   (const double &dValue)
{
	return dValue / 2.54;
}

double Cm_To_Dx     (const double &dValue)
{
	return dValue * 72 * 20 / 2.54;
}

double Cm_To_Sx     (const double &dValue)
{
	return dValue * 72 * 100 * 1000 / 20;
}

double Cm_To_Multi  (const double &dValue)
{
	return dValue * 72 * 20 / 2.54;
}
double Cm_To_Emu    (const double &dValue)
{
	return dValue * 360000;
}

double Mm_To_Cm     (const double &dValue)
{
	return dValue / 10;
}

double Mm_To_Pt     (const double &dValue)
{
	return dValue * 72 / 10 / 2.54;
}

double Mm_To_Px     (const double &dValue)
{
	return dValue * 72  * 4 / 3 / 10 / 2.54;
}

double Mm_To_Inch   (const double &dValue)
{
	return dValue / 2.54 / 10;
}

double Mm_To_Dx     (const double &dValue)
{
	return dValue * 72 * 20 / 10 / 2.54;
}

double Mm_To_Sx     (const double &dValue)
{
	return dValue * 72 * 100 * 1000 / 10 / 20;
}

double Mm_To_Multi  (const double &dValue)
{
	return dValue * 72 * 20 / 10 / 2.54;
}

double Mm_To_Emu    (const double &dValue)
{
	return dValue * 36000;
}

double Pt_To_Cm     (const double &dValue)
{
	return dValue * 2.54 / 72;
}

double Pt_To_Mm     (const double &dValue)
{
	return dValue * 2.54 * 10 / 72;
}

double Pt_To_Px     (const double &dValue)
{
	return dValue * 4 / 3;
}

double Pt_To_Inch   (const double &dValue)
{
	return dValue / 72;
}

double Pt_To_Dx     (const double &dValue)
{
	return dValue * 20;
}

double Pt_To_Sx     (const double &dValue)
{
	return dValue * 2.54 * 100 * 1000 / 20;
}

double Pt_To_Multi  (const double &dValue)
{
	return dValue * 20;
}

double Pt_To_Emu    (const double &dValue)
{
	return dValue * 12700;
}

double Px_To_Cm     (const double &dValue)
{
	return dValue * 2.54 * 3/ 72 / 4;
}

double Px_To_Mm     (const double &dValue)
{
	return dValue * 2.54 * 10 * 3/ 72 /4;
}

double Px_To_Pt     (const double &dValue)
{
	return dValue * 3 / 4;
}

double Px_To_Inch   (const double &dValue)
{
	return dValue * 3 / 72 / 4;
}

double Px_To_Dx     (const double &dValue)
{
	return dValue * 20 * 3 / 4;
}
double Px_To_Sx     (const double &dValue)
{
	return dValue * 2.54 * 100 * 1000 * 3/ 20 / 4;
}

double Px_To_Multi  (const double &dValue)
{
	return dValue * 20 * 3 / 4;
}

double Px_To_Emu    (const double &dValue)
{
	return dValue * 9525;
}

double Inch_To_Cm   (const double &dValue)
{
	return dValue * 2.54;
}

double Inch_To_Mm   (const double &dValue)
{
	return dValue * 2.54 * 10;
}

double Inch_To_Pt   (const double &dValue)
{
	return dValue * 72;
}

double Inch_To_Px   (const double &dValue)
{
	return dValue * 72 * 4 / 3;
}

double Inch_To_Dx   (const double &dValue)
{
	return dValue * 72 * 20;
}

double Inch_To_Sx   (const double &dValue)
{
	return dValue * 1000 * 100 * 2.54 * 72 / 20;
}

double Inch_To_Multi(const double &dValue)
{
	return dValue * 72 * 20;
}
double Inch_To_Emu  (const double &dValue)
{
	return dValue * 914400;
}

double Dx_To_Cm     (const double &dValue)
{
	return dValue * 2.54 / 72 / 20;
}

double Dx_To_Mm     (const double &dValue)
{
	return dValue * 2.54 * 10 / 72 / 20;
}

double Dx_To_Pt     (const double &dValue)
{
	return dValue / 20;
}


double Dx_To_Px     (const double &dValue)
{
	return dValue * 4 / 3 / 20;
}

double Dx_To_Inch   (const double &dValue)
{
	return dValue / 20 / 72;
}

double Dx_To_Sx     (const double &dValue)
{
	return dValue * 635;
}

double Dx_To_Multi  (const double &dValue)
{
	return dValue;
}

double Dx_To_Emu    (const double &dValue)
{
	return dValue * 635;
}

double Sx_To_Cm     (const double &dValue)
{
	return dValue * 20 / 72 / 100 / 1000;
}

double Sx_To_Mm     (const double &dValue)
{
	return dValue * 20 / 72 / 100 / 1000 * 10;
}

double Sx_To_Pt     (const double &dValue)
{
	return dValue * 20 / 100 / 1000 / 2.54;
}

double Sx_To_Px     (const double &dValue)
{
	return dValue * 20 * 4 / 3 / 100 / 1000 / 2.54;
}

double Sx_To_Inch   (const double &dValue)
{
	return dValue * 20 / 2.54 / 72 / 100 / 1000;
}

double Sx_To_Dx     (const double &dValue)
{
	return dValue * 20 * 20 / 2.54 / 100 / 1000;
}

double Sx_To_Multi  (const double &dValue)
{
	return dValue * 20 * 20 / 2.54 / 100 / 1000;
}

double Sx_To_Emu    (const double &dValue)
{
	return dValue;
}

double Multi_To_Cm  (const double &dValue)
{
	return dValue * 2.54 / 72 / 20;
}

double Multi_To_Mm  (const double &dValue)
{
	return dValue * 2.54 * 10 / 72 / 20;
}

double Multi_To_Pt  (const double &dValue)
{
	return dValue / 20;
}

double Multi_To_Px  (const double &dValue)
{
	return dValue * 4 / 3 / 20;
}

double Multi_To_Inch(const double &dValue)
{
	return dValue / 20 / 72;
}

double Multi_To_Sx  (const double &dValue)
{
	return dValue * 635;
}

double Multi_To_Dx  (const double &dValue)
{
	return dValue;
}

double Multi_To_Emu (const double &dValue)
{
	return dValue * 635;
}

double Emu_To_Cm    (const double &dValue)
{
	return dValue / 360000;
}
double Emu_To_Mm    (const double &dValue)
{
	return dValue / 36000;
}

double Emu_To_Pt    (const double &dValue)
{
	return dValue / 12700;
}

double Emu_To_Twips    (const double &dValue)
{
	return dValue / 635;
}

double Emu_To_Px    (const double &dValue)
{
	return dValue / 9525;
}

double Emu_To_Inch  (const double &dValue)
{
	return dValue / 914400;
}

double Emu_To_Sx    (const double &dValue)
{
	return dValue;
}

double Emu_To_Dx    (const double &dValue)
{
	return dValue / 635;
}

double Emu_To_Multi (const double &dValue)
{
	return dValue / 635;
}

#include <algorithm>
#include <sstream>
#include <stdio.h>

#ifdef _MSC_VER
#pragma warning (disable: 4100 4189)
#endif

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

namespace XmlUtils
{
	// common
	int GetDigit(wchar_t c)
	{
		if (c >= '0' && c <= '9')
			return (int)(c - '0');
		if (c >= 'a' && c <= 'f')
			return 10 + (int)(c - 'a');
		if (c >= 'A' && c <= 'F')
			return 10 + (int)(c - 'A');

		return 0;
	}
	int GetDigit(char c)
	{
		if (c >= '0' && c <= '9')
			return (int)(c - '0');
		if (c >= 'a' && c <= 'f')
			return 10 + (int)(c - 'a');
		if (c >= 'A' && c <= 'F')
			return 10 + (int)(c - 'A');

		return 0;
	}
	bool IsDigit(wchar_t c)
	{
		if (c >= '0' && c <= '9')
			return true;
		return false;
	}

	_INT64 GetHex(const std::wstring& string)
	{
		_INT64 nResult = 0;
		size_t nLen = string.length();
		for (size_t nIndex = 0; nIndex < nLen; ++nIndex )
		{
			nResult += GetDigit( string[nIndex] ) << ( 4 * ( nLen - 1 - nIndex ) );
		}

		return nResult;
	}
	_INT64 GetHex(const std::string& string)
	{
		_INT64 nResult = 0;
		size_t nLen = string.length();
		for (size_t nIndex = 0; nIndex < nLen; ++nIndex )
		{
			nResult += GetDigit( string[nIndex] ) << ( 4 * ( nLen - 1 - nIndex ) );
		}

		return nResult;
	}
	int	GetColorBGR(const std::wstring& string)
	{
		// variables
		int blue = 0;
		int green = 0;
		int red = 0;

		std::wstring color = string; //boost::algorithm::trim(color);

		if (color.find(L"0x") != -1)    color.erase(0, 2);
		if (color.find(L"#") != -1)     color.erase(0, 1);

		while (color.length() < 6)
			color = L"0" + color;

		red		= 16*GetDigit(color[0]) + GetDigit(color[1]);
		green	= 16*GetDigit(color[2]) + GetDigit(color[3]);
		blue	= 16*GetDigit(color[4]) + GetDigit(color[5]);

		return ((int)(((BYTE)(red)|((WORD)((BYTE)(green))<<8))|(((DWORD)(BYTE)(blue))<<16)));	//RGB(red, green, blue);
	}

	std::wstring GetLower(const std::wstring& string)
	{
		std::wstring sResult;

		for( size_t nIndex = 0; nIndex < string.length(); nIndex++)
			sResult += wchar_t( towlower(string[nIndex]) );

		return sResult;
	}
	std::wstring GetUpper(const std::wstring& string)
	{
		std::wstring sResult;

		for( size_t nIndex = 0; nIndex < string.length(); nIndex++)
			sResult += wchar_t( towupper(string[nIndex]) );

		return sResult;
	}

	void replace_all(std::wstring& subject, const std::wstring& search, const std::wstring& replace)
	{
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::wstring::npos)
		{
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
	}
	void replace_all(std::string& subject, const std::string& search, const std::string& replace)
	{
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos)
		{
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
	}

	bool GetBoolean (const std::wstring& string)
	{
		std::wstring s = XmlUtils::GetLower(string);

		return (s == L"true");
	}
	bool GetBoolean2(const std::wstring& string)
	{
		std::wstring sTemp = XmlUtils::GetLower(string);

		return ( L"true" == sTemp || L"1" == sTemp || L"t" == sTemp || L"on" == sTemp );
	}
	_INT64 GetInteger64(const std::wstring& string)
	{
		if (string.empty()) return 0;

		try
		{
            return std::wcstoll(string.c_str(), NULL, 10);
		}
		catch(...)
		{
			return 0;
		}
	}
	int GetInteger(const std::wstring& string)
	{
		if (string.empty()) return 0;

        int iVal = 0;
		try
		{
            iVal = std::stod(string);
		}
		catch(...)
		{
            try
            {
                iVal = std::wcstoll(string.c_str(), NULL, 10);
            }
            catch(...)
            {
                return 0;
            }
		}

        return iVal;
	}
	unsigned int GetUInteger(const std::wstring& string)
	{
        return (unsigned int)GetInteger(string);
	}
	double GetDouble(const std::wstring& string)
	{
		if (string.empty()) return 0;

		double d = 0;
#if defined (_WIN32) || defined (_WIN64)
		swscanf_s(string.c_str(), L"%lf", &d);
#else
		swscanf(string.c_str(), L"%lf", &d);
#endif
		return d;
	}
	float GetFloat(const std::wstring& string)
	{
		if (string.empty()) return 0;

		float f = 0;
#if defined (_WIN32) || defined (_WIN64)
		swscanf_s(string.c_str(), L"%f", &f);
#else
		swscanf(string.c_str(), L"%f", &f);
#endif
		return f;
	}

	std::wstring ToString  (const bool  & value)
	{
		return ( value ? L"true" : L"false" );
	}
	std::wstring ToString (const float & value)
	{
		return boost::lexical_cast<std::wstring>(value);
	}
	std::wstring ToString(const int& value)
	{
		return boost::lexical_cast<std::wstring>(value);
	}
	std::wstring ToString(const long& value)
	{
		return boost::lexical_cast<std::wstring>(value);
	}
	std::wstring ToString(const unsigned long& value)
	{
		return boost::lexical_cast<std::wstring>(value);
	}
	std::wstring ToString(const double& value)
	{
		return boost::lexical_cast<std::wstring>(value);
	}
	std::wstring ToString(BYTE value, const wchar_t* format)
	{
		if (format == NULL) return L"";
		std::wstringstream sstream;
		sstream << boost::wformat(format) % value;
		return sstream.str();
	}
	std::string ToString(BYTE value, const char* format)
	{
		if (format == NULL) return "";
		std::stringstream sstream;
		sstream << boost::format(format) % value;
		return sstream.str();
	}
	std::wstring ToString(_INT16 value, const wchar_t* format)
	{
		if (format == NULL) return L"";
		std::wstringstream sstream;
		sstream << boost::wformat(format) % value;
		return sstream.str();
	}
	std::string ToString(_INT16 value, const char* format)
	{
		if (format == NULL) return "";
		std::stringstream sstream;
		sstream << boost::format(format) % value;
		return sstream.str();
	}
	std::wstring ToString(_UINT16 value, const wchar_t* format)
	{
		if (format == NULL) return L"";
		std::wstringstream sstream;
		sstream << boost::wformat(format) % value;
		return sstream.str();
	}
	std::string ToString(_UINT16 value, const char* format)
	{
		if (format == NULL) return "";
		std::stringstream sstream;
		sstream << boost::format(format) % value;
		return sstream.str();
	}
	std::wstring ToString(_INT32 value, const wchar_t* format)
	{
		if (format == NULL) return L"";
		std::wstringstream sstream;
		sstream << boost::wformat(format) % value;
		return sstream.str();
	}
	std::string ToString(_INT32 value, const char* format)
	{
		if (format == NULL) return "";
		std::stringstream sstream;
		sstream << boost::format(format) % value;
		return sstream.str();
	}
	std::wstring ToString(_UINT32 value, const wchar_t* format)
	{
		if (format == NULL) return L"";
		std::wstringstream sstream;
		sstream << boost::wformat(format) % value;
		return sstream.str();
	}
	std::string ToString(_UINT32 value, const char* format)
	{
		if (format == NULL) return "";
		std::stringstream sstream;
		sstream << boost::format(format) % value;
		return sstream.str();
	}
	std::wstring ToString(_INT64 value, const wchar_t* format)
	{
		if (format == NULL) return L"";
		std::wstringstream sstream;
		sstream << boost::wformat(format) % value;
		return sstream.str();
	}
	std::string ToString(_INT64 value, const char* format)
	{
		if (format == NULL) return "";
		std::stringstream sstream;
		sstream << boost::format(format) % value;
		return sstream.str();
	}
	std::wstring ToString(_UINT64 value, const wchar_t* format)
	{
		if (format == NULL) return L"";
		std::wstringstream sstream;
		sstream << boost::wformat(format) % value;
		return sstream.str();
	}
	std::string ToString(_UINT64 value, const char* format)
	{
		if (format == NULL) return "";
		std::stringstream sstream;
		sstream << boost::format(format) % value;
		return sstream.str();
	}
	std::wstring ToString( double value, const wchar_t* format )
	{
		if ( format == NULL ) return L"";
		std::wstringstream sstream;
		sstream << boost::wformat(format) % value;
		return sstream.str();
	}
	std::string ToString(double value, const char* format )
	{
		if ( format == NULL ) return "";
		std::stringstream sstream;
		sstream << boost::format(format) % value;
		return sstream.str();
	}

	int Rand()
	{
		//rand returns integral value range between 0 and RAND_MAX.(RAND_MAX at least 32767.)
		static bool init = false;   /* ensure different random header each time */
		if (!init)
		{
			init = true;
			srand((unsigned int) time(NULL));
		}
		return std::rand();
	}
	int GenerateInt()
	{
		//todo c++11 <random>
		return ((Rand() & 0x7FFF) | ((Rand() & 0x7FFF) << 15) | ((Rand() & 0x3) << 30));
	}

	std::wstring GenerateGuid()
	{
		std::wstring result;
		//#if defined (_WIN32) || defined(_WIN64)
		//		GUID guid;
		//		CoCreateGuid(&guid);
		//
		//		OLECHAR* guidString;
		//		StringFromCLSID(guid, &guidString);
		//
		//		result = std::wstring(guidString);
		//
		//		CoTaskMemFree(guidString);
		//#else
		std::wstringstream sstream;
		sstream << boost::wformat(L"%04X%04X-%04X-%04X-%04X-%04X%04X%04X") % (Rand() & 0xff) % (Rand() & 0xff) % (Rand() & 0xff) % ((Rand() & 0x0fff) | 0x4000) % ((Rand() % 0x3fff) + 0x8000) %  (Rand() & 0xff) % (Rand() & 0xff) % (Rand() & 0xff);
		result = sstream.str();
		//#endif
		return result;
	}
	std::wstring DoubleToString( double value, wchar_t* format )
	{
		if ( format == NULL ) return L"";

		std::wstringstream sstream;
		sstream << boost::wformat(format) % value;
		return sstream.str();
	}

	bool IsUnicodeSymbol( unsigned int symbol )
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

	std::string EncodeXmlString(const std::string& data, bool bDeleteNoUnicode)
	{
		std::string buffer;
		buffer.reserve(data.size());

		if(bDeleteNoUnicode)
		{
			for(size_t pos = 0; pos < data.size(); ++pos)
			{
				switch(data[pos])
				{
				case '&':  buffer.append("&amp;");      break;
				case '\"': buffer.append("&quot;");     break;
				case '\'': buffer.append("&apos;");     break;
				case '<':  buffer.append("&lt;");       break;
				case '>':  buffer.append("&gt;");       break;
				default:
				{
					if ( false == IsUnicodeSymbol( data[pos] ) )
					{
						wchar_t symbol1 = data[pos];
						if(0xD800 <= symbol1 && symbol1 <= 0xDFFF && pos + 1 < data.size())
						{
							pos++;
							wchar_t symbol2 = data[pos];
							if (symbol1 < 0xDC00 && symbol2 >= 0xDC00 && symbol2 <= 0xDFFF)
							{
								buffer.append(&data[pos-1], 2);
							}
							else
							{
								buffer.append(" ");
							}
						}
						else
						{
							buffer.append(" ");
						}
					}
					else
						buffer.append(&data[pos], 1);
				}break;
				}
			}
		}
		else
		{
			for(size_t pos = 0; pos < data.size(); ++pos)
			{
				switch(data[pos])
				{
				case '&':  buffer.append("&amp;");      break;
				case '\"': buffer.append("&quot;");     break;
				case '\'': buffer.append("&apos;");     break;
				case '<':  buffer.append("&lt;");       break;
				case '>':  buffer.append("&gt;");       break;
				case '\0':
					return buffer;
				default:   buffer.append(&data[pos], 1);	break;
				}
			}
		}

		return buffer;
	}
	std::wstring EncodeXmlString(const std::wstring& data, bool bDeleteNoUnicode)
	{
		std::wstring buffer;
		buffer.reserve(data.size());

		if(bDeleteNoUnicode)
		{
			for(size_t pos = 0; pos < data.size(); ++pos)
			{
				switch(data[pos])
				{
				case '&':  buffer.append(L"&amp;");      break;
				case '\"': buffer.append(L"&quot;");     break;
				case '\'': buffer.append(L"&apos;");     break;
				case '<':  buffer.append(L"&lt;");       break;
				case '>':  buffer.append(L"&gt;");       break;
				default:
				{
					if ( false == IsUnicodeSymbol( data[pos] ) )
					{
						wchar_t symbol1 = data[pos];
						if(0xD800 <= symbol1 && symbol1 <= 0xDFFF && pos + 1 < data.size())
						{
							pos++;
							wchar_t symbol2 = data[pos];
							if (symbol1 < 0xDC00 && symbol2 >= 0xDC00 && symbol2 <= 0xDFFF)
							{
								buffer.append(&data[pos-1], 2);
							}
							else
							{
								buffer.append(L" ");
							}
						}
						else
						{
							buffer.append(L" ");
						}
					}
					else
						buffer.append(&data[pos], 1);
				}break;
				}
			}
		}
		else
		{
			for(size_t pos = 0; pos < data.size(); ++pos)
			{
				switch(data[pos])
				{
				case '&':  buffer.append(L"&amp;");      break;
				case '\"': buffer.append(L"&quot;");     break;
				case '\'': buffer.append(L"&apos;");     break;
				case '<':  buffer.append(L"&lt;");       break;
				case '>':  buffer.append(L"&gt;");       break;
				case '\0':
					return buffer;
				default:   buffer.append(&data[pos], 1);	break;
				}
			}
		}

		return buffer;
	}
	std::wstring DeleteNonUnicode(const std::wstring& data)
	{
		std::wstring buffer;
		buffer.reserve(data.size());

		for (size_t pos = 0; pos < data.size(); ++pos)
		{
			if (false == IsUnicodeSymbol(data[pos]))
			{
				wchar_t symbol1 = data[pos];
				if (0xD800 <= symbol1 && symbol1 <= 0xDFFF && pos + 1 < data.size())
				{
					pos++;
					wchar_t symbol2 = data[pos];
					if (symbol1 < 0xDC00 && symbol2 >= 0xDC00 && symbol2 <= 0xDFFF)
					{
						buffer.append(&data[pos - 1], 2);
					}
					else
					{
						buffer.append(L" ");
					}
				}
				else
				{
					buffer.append(L" ");
				}
			}
			else
			{
				buffer.append(&data[pos], 1);
			}
		}
		return buffer;
	}
	std::wstring EncodeXmlStringExtend(const std::wstring& data, bool bDeleteNoUnicode)
	{
		std::wstring buffer;
		buffer.reserve(data.size());

		if(bDeleteNoUnicode)
		{
			for(size_t pos = 0; pos < data.size(); ++pos)
			{
				switch(data[pos])
				{
				case '&':  buffer.append(L"&amp;");      break;
				case '\"': buffer.append(L"&quot;");     break;
				case '\'': buffer.append(L"&apos;");     break;
				case '<':  buffer.append(L"&lt;");       break;
				case '>':  buffer.append(L"&gt;");       break;
				case '\n': buffer.append(L"&#xA;");      break;
				case '\r': buffer.append(L"&#xD;");      break;
				case '\t': buffer.append(L"&#x9;");      break;
				default:
				{
					if ( false == IsUnicodeSymbol( data[pos] ) )
					{
						wchar_t symbol1 = data[pos];
						if(0xD800 <= symbol1 && symbol1 <= 0xDFFF && pos + 1 < data.size())
						{
							pos++;
							wchar_t symbol2 = data[pos];
							if (symbol1 < 0xDC00 && symbol2 >= 0xDC00 && symbol2 <= 0xDFFF)
							{
								buffer.append(&data[pos-1], 2);
							}
							else
							{
								buffer.append(L" ");
							}
						}
						else
						{
							buffer.append(L" ");
						}
					}
					else
						buffer.append(&data[pos], 1);
				}break;
				}
			}
		}
		else
		{
			for(size_t pos = 0; pos < data.size(); ++pos)
			{
				switch(data[pos])
				{
				case '&':  buffer.append(L"&amp;");      break;
				case '\"': buffer.append(L"&quot;");     break;
				case '\'': buffer.append(L"&apos;");     break;
				case '<':  buffer.append(L"&lt;");       break;
				case '>':  buffer.append(L"&gt;");       break;
				case '\n': buffer.append(L"&#xA;");      break;
				case '\r': buffer.append(L"&#xD;");      break;
				case '\t': buffer.append(L"&#x9;");      break;
				case '\0':
					return buffer;
				default:   buffer.append(&data[pos], 1);	break;
				}
			}
		}

		return buffer;
	}
}
