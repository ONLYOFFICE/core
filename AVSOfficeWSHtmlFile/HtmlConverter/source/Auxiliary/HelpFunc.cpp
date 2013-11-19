#include "stdafx.h"
#include "HelpFunc.h"
#include "shlwapi.h"
#include <Exception/UnexpectedProgramPath.h>
#include <boost/regex.hpp>

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
	static char num_buf[_CVTBUFSIZE];
	_gcvt_s(num_buf, _CVTBUFSIZE, val, 309);
	return static_cast<wchar_t*>(_bstr_t(num_buf));
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


const _bstr_t guid2bstr(const GUID guid)
{
	LPOLESTR guid_str;
	if(S_OK != StringFromIID(guid, &guid_str))
	{
		// The only case is E_OUTOFMEMORY, so just throw anything
		throw EXCEPT::LE::WhatIsTheFuck("StringFromIID failed.", "guid2bstr");
	}
	_bstr_t guid_ret = guid_str;
	CoTaskMemFree(guid_str);
	return guid_ret;
}


const std::string guid2str(const GUID guid)
{
	return static_cast<char*>(guid2bstr(guid));
}


const bool bstr2guid(const _bstr_t& guid_str, GUID& guid)
{
	HRESULT res = IIDFromString(guid_str, &guid);
	if(S_OK != res)
	{
		switch(res)
		{
			case E_INVALIDARG:
				return false;
			case E_OUTOFMEMORY:
				throw EXCEPT::LE::WhatIsTheFuck("IIDFromString failed.", "bstr2guid");
		}
	}
	return true;
}


const std::string int2hex_str(const int val, const size_t size_of)
{
	static char num_buf[10];
	if(size_of > 4)
	{
		return "";
	}
	sprintf_s(num_buf, 9, ("%0" + STR::int2str(size_of << 1, 10) + "X").c_str(), val);
	return num_buf;
}


const std::wstring escapeW(const std::wstring& str)
{
	wchar_t dumb[2];
	DWORD buffer_size = 1;
	HRESULT res = UrlEscapeW(str.c_str(), dumb, &buffer_size, 0);
	
	wchar_t* buffer = new wchar_t[buffer_size + 1];
	res = UrlEscapeW(str.c_str(), buffer, &buffer_size, 0);

	std::wstring ret_val(buffer);
	delete[] buffer;
	return ret_val;
}


const std::string toARGB(const DWORD rgba)
{
	return toARGB(static_cast<BYTE>(rgba & 0xff), static_cast<BYTE>((rgba >> 8) & 0xff),
		static_cast<BYTE>((rgba >> 16) & 0xff), static_cast<BYTE>((rgba >> 24) & 0xff));
}


const std::string toARGB(const BYTE red, const BYTE green, const BYTE blue, const BYTE alpha)
{
	return int2hex_str(alpha, sizeof(alpha)) + int2hex_str(red, sizeof(red)) + 
		int2hex_str(green,sizeof(green)) + int2hex_str(blue,sizeof(blue));
}


const bool fromARGB(const std::wstring& argb, BYTE& red, BYTE& green, BYTE& blue, BYTE& alpha)
{
	if(argb.length() < 8)
	{
		return false;
	}
	alpha = static_cast<BYTE>(hex_str2int(argb.substr(0, 2)));
	red = static_cast<BYTE>(hex_str2int(argb.substr(2, 2)));
	green = static_cast<BYTE>(hex_str2int(argb.substr(4, 2)));
	blue = static_cast<BYTE>(hex_str2int(argb.substr(6, 2)));

	return true;
}


const size_t hex_str2int(const std::wstring& hex)
{
	static const std::wstring hex_digits(L"0123456789ABCDEF");
	size_t numeric = 0;
	for(std::wstring::const_iterator it = hex.begin(), itEnd = hex.end(); it != itEnd; ++it)
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


const std::string hres2str(const HRESULT hres)
{
	_com_error error(hres);
	_bstr_t errorText = error.ErrorMessage();
	return static_cast<char*>(errorText);
}



} // namespace STR


