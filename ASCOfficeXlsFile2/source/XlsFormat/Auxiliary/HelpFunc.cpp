

#include "HelpFunc.h"
#include "shlwapi.h"

#include <Logic/Biff_structures/CellRangeRef.h>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

namespace AUX
{;

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


const std::wstring column2str(const int column)
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
	return ret_val;
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


const std::wstring row2str(const int row)
{
	int row_value = normalizeRow(row);
	return STR::int2wstr(row_value + 1, 10);
}


const std::wstring loc2str(const long row, const bool row_rel, const long column, const bool col_rel)
{
	return (col_rel ? L"" : L"$") + column2str(column) + (row_rel ? L"" : L"$") + row2str(row);
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


void str2loc(const std::wstring& str, long& row, bool& row_rel, long& column, bool& col_rel)
{
	str2loc(str.begin(), str.end(), row, row_rel, column, col_rel);
}


void str2loc(std::wstring::const_iterator& str_begin, std::wstring::const_iterator& str_end, long& row, bool& row_rel, long& column, bool& col_rel)
{
	col_rel = str2rel(str_begin, str_end);
	column = str2column(str_begin, str_end);
	row_rel = str2rel(str_begin, str_end);
	row = str2row(str_begin, str_end);
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
	static char num_buf[10];
	_itoa_s(val, num_buf, 9, radix);
	return num_buf;
}


const std::wstring int2wstr(const int val, const int radix)
{
	static wchar_t num_buf[10];
	_itow_s(val, num_buf, 9, radix);
	return num_buf;
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
	//todooooo

	//LPOLESTR guid_str;
	//if(S_OK != StringFromIID(guid, &guid_str))
	//{
	//	// The only case is E_OUTOFMEMORY, so just throw anything
	//	throw;// EXCEPT::LE::WhatIsTheFuck("StringFromIID failed.", "guid2bstr");
	//}
	std::wstring  guid_ret;// = guid_str;
	//CoTaskMemFree(guid_str);
	return guid_ret;
}


const std::string guid2str(const _GUID_ guid)
{
	std::wstring s = guid2bstr(guid);
	return std::string(s.begin(),s.end());
}


const bool bstr2guid(const std::wstring & guid_str, _GUID_& guid)
{
	//todoooo
	//HRESULT res = IIDFromString((LPWSTR)(guid_str.c_str()), &guid);
	//if(S_OK != res)
	//{
	//	switch(res)
	//	{
	//		case E_INVALIDARG:
	//			return false;
	//		case E_OUTOFMEMORY:
	//			throw;// EXCEPT::LE::WhatIsTheFuck("IIDFromString failed.", "bstr2guid");
	//	}
	//}
	return true;
}


const std::wstring int2hex_wstr(const int val, const size_t size_of)
{
	static wchar_t num_buf[10];
	if(size_of > 4)
	{
		return L"";
	}
	swprintf_s(num_buf, 9, (L"%0" + STR::int2wstr(size_of << 1, 10) + L"X").c_str(), val);
	return std::wstring(num_buf);
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


const std::wstring escapeUrlW(const std::wstring& str)
{
	wchar_t dumb[2];
	DWORD buffer_size = 1;
	HRESULT res = UrlEscapeW(str.c_str(), dumb, &buffer_size, 0);
	if (0 == buffer_size)
	{
		return L"";
	}
	
	wchar_t* buffer = new wchar_t[buffer_size + 1];
	res = UrlEscapeW(str.c_str(), buffer, &buffer_size, 0);

	std::wstring ret_val(buffer);
	delete[] buffer;
	return ret_val;
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
	while(wstr_end != (x_pos_next = boost::algorithm::find_first(boost::make_iterator_range(x_pos_noncopied, wstr_end), L"_x").begin()))
	{
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
	alpha = static_cast<unsigned char>(hex_str2int(argb.substr(0, 2)));
	red = static_cast<unsigned char>(hex_str2int(argb.substr(2, 2)));
	green = static_cast<unsigned char>(hex_str2int(argb.substr(4, 2)));
	blue = static_cast<unsigned char>(hex_str2int(argb.substr(6, 2)));

	return true;
}


const size_t hex_str2int(const std::wstring& hex)
{
	return hex_str2int(hex.begin(), hex.end());
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

const std::string toStdString(const std::wstring& wide_string, const unsigned int code_page)
{
	//todooo разрулить на linux - e !!!
	unsigned int bufferSize = (unsigned int)wide_string.length() + 1;
	boost::scoped_array<char> pBuffer(new char[bufferSize]);
	WideCharToMultiByte(code_page, 0, wide_string.c_str(), (int)wide_string.length(), pBuffer.get(), bufferSize, NULL, NULL);
	pBuffer[bufferSize - 1] = '\0';
	return pBuffer.get();
}


const std::wstring toStdWString(const std::string& ansi_string, const unsigned int code_page)
{
	//todooo разрулить на linux - e !!!
	unsigned int bufferSize = (unsigned int)ansi_string.length() + 1;
	boost::scoped_array<wchar_t> pBuffer(new wchar_t[bufferSize]);
	unsigned int code = code_page == 1251 ? 1252 : code_page ;

	MultiByteToWideChar(/*code_page*/code, 0, ansi_string.c_str(), (int)ansi_string.length(), pBuffer.get(), bufferSize);
	pBuffer[bufferSize - 1] = L'\0';
	return pBuffer.get();
}


} // namespace STR


namespace XMLSTUFF
{;



const std::wstring tab2sheet_name(const short tabid, std::vector<std::wstring>& sheets_names)
{
	if(tabid >= 0 && static_cast<unsigned short>(tabid) < sheets_names.size())
	{
		return sheets_names[tabid];
	}
	return L"#REF!";
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
		return L"#REF!";
	}
	std::wstring sheets_prefix = tab2sheet_name(itabFirst, sheets_names) + ((itabLast == itabFirst) ? L"" : L':' + tab2sheet_name(itabLast, sheets_names));

	std::wstring escaped_prefix(boost::algorithm::replace_all_copy(sheets_prefix, L"'", L"''"));

	static boost::wregex correct_sheet_name(L"^\\w[\\w\\d.]*(:\\w[\\w\\d.]*)?$");
	if(!boost::regex_search(escaped_prefix.begin(), escaped_prefix.end(), correct_sheet_name))
	{
		return L'\'' + escaped_prefix + L'\'';
	}
	return escaped_prefix;
}

const std::wstring make3dRef(const unsigned short ixti, const std::wstring cell_ref, std::vector<std::wstring>& xti_parsed)
{
	std::wstring sheets_prefix = xti2sheets(ixti, xti_parsed);
	if(L"#REF!" == sheets_prefix)
	{
		return sheets_prefix;
	}
	return sheets_prefix + L'!' + cell_ref;
}


} //namespace XMLSTUFF

