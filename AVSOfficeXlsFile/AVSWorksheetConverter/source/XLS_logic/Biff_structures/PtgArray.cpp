#include "stdafx.h"
#include "PtgArray.h"
#include "PtgExtraArray.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


PtgArray::PtgArray()
{
}


PtgArray::PtgArray(const PtgDataType data_type)
:	OperandPtg(fixed_id | (static_cast<BYTE>(data_type) << 5))
{
}


BiffStructurePtr PtgArray::clone()
{
	return BiffStructurePtr(new PtgArray(*this));
}


void PtgArray::storeFields(CFRecord& record)
{
	record.reserveNBytes(7); // unused
}


void PtgArray::loadFields(CFRecord& record)
{
	record.skipNBytes(7); // unused
}


void PtgArray::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
	std::wstring array_string;

	PtgExtraArrayPtr extra_array;

	if(!extra_data.empty() && (extra_array = boost::dynamic_pointer_cast<PtgExtraArray>(extra_data.front())))
	{
		array_string = L"{" + extra_array->toString() + L"}";
		extra_data.pop();
	}

	ptg_stack.push(array_string);
}


} // namespace XLS

