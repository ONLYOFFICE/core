
#include "DXFNum.h"
#include <Binary/CFRecord.h>
//#include <Exception/StructureParameterNotSet.h>

namespace XLS
{;


void DXFNum::setIsUserDefined(const bool is_user_defined)
{
	is_user_defined_ = is_user_defined;
}


const bool DXFNum::getIsUserDefined() const
{
	return is_user_defined_;
}


BiffStructurePtr DXFNum::clone()
{
	return BiffStructurePtr(new DXFNum(*this));
}


//void DXFNum::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	if(is_user_defined_)
//	{
//		user_defined.toXML(xml_tag);
//	}
//	else
//	{
//		xml_tag->setAttribute(L"ifmt", fmt_id.ifmt);
//	}
//}


//void DXFNum::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	if(is_user_defined_)
//	{
//		user_defined.fromXML(xml_tag);
//	}
//	else
//	{
//		fmt_id.ifmt = getStructAttribute(xml_tag, L"ifmt");
//	}
//}


void DXFNum::store(CFRecord& record)
{
	if(is_user_defined_)
	{
		record << user_defined;
	}
	else
	{
		record << fmt_id;
	}
}


void DXFNum::load(CFRecord& record)
{
	if(is_user_defined_)
	{
		record >> user_defined;
	}
	else
	{
		record >> fmt_id;
	}
}


} // namespace XLS

