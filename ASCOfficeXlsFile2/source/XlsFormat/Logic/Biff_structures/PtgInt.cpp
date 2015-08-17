
#include "PtgInt.h"
#include <Binary/CFRecord.h>

namespace XLS
{


PtgInt::PtgInt()
{
}


PtgInt::PtgInt(const std::wstring& word)
:	OperandPtg(fixed_id)
{
#if defined(_WIN32) || defined (_WIN64)
    integer = _wtoi(word.c_str());
#else
     wchar_t * pEnd;
    integer = wcstol(word.c_str(), &pEnd ,10);
#endif
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


void PtgInt::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data)
{
	ptg_stack.push(STR::int2wstr(integer));
}


} // namespace XLS

