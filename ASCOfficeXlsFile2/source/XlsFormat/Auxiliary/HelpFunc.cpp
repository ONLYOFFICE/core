/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "HelpFunc.h"

#ifndef _ASC_USE_UNICODE_CONVERTER_
	#if defined (_WIN32) || defined (_WIN64)
		#include "shlwapi.h"
	#else
		#include <iconv.h>
	#endif
#else
	#include "../../../../UnicodeConverter/UnicodeConverter.h"
#endif

#include <Logic/Biff_structures/CellRangeRef.h>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

namespace AUX
{

const int normalizeColumn(const int column)
{
	int norm_col = column;
	while(norm_col > 255)
	{
		norm_col -= 0x100;
	}
	while(norm_col < 0)
	{
		norm_col += 0x100; // It is correct. must be on the second place after 255
	}
	return norm_col;
}


const std::wstring column2str(const int column, const bool col_rel)
{
	int column_value = normalizeColumn(column);
	const int radix = L'Z' - L'A' + 1;
	std::wstring ret_val;
	++column_value;
	while(column_value)
	{
		wchar_t sym = (column_value - 1) % radix + L'A';
		ret_val = sym + ret_val;
		column_value = (column_value - 1) / radix;
	}

	return (col_rel ? L"" : L"$") + ret_val;
}


const int normalizeRow(const int row)
{
	int norm_row = row;
	while(norm_row > 65535)
	{
		norm_row -= 0x10000;
	}
	while(norm_row < 0) 
	{
		norm_row += 0x10000; // It is correct. must be on the second place after 65535
	}
	return norm_row;
}


const std::wstring row2str(const int row, const bool row_rel)
{
	int row_value = normalizeRow(row);
	return  (row_rel ? L"" : L"$") + STR::int2wstr(row_value + 1, 10);
}


const std::wstring loc2str(const int row, const bool row_rel, const int column, const bool col_rel)
{
	return column2str(column, col_rel) + row2str(row, row_rel);
}


const int str2column(std::wstring::const_iterator& str_begin, std::wstring::const_iterator& str_end)
{
	const int radix = L'Z' - L'A' + 1;
	int  column = -1;

	for(; str_begin != str_end; ++str_begin)
	{
		const wchar_t symb = *str_begin;
		if(L'A' > symb || L'Z' < symb)
		{
			break;
		}
		column = (column + 1) * radix + (symb - L'A');
	}

	if(column > 255)
	{
		column = 255;
	}

	return column;
}


const int str2row(std::wstring::const_iterator& str_begin, std::wstring::const_iterator& str_end)
{
	int row = 0;
	for(; str_begin != str_end; ++str_begin)
	{
		const wchar_t symb = *str_begin;
		if(L'0' > symb || L'9' < symb)
		{
			break;
		}
		row = row * 10 + (symb - L'0');
	}
	--row;
	if(row > 65535)
	{
		row = 65535;
	}
	return row;
}


const bool str2rel(std::wstring::const_iterator& str_begin, std::wstring::const_iterator& str_end)
{
	if(str_begin != str_end && L'$' == *str_begin)
	{
		++str_begin;
		return false;
	}
	return true;
}

void str2loc(std::wstring::const_iterator& str_begin, std::wstring::const_iterator& str_end, int& row, bool& row_rel, int& column, bool& col_rel)
{
    col_rel     = str2rel       (str_begin, str_end);
    column      = str2column    (str_begin, str_end);
    row_rel     = str2rel       (str_begin, str_end);
    row         = str2row       (str_begin, str_end);
}

void str2loc(const std::wstring& str, int& row, bool& row_rel, int& column, bool& col_rel)
{
   std::wstring::const_iterator str_begin  = str.begin();
   std::wstring::const_iterator str_end    = str.end();

    str2loc(str_begin, str_end, row, row_rel, column, col_rel);
}




void str2refs(const std::wstring& str, std::vector<XLS::CellRangeRef>& vec)
{
	std::wstring str_left(str);
	do
	{
		XLS::CellRangeRef ref1(str_left);
		vec.push_back(ref1);
		size_t next_pos = str_left.find_first_of(L' ');
		if(std::wstring::npos == next_pos)
		{
			break;
		}
		str_left = str_left.substr(next_pos + 1, str_left.length() - next_pos - 1);
	}
	while(str_left.length());
}


} //namespace AUX


