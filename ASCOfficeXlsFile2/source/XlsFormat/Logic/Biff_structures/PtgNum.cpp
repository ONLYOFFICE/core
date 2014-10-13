#include "precompiled_xls.h"
#include "PtgNum.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


PtgNum::PtgNum()
{
}


PtgNum::PtgNum(const std::wstring& word)
:	OperandPtg(fixed_id)
{
	value_ = _wtof(word.c_str());
}


BiffStructurePtr PtgNum::clone()
{
	return BiffStructurePtr(new PtgNum(*this));
}


//void PtgNum::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"value", value_);
//}


void PtgNum::storeFields(CFRecord& record)
{
	record << value_;
}


void PtgNum::loadFields(CFRecord& record)
{
	record >> value_;
}


void PtgNum::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
	ptg_stack.push(STR::double2str(value_));
}


} // namespace XLS

