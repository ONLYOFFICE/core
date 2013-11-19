#pragma once

#include "BiffAttribute.h"
#include <Exception/WrongIntermediateXMLFormat.h>

namespace XLS
{;

class CFRecord;

template<class Type>
struct Boolean : public BiffAttributeSimple<Type>
{
	Boolean<Type>() {};
	Boolean<Type>(const Type& val) : BiffAttributeSimple<Type>(val) {};
	BiffStructurePtr clone()
	{
		return BiffStructurePtr(new Boolean<Type>(*this));
	};
	operator const _variant_t() const { return toString().c_str(); };
	operator const bool()
	{
		return static_cast<Type>(1) == val;
	};
	operator const Type()
	{
		return val;
	};
	operator const std::wstring() const	{ return toString();};
	const std::wstring toString() const
	{
		return static_cast<Type>(1) == val ? L"true" : L"false";
	};
	Boolean<Type> operator=(const Boolean<Type>& other)
	{
		val = other.val;
		return *this;
	};

	virtual void toXML(MSXML2::IXMLDOMElementPtr xml_tag, const _bstr_t& attrib_name)
	{
		xml_tag->setAttribute(attrib_name, toString().c_str());
	};
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr xml_tag, const _bstr_t& attrib_name)
	{
		_bstr_t raw_val = getStructAttribute(xml_tag, attrib_name);
		if(_bstr_t(L"0") == raw_val || _bstr_t(L"false") == raw_val || _bstr_t(L"FALSE") == raw_val)
		{
			val = static_cast<Type>(0);
			return true;
		}
		if(_bstr_t(L"1") == raw_val || _bstr_t(L"true") == raw_val || _bstr_t(L"TRUE") == raw_val)
		{
			val = static_cast<Type>(1);
			return true;
		}
		throw EXCEPT::LE::WrongIntermediateXMLFormat(std::string("Format of @") + static_cast<char*>(attrib_name) + " attribute of <" +
			static_cast<char*>(xml_tag->GetnodeName()) + "> tag is not boolean. Received value: \"" + 
			static_cast<char*>(raw_val) + "\"", __FUNCTION__);
	};
};

} // namespace XLS