namespace STR
{;

const std::string int2str(const int val, const int radix)
{
    static char num_buf[10]={};
#if defined(_WIN32) || defined(_WIN64)
    _itoa_s(val, num_buf, 9, radix);
#else
    sprintf( num_buf, "%d", val);
#endif
    return num_buf;
}


const std::wstring int2wstr(const int val, const int radix)
{
#if defined(_WIN32) || defined(_WIN64)
    static wchar_t num_buf[20]={};
    _itow_s(val, num_buf, 19, radix);
    return std::wstring(num_buf);
#else
    static char num_buf[20]={};
    sprintf( num_buf, "%d", val);
    std::string a_str(num_buf);
    return std::wstring(a_str.begin(), a_str.end());
#endif
}


const std::wstring double2str(const double val)
{
	return boost::lexical_cast<std::wstring>(val);
}


const std::string bin2str(const char* buf, const size_t nbuf)
{
	static const char hex[17] = "0123456789ABCDEF";
	std::vector<char> code_string(nbuf * 3 + 1);
	size_t idx = 0;
	for(size_t i = 0; i < nbuf; ++i)
	{
		code_string[idx++] = '%';
		code_string[idx++] = hex[(buf[i] & 0xf0) >> 4];
		code_string[idx++] = hex[buf[i] & 0x0f];
	}
	code_string[idx] = 0;
	return &code_string[0];
}


const std::wstring  guid2bstr(const _GUID_ guid)
{
	std::wstring  guid_ret=L"{";

	guid_ret += int2hex_wstr(guid.Data1, 4) + L"-" + 
				int2hex_wstr(guid.Data2, 2) + L"-" + 
				int2hex_wstr(guid.Data3, 2) + L"-" +
				int2hex_wstr(guid.Data4[0], 1) + int2hex_wstr(guid.Data4[1], 1) + L"-" +
				int2hex_wstr(guid.Data4[2], 1) + int2hex_wstr(guid.Data4[3], 1) +
				int2hex_wstr(guid.Data4[4], 1) + int2hex_wstr(guid.Data4[5], 1) +
				int2hex_wstr(guid.Data4[6], 1) + int2hex_wstr(guid.Data4[7], 1);
	return guid_ret + L"}";
}


const std::string guid2str(const _GUID_ guid)
{
	std::wstring s = guid2bstr(guid);
	return std::string(s.begin(),s.end());
}


const bool bstr2guid(const std::wstring & guid_str, _GUID_& guid)
{
	return false;
}


const std::wstring int2hex_wstr(const int val, const size_t size_of)
{
    if(size_of > 4) return L"";


#if defined(_WIN32) || defined(_WIN64)
    static wchar_t num_buf[10]={};
    std::wstring wstr =  STR::int2wstr(size_of << 1, 10);
    swprintf_s(num_buf, 9, (L"%0" + wstr + L"X").c_str(), val);
    return std::wstring(num_buf);
#else
    char num_buf[10]={};
    std::string str =  STR::int2str(size_of << 1, 10);
    snprintf(num_buf, 9, ("%0" + str + "X").c_str(), val);
    std::string res(num_buf);
    return std::wstring(res.begin(), res.end());
#endif
}


const std::wstring wchar_t2hex_str(const wchar_t val)
{
	static const wchar_t hex[17] = L"0123456789ABCDEF";
	wchar_t code_string[5];
	code_string[0] = hex[(val & 0xf000) >> 12];
	code_string[1] = hex[(val & 0x0f00) >> 8];
	code_string[2] = hex[(val & 0x00f0) >> 4];
	code_string[3] = hex[(val & 0x000f)];
	code_string[4] = 0;

	return code_string;
}


static boost::wregex match_hex(L"^_x[0-9A-F]{4}_");
const std::wstring escape_ST_Xstring(const std::wstring& wstr)
{
	std::wstring ret_val = L"";

	for (size_t i = 0; i < wstr.length(); i++)
	{
		wchar_t in_symb = wstr[i];
		if (in_symb < 0x0009 || in_symb == 0x000B || in_symb == 0x000C || in_symb > 0x000D && in_symb < 0x0020 ||
			in_symb > 0xD7FF && in_symb < 0xE000 || in_symb > 0xFFFD)
		{
			ret_val += L"_x" + wchar_t2hex_str(in_symb) + L"_";
		}
		else if(L'_' == in_symb && boost::regex_search(std::wstring(wstr, i, 7), match_hex))
		{
			ret_val += L"_x005F_";
		}
		else
		{
			ret_val += in_symb;
		}
	}
	return ret_val;
}


const std::wstring unescape_ST_Xstring(const std::wstring& wstr)
{
	std::wstring::const_iterator x_pos_noncopied = wstr.begin();
	std::wstring::const_iterator x_pos_next;// = wstr.begin();
	std::wstring::const_iterator wstr_end = wstr.end();
	std::wstring ret_val = L"";

    while(true)
	{
#if defined(__linux__) || defined(_MAC)
		const auto it_range = boost::make_iterator_range(x_pos_noncopied, wstr_end);
        x_pos_next = boost::algorithm::find_first(it_range, L"_x").begin();
#else
        x_pos_next = boost::algorithm::find_first(boost::make_iterator_range(x_pos_noncopied, wstr_end), L"_x").begin();
#endif
        if ( wstr_end == x_pos_next) break;
        if(!boost::regex_search(x_pos_next, wstr_end, match_hex))
		{
			x_pos_next += 2;
			ret_val.append(x_pos_noncopied, x_pos_next);
			x_pos_noncopied = x_pos_next;
		}
		else
		{
			ret_val += static_cast<wchar_t>(hex_str2int(x_pos_next + 2, x_pos_next + 6));
			x_pos_noncopied = x_pos_next + 7;
		}
	}

	ret_val.append(x_pos_noncopied, x_pos_next);

	return ret_val;
}


const std::wstring toARGB(const unsigned int rgba)
{
	return toARGB(static_cast<unsigned char>(rgba & 0xff), static_cast<unsigned char>((rgba >> 8) & 0xff),
		static_cast<unsigned char>((rgba >> 16) & 0xff), static_cast<unsigned char>((rgba >> 24) & 0xff));
}

const std::wstring toRGB(const unsigned int rgba)
{
	unsigned char R = static_cast<unsigned char>((rgba >> 16) & 0xff);
	unsigned char G = static_cast<unsigned char>((rgba >> 8) & 0xff);
	unsigned char B = static_cast<unsigned char>(rgba & 0xff);
	return toRGB(R, G ,B);
}

const std::wstring toARGB(const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha)
{
	return int2hex_wstr(alpha, sizeof(alpha)) + toRGB(red, green, blue);
}


const std::wstring toRGB(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	return int2hex_wstr(red, sizeof(red)) + int2hex_wstr(green,sizeof(green)) + int2hex_wstr(blue,sizeof(blue));
}


const bool fromARGB(const std::wstring& argb, unsigned char& red, unsigned char& green, unsigned char& blue, unsigned char& alpha)
{
	if(argb.length() < 8)
	{
		return false;
	}
	alpha	= static_cast<unsigned char>	(hex_str2int(argb.substr(0, 2)));
	red		= static_cast<unsigned char>	(hex_str2int(argb.substr(2, 2)));
	green	= static_cast<unsigned char>	(hex_str2int(argb.substr(4, 2)));
	blue	= static_cast<unsigned char>	(hex_str2int(argb.substr(6, 2)));

	return true;
}

const int HexChar2Int(const char value)
{
	if (value >= '0' && value <= '9')
		return value - '0';
	if (value >= 'a' && value <= 'f')
		return 10 + value - 'a';
	if (value >= 'A' && value <= 'F')
		return 10 + value - 'A';
	return 0;
}
const size_t hex_str2int(const std::wstring& hex)
{
	size_t  summa = 0;
	for (int i = 0; i != hex.length(); ++i)
		summa += HexChar2Int((char)hex[i]) << (4 * (hex.length() - i - 1));
	return summa;
}

const size_t hex_str2int(const std::wstring::const_iterator& it_begin, const std::wstring::const_iterator& it_end)
{
	static const std::wstring hex_digits(L"0123456789ABCDEF");
	size_t numeric = 0;
	for(std::wstring::const_iterator it = it_begin, itEnd = it_end; it != itEnd; ++it)
	{
		size_t pos = hex_digits.find_first_of(*it);
		if(std::wstring::npos == pos)
		{
			break;
		}
		numeric = numeric * 16 + pos;
	}
	return numeric;
}

