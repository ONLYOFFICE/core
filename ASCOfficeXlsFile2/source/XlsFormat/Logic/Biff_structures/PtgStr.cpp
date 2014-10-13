#include "precompiled_xls.h"
#include "PtgStr.h"
#include <Binary/CFRecord.h>

namespace XLS
{;

PtgStr::PtgStr()
{
}


PtgStr::PtgStr(const std::wstring & str)
:	string_(str),
	OperandPtg(fixed_id)
{
}


BiffStructurePtr PtgStr::clone()
{
	return BiffStructurePtr(new PtgStr(*this));
}


//void PtgStr::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"string", string_);
//}


void PtgStr::storeFields(CFRecord& record)
{
	record << string_;
}


void PtgStr::loadFields(CFRecord& record)
{
	record >> string_;
}


void PtgStr::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
	ptg_stack.push(L"\"" + boost::algorithm::replace_all_copy(std::wstring(string_), L"\"", L"\"\"") + L"\"");
}


} // namespace XLS

