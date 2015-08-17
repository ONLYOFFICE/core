
#include "PtgNum.h"
#include <Binary/CFRecord.h>

namespace XLS
{


PtgNum::PtgNum()
{
}


PtgNum::PtgNum(const std::wstring& word)
:	OperandPtg(fixed_id)
{
#if defined(_WIN32) || defined (_WIN64)
    value_ = _wtof(word.c_str());
#else
    wchar_t * pEnd;
    value_ = wcstod(word.c_str(), &pEnd);
#endif
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


void PtgNum::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data)
{
	ptg_stack.push(STR::double2str(value_));
}


} // namespace XLS