namespace XMLSTUFF
{;

MSXML2::IXMLDOMElementPtr findElementAmongParents(MSXML2::IXMLDOMElementPtr own_tag, const _bstr_t& tag_name, const bool checkSelf)
{
	_bstr_t query = _bstr_t(checkSelf ? L"ancestor-or-self::" : L"ancestor::") + tag_name;
	MSXML2::IXMLDOMElementPtr found = own_tag->selectSingleNode(query);
	return found;
}

const bool isChildOf(MSXML2::IXMLDOMElementPtr own_tag, const _bstr_t& tag_name, const bool checkSelf)
{
	return findElementAmongParents(own_tag, tag_name, checkSelf);
}


/*
const std::wstring moduleFileName()
{
static wchar_t buffer[255];
::GetModuleFileName(AfxGetInstanceHandle(), buffer, 254);
std::wstring module = buffer;
size_t fnpos = module.find_last_of('\\') + 1;
module = module.substr(fnpos, module.length() - fnpos);
return module;
}

*/
const bool loadXSLTFromResources(MSXML2::IXMLDOMDocumentPtr xslt_doc, const std::vector<std::wstring>& resources)
{
	if(resources.empty())
	{
		return true;
	}
	//std::wstring prefix = std::wstring(L"res://") + moduleFileName() + L"/XSL/";

	if(VARIANT_TRUE != xslt_doc->load(resources.begin()->c_str()))
	{
		return false;
	}
	MSXML2::IXMLDOMNodePtr dst_node = xslt_doc->GetdocumentElement(); // assuming xsl:stylesheet
	if(!dst_node)
	{
		return false;
	}
	for (std::vector<std::wstring>::const_iterator it = ++resources.begin(), itEnd = resources.end(); it != itEnd; ++it)
	{
		MSXML2::IXMLDOMDocument3Ptr another_xslt(_T("Msxml2.FreeThreadedDOMDocument.6.0"));
		another_xslt->Putasync(VARIANT_FALSE);
		if(VARIANT_TRUE != another_xslt->load(it->c_str()))
		{
			return false;
		}
		MSXML2::IXMLDOMDocumentFragmentPtr fragment = another_xslt->createDocumentFragment();
		//MSXML2::IXMLDOMNodeListPtr ins_elems = another_xslt->selectNodes(L"xsl:stylesheet/* | xsl:stylesheet/@*" );
		MSXML2::IXMLDOMNodePtr root_node = another_xslt->GetdocumentElement(); // assuming xsl:stylesheet
		MSXML2::IXMLDOMNodeListPtr ins_elems  = root_node->GetchildNodes();
		MSXML2::IXMLDOMNodePtr child;
		while(child = ins_elems->nextNode())
		{
			fragment->appendChild(child->cloneNode(VARIANT_TRUE));
		}

		dst_node->appendChild(fragment);
	}

	return true;
}


// Makes a new XML tag and append it to parent (no attributes set)
MSXML2::IXMLDOMElementPtr makeXMLNode(const std::string& tag_name, MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = parent->GetownerDocument()->createElement(tag_name.c_str());
	parent->appendChild(own_tag);
	return own_tag;
}


const std::wstring tab2sheet_name(const _bstr_t tabid, MSXML2::IXMLDOMDocumentPtr doc)
{
	MSXML2::IXMLDOMNodePtr node = doc->selectSingleNode(L"root/WorkbookStreamObject/GlobalsSubstream/BUNDLESHEET[position() = " + 
		tabid + L" + 1]/BoundSheet8/@stName");
	if(node)
	{
		return static_cast<wchar_t*>(node->Gettext());
	}

	return L"#REF!";
}


const std::wstring xti2sheets(const WORD ixti, MSXML2::IXMLDOMDocumentPtr doc)
{
	std::wstring query = L"root/WorkbookStreamObject/GlobalsSubstream/SUPBOOK/ExternSheet/XTI[position() = " + 
		STR::int2wstr(ixti + 1,10) + L"]";
	MSXML2::IXMLDOMElementPtr node = doc->selectSingleNode(query.c_str());
	if(node)
	{
		_bstr_t itabFirst = node->getAttribute(L"itabFirst");
		if(L"-1" == itabFirst)
		{
			return L"#REF!";
		}
		_bstr_t itabLast = node->getAttribute(L"itabLast");
		return tab2sheet_name(itabFirst, doc) + ((itabLast == itabFirst) ? L"" : L':' + tab2sheet_name(itabLast, doc));
	}
	return L"#REF!";
}


const std::wstring make3dRef(const WORD ixti, const std::wstring cell_ref, MSXML2::IXMLDOMDocumentPtr doc)
{
	std::wstring sheets_prefix = xti2sheets(ixti, doc);
	if(L"#REF!" == sheets_prefix)
	{
		return sheets_prefix;
	}
	std::wstring escaped_name(boost::algorithm::replace_all_copy(sheets_prefix, L"'", L"''"));
	if(!boost::regex_search(escaped_name.begin(), escaped_name.end(), boost::wregex(L"^\\w[\\w\\d.]*(:\\w[\\w\\d.]*)?$")))
	{
		return L'\'' + escaped_name + L"\'!" + cell_ref;
	}
	return escaped_name + L'!' + cell_ref;
}


const _bstr_t sheet_name2tabid(const std::wstring sheet_name, MSXML2::IXMLDOMDocumentPtr doc)
{
	_bstr_t query = L"root/WorkbookStreamObject/GlobalsSubstream/BUNDLESHEET/BoundSheet8";
	MSXML2::IXMLDOMNodeListPtr nodes = doc->selectNodes(query);
	MSXML2::IXMLDOMElementPtr sheet;
	for(size_t counter = 0; (sheet = nodes->nextNode()); ++counter)
	{
		MSXML2::IXMLDOMAttributePtr stName(sheet->getAttributeNode(L"stName"));
		if(stName && _bstr_t(sheet_name.c_str()) == static_cast<_bstr_t>(stName->Getvalue()))
		{
			return STR::int2wstr(counter).c_str();
		}
	}
	return L"-1";
}


const WORD sheetsnames2ixti(const std::wstring str, MSXML2::IXMLDOMDocumentPtr doc)
{
	_bstr_t sheet_first_id;
	_bstr_t sheet_last_id;
	size_t colon_pos = str.find_first_of(L':');
	if(std::wstring::npos == colon_pos)
	{
		sheet_last_id = sheet_first_id = sheet_name2tabid(str, doc);
	}
	else
	{
		sheet_first_id = sheet_name2tabid(str.substr(0, colon_pos), doc);
		sheet_last_id = sheet_name2tabid(str.substr(colon_pos + 1, str.length() - colon_pos - 1), doc);
	}

	_bstr_t query(L"root/WorkbookStreamObject/GlobalsSubstream/SUPBOOK/ExternSheet/XTI");
	MSXML2::IXMLDOMNodeListPtr nodes = doc->selectNodes(query);
	MSXML2::IXMLDOMElementPtr xti;
	for(size_t counter = 0; (xti = nodes->nextNode()); ++counter)
	{
		MSXML2::IXMLDOMAttributePtr itabFirst(xti->getAttributeNode(L"itabFirst"));
		MSXML2::IXMLDOMAttributePtr itabLast(xti->getAttributeNode(L"itabLast"));
		if(itabFirst && itabLast && sheet_first_id == static_cast<_bstr_t>(itabFirst->Getvalue()) &&
									sheet_last_id == static_cast<_bstr_t>(itabLast->Getvalue()))
		{
			return counter;
		}
	}
	return 0xFFFF;
}



} //namespace XMLSTUFF

