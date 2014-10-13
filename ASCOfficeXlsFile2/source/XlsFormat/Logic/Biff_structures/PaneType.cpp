#include "precompiled_xls.h"
#include "PaneType.h"
//#include <Exception/WrongBiffRecord.h>

namespace XLS
{;


BiffStructurePtr PaneType::clone()
{
	return BiffStructurePtr(new PaneType(*this));
}


//void PaneType::toXML(BiffStructurePtr & parent, const std::wstring & attrib_name)
//{
//	_variant_t result;
//	switch(val)
//	{
//		case REVTPNNBOTRIGHT:
//			result = L"bottomRight";
//			break;
//		case REVTPNNTOPRIGHT:
//			result = L"topRight";
//			break;
//		case REVTPNNBOTLEFT:
//			result = L"bottomLeft";
//			break;
//		case REVTPNNTOPLEFT:
//			result = L"topLeft";
//			break;
//		default:
//			throw;// EXCEPT::RT::WrongBiffRecord("Unsupported type of PaneType.", "unknown");
//	}
//
//	parent->setAttribute(attrib_name, result);
//}
//
//
//const bool PaneType::fromXML(BiffStructurePtr & parent, const std::wstring & attrib_name)
//{
//	std::wstring  in_value = getStructAttribute(parent, attrib_name);
//
//	if(std::wstring (L"bottomRight") == in_value)
//	{
//		val = REVTPNNBOTRIGHT;
//	}
//	else if(std::wstring (L"topRight") == in_value)
//	{
//		val = REVTPNNTOPRIGHT;
//	}
//	else if(std::wstring (L"bottomLeft") == in_value)
//	{
//		val = REVTPNNBOTLEFT;
//	}
//	else if(std::wstring (L"topLeft") == in_value)
//	{
//		val = REVTPNNTOPLEFT;
//	}
//	else
//	{
//		throw;// EXCEPT::RT::WrongBiffRecord("Unsupported type of PaneType.", "unknown");
//	}
//	return true;
//}
//
//
} // namespace XLS