 #ifdef _ASC_USE_UNICODE_CONVERTER_
const std::wstring toStdWString(std::string ansi_string, const unsigned int code_page)
{
    std::string sCodePage;
    for (int i = 0; i < UNICODE_CONVERTER_ENCODINGS_COUNT; ++i)
    {
        if (code_page == NSUnicodeConverter::Encodings[i].WindowsCodePage)
        {
            sCodePage = NSUnicodeConverter::Encodings[i].Name;
            break;
        }
    }
    if (sCodePage.empty())
        sCodePage = "CP" + std::to_string(code_page);

    NSUnicodeConverter::CUnicodeConverter oConverter;
    return oConverter.toUnicode(ansi_string, sCodePage.c_str());
}
const std::wstring	toStdWString(char* ansi, int size, const unsigned int code_page)
{
    std::string sCodePage;
    for (int i = 0; i < UNICODE_CONVERTER_ENCODINGS_COUNT; ++i)
    {
        if (code_page == NSUnicodeConverter::Encodings[i].WindowsCodePage)
        {
            sCodePage = NSUnicodeConverter::Encodings[i].Name;
            break;
        }
    }
    if (sCodePage.empty())
        sCodePage = "CP" + std::to_string(code_page);

    NSUnicodeConverter::CUnicodeConverter oConverter;
    return oConverter.toUnicode(ansi, size, sCodePage.c_str());
}
const std::string toStdString(std::wstring wide_string, const unsigned int code_page)
{
    std::string sCodePage;
    for (int i = 0; i < UNICODE_CONVERTER_ENCODINGS_COUNT; ++i)
    {
        if (code_page == NSUnicodeConverter::Encodings[i].WindowsCodePage)
        {
            sCodePage = NSUnicodeConverter::Encodings[i].Name;
            break;
        }
    }
    if (sCodePage.empty())
        sCodePage = "CP" + std::to_string(code_page);

    NSUnicodeConverter::CUnicodeConverter oConverter;
    return oConverter.fromUnicode(wide_string, sCodePage.c_str());
}
#else
const std::string toStdString(std::wstring wide_string, const unsigned int code_page)
{
#if defined (_WIN32) || defined (_WIN64)
    const int nSize = WideCharToMultiByte(code_page, 0, wide_string.c_str(), wide_string.length(), NULL, 0, NULL, NULL);
    char *sTemp = new char[nSize];
    if (!sTemp)
        return std::string();

    int size = WideCharToMultiByte(code_page, 0, wide_string.c_str(), wide_string.length(), sTemp, nSize, NULL, NULL);

    std::string sResult(sTemp, size);
    delete []sTemp;

    return sResult;
#else
    std::string out;
    bool ansi = true;

    size_t insize = wide_string.length();
    out.reserve(insize);

    char *inptr = (char*)wide_string.c_str();
    char* outptr = (char*)out.c_str();

    if (code_page >= 0)
    {
        std::string sCodepage =  "CP" + std::to_string(code_page);

        iconv_t ic= iconv_open(sCodepage.c_str(), "WCHAR_T");
        if (ic != (iconv_t) -1)
        {
            size_t nconv = 0, avail = insize * sizeof(wchar_t);

            nconv = iconv (ic, &inptr, &insize, &outptr, &avail);
            if (nconv == 0) ansi = false;
            iconv_close(ic);
        }
    }

    if (ansi)
        out = std::string(wide_string.begin(), wide_string.end());

    return out;

#endif
}
 const std::wstring	toStdWString(char* ansi, int size, const unsigned int code_page)
 {
 #if defined (_WIN32) || defined (_WIN64)
    const int nSize = MultiByteToWideChar(code_page, 0, ansi, size, NULL, 0);

    wchar_t *sTemp = new wchar_t[nSize];
    if (!sTemp)
        return std::wstring();

    int size_out = MultiByteToWideChar(code_page, 0, ansi, size, sTemp, nSize);

    std::wstring sResult(sTemp, size_out);
    delete []sTemp;

    return sResult;
#else
    bool bAnsi = true;

    size_t insize = size;
    std::wstring w_out;

    char *inptr = ansi;

    if (code_page >= 0)
    {
        std::string sCodepage =  "CP" + std::to_string(code_page);

        iconv_t ic= iconv_open("WCHAR_T", sCodepage.c_str());
        if (ic != (iconv_t) -1)
        {
            size_t nconv = 0, avail = (insize + 1) * sizeof(wchar_t);

            char* out_str   = new char[avail];
            char* outptr    = out_str;

            nconv = iconv (ic, &inptr, &insize, &outptr, &avail);
            if (nconv == 0)
            {
                insize = size;
                ((wchar_t*)out_str)[insize] = 0;
                w_out = std::wstring((wchar_t*)out_str, insize);
                bAnsi = false;
            }
            iconv_close(ic);
            delete []out_str;
        }
    }
    if (bAnsi)
	{
		std::string ansi_string(ansi, size);
        w_out = std::wstring(ansi_string.begin(), ansi_string.end());
	}

    return w_out;
#endif
 }

const std::wstring toStdWString(std::string ansi_string, const unsigned int code_page)
{
	return toStdWString((char*)ansi_string.c_str(), (int)ansi_string.length(), code_page);
}
#endif


} // namespace STR


