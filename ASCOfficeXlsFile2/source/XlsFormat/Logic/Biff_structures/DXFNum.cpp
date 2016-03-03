
#include "DXFN.h"
#include <Binary/CFRecord.h>

#include <utils.h>

namespace XLS
{


void DXFNum::setIsUserDefined(const bool is_user_defined)
{
	is_user_defined_ = is_user_defined;
}


const bool DXFNum::getIsUserDefined() const
{
    return true == is_user_defined_;
}


BiffStructurePtr DXFNum::clone()
{
	return BiffStructurePtr(new DXFNum(*this));
}


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


int DXFNum::serialize(std::wostream & stream)
{
	if (parent->ifmtNinch && parent->fIfmtUser) return 0;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"numFmt")
		{	
			if (!parent->ifmtNinch)
				CP_XML_ATTR(L"numFmtId", fmt_id.ifmt);
 			if (!parent->fIfmtUser)
 				CP_XML_ATTR(L"formatCode", /*xml::utils::replace_text_to_xml*/(user_defined.fmt.value()));
		}
	}
	return 0;
}



} // namespace XLS

