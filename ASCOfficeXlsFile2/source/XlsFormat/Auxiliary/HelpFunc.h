#pragma once

#include "guiddef.h"

namespace XLS
{
	class CellRangeRef;
};

namespace AUX
{
	const int normalizeColumn(const int column);
	const int normalizeRow(const int row);
	const std::wstring column2str(const int column);
	const std::wstring row2str(const int row);
	const std::wstring loc2str(const long row, const bool row_rel, const long column, const bool col_rel);
	void str2loc(const std::wstring& str, long& row, bool& row_rel, long& column, bool& col_rel);
	void str2loc(std::wstring::const_iterator& str_begin, std::wstring::const_iterator& str_end, long& row, bool& row_rel, long& column, bool& col_rel);
	void str2refs(const std::wstring& str, std::vector<XLS::CellRangeRef>& vec);
};


namespace STR
{
	const std::string int2str(const int val, const int radix = 10);
	const std::wstring int2wstr(const int val, const int radix = 10);
	const std::wstring double2str(const double val);
	const std::string bin2str(const char* buf, const size_t nbuf);
	const std::wstring  guid2bstr(const GUID guid);
	const std::string guid2str(const GUID guid);
	const bool bstr2guid(const std::wstring & guid_str, GUID& guid);
	const std::wstring int2hex_wstr(const int val, const size_t size_of = 4);
	const std::wstring wchar_t2hex_str(const wchar_t val);
	const std::wstring escapeUrlW(const std::wstring& str);
	const std::wstring escape_ST_Xstring(const std::wstring& str);
	const std::wstring unescape_ST_Xstring(const std::wstring& str);
	const std::wstring toARGB(const unsigned int rgba);
	const std::wstring toARGB(const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha);
	const std::wstring toRGB(const unsigned char red, const unsigned char green, const unsigned char blue);
	const bool fromARGB(const std::wstring& argb, unsigned char& red, unsigned char& green, unsigned char& blue, unsigned char& alpha);
	const size_t hex_str2int(const std::wstring& hex);
	const size_t hex_str2int(const std::wstring::const_iterator& it_begin, const std::wstring::const_iterator& it_end);
	//const std::string hres2wstr(const HRESULT hres);

	const std::string toStdString(const std::wstring& wide_string, const unsigned int code_page);
	const std::wstring toStdWString(const std::string& ansi_string, const unsigned int code_page);
};

namespace XMLSTUFF
{;

// Makes a new tag and append it to parent (no attributes set)
//BiffStructurePtr createElement(const std::wstring & tag_name, BiffStructurePtr & parent);

const std::wstring make3dRef(const unsigned __int16 ixti, const std::wstring cell_ref, std::vector<std::wstring>& xti_parsed);
//const unsigned __int16 sheetsnames2ixti(const std::wstring str, MSXML2::IXMLDOMDocumentPtr doc);

const std::wstring xti_indexes2sheet_name(const short itabFirst, const short itabLast, std::vector<std::wstring>& sheets_names);

}

