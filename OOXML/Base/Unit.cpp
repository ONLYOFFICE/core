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
        if (string.empty()) return 0;

        unsigned int iVal = 0;
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
		return std::to_wstring(value);
	}
	std::wstring ToString(const long& value)
	{
		return std::to_wstring(value);
	}
	std::wstring ToString(const unsigned long& value)
	{
		return std::to_wstring(value);
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

	std::wstring DoubleToString( double value, wchar_t* format )
	{
		if ( format == NULL ) return L"";

		std::wstringstream sstream;
		sstream << boost::wformat(format) % value;
		return sstream.str();
	}

	bool IsUnicodeSymbol(unsigned int symbol)
	{
		return (symbol == 0x0009) || (symbol == 0x000A) || (symbol == 0x000D)
			|| (symbol >= 0x0020   && symbol <= 0xD7FF)
			|| (symbol >= 0xE000   && symbol <= 0xFFFD)
			|| (symbol >= 0x10000  && symbol <= 0x10FFFF);
	}

	std::wstring EncodeXmlString(const std::wstring& data, bool bDeleteNoUnicode)
	{
		std::wstring buffer;
		buffer.reserve(data.size());

		if (bDeleteNoUnicode)
		{
			for (size_t pos = 0; pos < data.size(); ++pos)
			{
				switch (data[pos])
				{
				case L'&':  buffer.append(L"&amp;");  continue;
				case L'\"': buffer.append(L"&quot;"); continue;
				case L'\'': buffer.append(L"&apos;"); continue;
				case L'<':  buffer.append(L"&lt;");   continue;
				case L'>':  buffer.append(L"&gt;");   continue;
				default: break;
				}

				unsigned int codepoint = 0;
				size_t consumed = 1;

#if defined(_WIN32) || defined(_WIN64)
				uint16_t w1 = static_cast<uint16_t>(data[pos]);
				if (w1 >= 0xD800 && w1 <= 0xDBFF && pos + 1 < data.size())
				{
					uint16_t w2 = static_cast<uint16_t>(data[pos + 1]);
					if (w2 >= 0xDC00 && w2 <= 0xDFFF)
					{
						codepoint = 0x10000 + ((w1 - 0xD800) << 10) + (w2 - 0xDC00);
						consumed = 2;
					}
					else
					{
						codepoint = w1;
					}
				}
				else
				{
					codepoint = w1;
				}
#else
				codepoint = static_cast<unsigned int>(data[pos]);
#endif

				if (IsUnicodeSymbol(codepoint))
					buffer.append(&data[pos], consumed);
				else
					buffer.append(L" ");

				if (consumed == 2)
					++pos;
			}
		}
		else
		{
			for (size_t pos = 0; pos < data.size(); ++pos)
			{
				switch (data[pos])
				{
				case L'&':  buffer.append(L"&amp;");  break;
				case L'\"': buffer.append(L"&quot;"); break;
				case L'\'': buffer.append(L"&apos;"); break;
				case L'<':  buffer.append(L"&lt;");   break;
				case L'>':  buffer.append(L"&gt;");   break;
				case L'\0': return buffer;
				default:    buffer.append(&data[pos], 1); break;
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

#include "./Rand.h"
namespace XmlUtils
{
	int GenerateInt()
	{
		return static_cast<int>(RandUInt());
	}

	std::wstring GenerateGuid()
	{
		std::wstring result;

		std::wstringstream sstream;
		sstream << boost::wformat(L"%04X%04X-%04X-%04X-%04X-%04X%04X%04X")
					   % (RandUInt() & 0xffff)
					   % (RandUInt() & 0xffff)
					   % (RandUInt() & 0xffff)
					   % ((RandUInt() & 0x0fff) | 0x4000)
					   % ((RandUInt() & 0x3fff) | 0x8000)
					   % (RandUInt() & 0xffff)
					   % (RandUInt() & 0xffff)
					   % (RandUInt() & 0xffff);
		result = sstream.str();

		return result;
	}
}
