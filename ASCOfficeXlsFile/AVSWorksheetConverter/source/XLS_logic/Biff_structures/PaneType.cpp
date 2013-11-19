#include "stdafx.h"
#include "PaneType.h"
#include <Exception/WrongBiffRecord.h>

namespace XLS
{;


BiffStructurePtr PaneType::clone()
{
	return BiffStructurePtr(new PaneType(*this));
}


void PaneType::toXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name)
{
	_variant_t result;
	switch(val)
	{
		case REVTPNNBOTRIGHT:
			result = L"bottomRight";
			break;
		case REVTPNNTOPRIGHT:
			result = L"topRight";
			break;
		case REVTPNNBOTLEFT:
			result = L"bottomLeft";
			break;
		case REVTPNNTOPLEFT:
			result = L"topLeft";
			break;
		default:
			throw EXCEPT::RT::WrongBiffRecord("Unsupported type of PaneType.", "unknown");
	}

	parent->setAttribute(attrib_name, result);
}


const bool PaneType::fromXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name)
{
	_bstr_t in_value = getStructAttribute(parent, attrib_name);

	if(_bstr_t(L"bottomRight") == in_value)
	{
		val = REVTPNNBOTRIGHT;
	}
	else if(_bstr_t(L"topRight") == in_value)
	{
		val = REVTPNNTOPRIGHT;
	}
	else if(_bstr_t(L"bottomLeft") == in_value)
	{
		val = REVTPNNBOTLEFT;
	}
	else if(_bstr_t(L"topLeft") == in_value)
	{
		val = REVTPNNTOPLEFT;
	}
	else
	{
		throw EXCEPT::RT::WrongBiffRecord("Unsupported type of PaneType.", "unknown");
	}
	return true;
}


} // namespace XLS

