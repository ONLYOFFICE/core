
#include "PtgStr.h"
#include <Binary/CFRecord.h>

#include <boost/algorithm/string.hpp>

namespace XLS
{

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


void PtgStr::storeFields(CFRecord& record)
{
	record << string_;
}


void PtgStr::loadFields(CFRecord& record)
{
	record >> string_;
}


void PtgStr::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	ptg_stack.push(L"\"" + boost::algorithm::replace_all_copy(std::wstring(string_), L"\"", L"\"\"") + L"\"");
}


} // namespace XLS