namespace XMLSTUFF
{;



const std::wstring tab2sheet_name(const short tabid, std::vector<std::wstring>& sheets_names)
{
	if(tabid >= 0 && static_cast<unsigned short>(tabid) < sheets_names.size())
	{
		return sheets_names[tabid];
	}
	return L"#REF";
}

//
const std::wstring xti2sheets(const unsigned short ixti, std::vector<std::wstring>& xti_parsed)
{
	if(ixti >= 0 && static_cast<unsigned short>(ixti) < xti_parsed.size())
	{
		return xti_parsed[ixti];
	}
	return L"#REF!";
}
//
const std::wstring xti_indexes2sheet_name(const short itabFirst, const short itabLast, std::vector<std::wstring>& sheets_names)
{
	if(-1 == itabFirst)
	{
		return L"#REF";
	}
	static boost::wregex correct_sheet_name(L"^\\'.+?\\'$");
    static boost::wregex test_sheet_name(L"[\\s)(\\':.-]+");
	
	std::wstring sheet_first = tab2sheet_name(itabFirst, sheets_names);
	if(!boost::regex_search(sheet_first.begin(), sheet_first.end(), correct_sheet_name))
	{	
		if(boost::regex_search(sheet_first.begin(), sheet_first.end(), test_sheet_name))
		{	
			sheet_first = boost::algorithm::replace_all_copy(sheet_first, L"'", L"''"); 
			sheet_first = std::wstring(L"\'") + sheet_first + std::wstring(L"\'");
		}
	}
	else
	{
		//todooo найти хоть один файл где в апострофах уже есть внутренний не экранированный апостроф
		//static boost::wregex test_sheet_name1(L"[\']+");
		//if(boost::regex_search(sheet_first.begin() + 1, sheet_first.end() - 1, test_sheet_name1))
		//{	
		//	sheet_first = boost::algorithm::replace_all_copy(sheet_first.begin()+1, sheet_first.end() - 1 , L"'", L"''"); 
		//	sheet_first = std::wstring(L"\'") + sheet_first + std::wstring(L"\'");
		//}
	}

	std::wstring sheet_last;
	if (itabLast != itabFirst)
	{
		sheet_last = tab2sheet_name(itabLast, sheets_names);
		
		if(!boost::regex_search(sheet_last.begin(), sheet_last.end(), correct_sheet_name))
		{	
			if(boost::regex_search(sheet_last.begin(), sheet_last.end(), test_sheet_name))
			{	
				sheet_last = boost::algorithm::replace_all_copy(sheet_last, L"'", L"''"); 
				sheet_last = std::wstring(L"\'") + sheet_last + std::wstring(L"\'");
			}
		}
		sheet_last = std::wstring(L":") + sheet_last;
	}

	return sheet_first + sheet_last;
}

const std::wstring make3dRef(const unsigned short ixti, const std::wstring cell_ref, std::vector<std::wstring>& xti_parsed, bool full_ref)
{
	std::wstring sheets_prefix = xti2sheets(ixti, xti_parsed);
	
	if(L"#REF!" == sheets_prefix)
	{
		return sheets_prefix;
	}
	else if (!sheets_prefix.empty())				sheets_prefix += L"!";
	else if (sheets_prefix.empty() && full_ref)		sheets_prefix += L"#REF!";
	
	return sheets_prefix + cell_ref;
}


} //namespace XMLSTUFF

