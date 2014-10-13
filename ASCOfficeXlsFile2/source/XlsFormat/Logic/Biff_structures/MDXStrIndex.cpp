#include "precompiled_xls.h"
#include "MDXStrIndex.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr MDXStrIndex::clone()
{
	return BiffStructurePtr(new MDXStrIndex(*this));
}


//void MDXStrIndex::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"index", index);
//}
//
//
//void MDXStrIndex::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	index = getStructAttribute(xml_tag, L"index");
//}
//

void MDXStrIndex::store(CFRecord& record)
{
	record >> index;
}


void MDXStrIndex::load(CFRecord& record)
{
	record >> index;
}


} // namespace XLS

