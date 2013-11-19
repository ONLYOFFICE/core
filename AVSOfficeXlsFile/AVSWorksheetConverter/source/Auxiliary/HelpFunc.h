#pragma once

namespace XLS
{;
class CellRangeRef;
}

namespace AUX
{;


const int normalizeColumn(const int column);
const int normalizeRow(const int row);
const std::wstring column2str(const int column);
const std::wstring row2str(const int row);
const std::wstring loc2str(const long row, const bool row_rel, const long column, const bool col_rel);
void str2loc(const std::wstring& str, long& row, bool& row_rel, long& column, bool& col_rel);
void str2loc(std::wstring::const_iterator& str_begin, std::wstring::const_iterator& str_end, long& row, bool& row_rel, long& column, bool& col_rel);
void str2refs(const std::wstring& str, std::vector<XLS::CellRangeRef>& vec);


}

namespace STR
{;

const std::string int2str(const int val, const int radix = 10);
const std::wstring int2wstr(const int val, const int radix = 10);
const std::wstring double2str(const double val);
const std::string bin2str(const char* buf, const size_t nbuf);
const _bstr_t guid2bstr(const GUID guid);
const std::string guid2str(const GUID guid);
const bool bstr2guid(const _bstr_t& guid_str, GUID& guid);
const std::string int2hex_str(const int val, const size_t size_of = 4);
const std::wstring wchar_t2hex_str(const wchar_t val);
const std::wstring escapeUrlW(const std::wstring& str);
const std::wstring escape_ST_Xstring(const std::wstring& str);
const std::wstring unescape_ST_Xstring(const std::wstring& str);
const std::string toARGB(const DWORD rgba);
const std::string toARGB(const BYTE red, const BYTE green, const BYTE blue, const BYTE alpha);
const std::string toRGB(const BYTE red, const BYTE green, const BYTE blue);
const bool fromARGB(const std::wstring& argb, BYTE& red, BYTE& green, BYTE& blue, BYTE& alpha);
const size_t hex_str2int(const std::wstring& hex);
const size_t hex_str2int(const std::wstring::const_iterator& it_begin, const std::wstring::const_iterator& it_end);
const std::string hres2str(const HRESULT hres);

const std::string toStdString(const std::wstring& wide_string, const UINT code_page);
const std::wstring toStdWString(const std::string& ansi_string, const UINT code_page);

}

namespace XMLSTUFF
{;

const bool loadXSLTFromResources(MSXML2::IXMLDOMDocumentPtr xslt_doc, const std::vector<std::wstring>& resources);
// Makes a new XML tag and append it to parent (no attributes set)
MSXML2::IXMLDOMElementPtr makeXMLNode(const _bstr_t& tag_name, MSXML2::IXMLDOMElementPtr parent);
const std::wstring make3dRef(const WORD ixti, const std::wstring cell_ref, std::vector<std::wstring>& xti_parsed);
const WORD sheetsnames2ixti(const std::wstring str, MSXML2::IXMLDOMDocumentPtr doc);
const std::wstring xti_indexes2sheet_name(const short itabFirst, const short itabLast, std::vector<std::wstring>& sheets_names);

}

