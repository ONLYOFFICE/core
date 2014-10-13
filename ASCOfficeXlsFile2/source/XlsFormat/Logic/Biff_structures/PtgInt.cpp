#include "precompiled_xls.h"
#include "PtgInt.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


PtgInt::PtgInt()
{
}


PtgInt::PtgInt(const std::wstring& word)
:	OperandPtg(fixed_id)
{
	integer = _wtoi(word.c_str());
}


BiffStructurePtr PtgInt::clone()
{
	return BiffStructurePtr(new PtgInt(*this));
}

//
//void PtgInt::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"integer", integer);
//}


void PtgInt::storeFields(CFRecord& record)
{
	record << integer;
}


void PtgInt::loadFields(CFRecord& record)
{
	record >> integer;
}


void PtgInt::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
	ptg_stack.push(STR::int2wstr(integer));
}


} // namespace XLS

