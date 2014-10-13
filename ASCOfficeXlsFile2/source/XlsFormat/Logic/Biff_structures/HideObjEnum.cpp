#include "precompiled_xls.h"
#include "HideObjEnum.h"
//#include <Exception/WrongBiffRecord.h>

namespace XLS
{;


BiffStructurePtr HideObjEnum::clone()
{
	return BiffStructurePtr(new HideObjEnum(*this));
}


//void HideObjEnum::toXML(BiffStructurePtr & parent, const std::wstring & attrib_name)
//{
//	std::wstring  out_value;
//	switch(val)
//	{
//		case SHOWALL:
//			out_value = L"all";
//			break;
//		case SHOWPLACEHOLDER:
//			out_value = L"placeholders";
//			break;
//		case HIDEALL:
//			out_value = L"none";
//			break;
//		default:
//			throw;// EXCEPT::RT::WrongBiffRecord("Unsupported type of HideObjEnum.", "unknown");
//	}
//
//	parent->setAttribute(attrib_name, out_value);
//}
//
//
//const bool HideObjEnum::fromXML(BiffStructurePtr & parent, const std::wstring & attrib_name)
//{
//	std::wstring  in_value = getStructAttribute(parent, attrib_name);
//
//	if(std::wstring (L"all") == in_value)
//	{
//		val = SHOWALL;
//	}
//	else if(std::wstring (L"placeholders") == in_value)
//	{
//		val = SHOWPLACEHOLDER;
//	}
//	else if(std::wstring (L"none") == in_value)
//	{
//		val = HIDEALL;
//	}
//	else
//	{
//		throw;// EXCEPT::RT::WrongBiffRecord("Unsupported type of HideObjEnum.", "unknown");
//	}
//	return true;
//}
//
//
} // namespace XLS

