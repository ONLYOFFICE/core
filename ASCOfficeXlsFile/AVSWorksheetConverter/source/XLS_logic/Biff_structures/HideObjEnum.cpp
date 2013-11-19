#include "stdafx.h"
#include "HideObjEnum.h"
#include <Exception/WrongBiffRecord.h>

namespace XLS
{;


BiffStructurePtr HideObjEnum::clone()
{
	return BiffStructurePtr(new HideObjEnum(*this));
}


void HideObjEnum::toXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name)
{
	_bstr_t out_value;
	switch(val)
	{
		case SHOWALL:
			out_value = L"all";
			break;
		case SHOWPLACEHOLDER:
			out_value = L"placeholders";
			break;
		case HIDEALL:
			out_value = L"none";
			break;
		default:
			throw EXCEPT::RT::WrongBiffRecord("Unsupported type of HideObjEnum.", "unknown");
	}

	parent->setAttribute(attrib_name, out_value);
}


const bool HideObjEnum::fromXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name)
{
	_bstr_t in_value = getStructAttribute(parent, attrib_name);

	if(_bstr_t(L"all") == in_value)
	{
		val = SHOWALL;
	}
	else if(_bstr_t(L"placeholders") == in_value)
	{
		val = SHOWPLACEHOLDER;
	}
	else if(_bstr_t(L"none") == in_value)
	{
		val = HIDEALL;
	}
	else
	{
		throw EXCEPT::RT::WrongBiffRecord("Unsupported type of HideObjEnum.", "unknown");
	}
	return true;
}


} // namespace XLS

