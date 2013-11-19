#include "stdafx.h"
#include "BiffString.h"
#include <Exception/UnexpectedProgramPath.h>
#include <Exception/StructureSizeNotSet.h>

namespace XLS
{;


BiffString::BiffString()
:	struct_size(0)
{
}


BiffString::BiffString(const size_t size)
:	struct_size(0),
	cch_(size)
{
}


BiffString::BiffString(const _bstr_t& str)
:	struct_size(0),
	str_(!str ? L"" : str),
	cch_(str.length())
{
}


BiffString::~BiffString() 
{
}


BiffString::operator const _variant_t () const 
{
	return str_;
}


BiffString::operator const _bstr_t () const 
{
	return str_;
}


BiffString::operator const std::string () const 
{
	return static_cast<char*>(str_);
}


BiffString::operator const std::wstring () const 
{
	return static_cast<wchar_t*>(str_);
}


BiffString BiffString::operator=(const _bstr_t& str)
{
	str_ = str;
	cch_ = str_.length();
	return *this;
}


BiffStructurePtr BiffString::clone()
{
	return BiffStructurePtr(new BiffString(*this));
}


// Attribute version
void BiffString::toXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name)
{
	parent->setAttribute(attrib_name, getEscaped_ST_Xstring());
}


// Attribute version
const bool BiffString::fromXML(MSXML2::IXMLDOMElementPtr xml_tag, const _bstr_t& attrib_name)
{
	str_ = getStructAttribute(xml_tag, attrib_name);
	str_ = STR::unescape_ST_Xstring(static_cast<wchar_t*>(str_)).c_str();
	cch_ = str_.length();
	return true;
}


/*
void BiffString::toXML(MSXML2::IXMLDOMElementPtr parent, const std::string& tag_name)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(tag_name.c_str(), parent);
	own_tag->setAttribute(L"str", str_);
}


const bool BiffString::fromXML(MSXML2::IXMLDOMElementPtr parent, const std::string& tag_name)
{
#pragma message("####################### BiffString record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! BiffString record has no BiffStructure::fromXML() implemented.");
	return false;
}
*/


void BiffString::store(CFRecord& record)
{
	throw EXCEPT::LE::WhatIsTheFuck("Wrong usage of BiffString. The function must not be called.", __FUNCTION__);
}


void BiffString::load(CFRecord& record)
{
	throw EXCEPT::LE::WhatIsTheFuck("Wrong usage of BiffString. Stack overflow stopped.", __FUNCTION__);
//	record >> *this; // :-)
}


void BiffString::load(CFRecord& record, const size_t cch, const bool is_wide)
{
	size_t raw_length = cch << (is_wide ? 1 : 0);
	record.checkFitRead(raw_length);

	if(is_wide)
	{
		std::wstring int_str(record.getCurData<wchar_t>(), cch);
		str_ = int_str.c_str();
	}
	else
	{
		std::string int_str(record.getCurData<char>(), cch);
		str_ = STR::toStdWString(int_str, record.getGlobalWorkbookInfo()->CodePage).c_str();
	}
	record.skipNBytes(raw_length);
}


void BiffString::store(CFRecord& record, const bool is_wide)
{
	size_t raw_length = getSize() << (is_wide ? 1 : 0);
	record.checkFitWrite(raw_length);

	if(is_wide)
	{
		std::wstring int_str = static_cast<wchar_t*>(str_);
		for(std::wstring::iterator it = int_str.begin(), itEnd = int_str.end(); it != itEnd; ++it)
		{
			record << *it;
		}
		// no trailing zero - it is OK
	}
	else
	{
		std::string int_str = STR::toStdString(static_cast<wchar_t*>(str_), record.getGlobalWorkbookInfo()->CodePage);
		for(std::string::iterator it = int_str.begin(), itEnd = int_str.end(); it != itEnd; ++it)
		{
			record << *it;
		}
		// no trailing zero - it is OK
	}
}


const size_t BiffString::getSize() const
{
	if(!cch_.is_init())
	{
		throw EXCEPT::LE::StructureSizeNotSet("BiffString", __FUNCTION__/*__FUNCDNAME__*/); 
	}
	return cch_;
}


void BiffString::setSize(const size_t size)
{
	cch_ = size;
}


const size_t BiffString::getStructSize() const
{
	return struct_size;
}


// Set number of bytes read while loading. Must be used by >> operator only
void BiffString::setStructSize(const size_t size)
{
	struct_size = size;
}


const bool BiffString::isConformToOleLink() const
{
#pragma message("####################### BiffString::isConformToOleLink is not implemented")
	Log::info("BiffString::isConformToOleLink is not implemented.");
	size_t len = str_.length();
	for(size_t i = 0; i < len; ++i)
	{
		
	}
	return true;
}


const bool BiffString::isConformToVirtPath() const
{
#pragma message("####################### BiffString::isConformToVirtPath is not implemented")
	Log::info("BiffString::isConformToVirtPath is not implemented.");
	return true;
}



const _bstr_t BiffString::getEscapedUrlW() const
{
	if(!str_.length())
	{
		return str_;
	}
	std::wstring copy_str((wchar_t*)str_);
	return STR::escapeUrlW(copy_str).c_str();
}


void BiffString::EscapeUrlW()
{
	str_ = getEscapedUrlW();
}


const _bstr_t BiffString::getEscaped_ST_Xstring() const
{
	if(!str_.length())
	{
		return str_;
	}
	std::wstring copy_str((wchar_t*)str_);
	return STR::escape_ST_Xstring(copy_str).c_str();
}


void BiffString::Escape_ST_Xstring()
{
	str_ = getEscaped_ST_Xstring();
}


const bool BiffString::getWideRecommendation() const
{
	std::wstring str(static_cast<wchar_t*>(str_));
	for(std::wstring::const_iterator it = str.begin(), itEnd = str.end(); it != itEnd; ++it)
	{
		if(0xff00 & *it)
		{
			return true;
		}
	}
	return false;
}

} // namespace XLS
