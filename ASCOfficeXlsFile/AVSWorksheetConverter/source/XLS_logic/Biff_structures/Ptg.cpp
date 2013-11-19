#include "stdafx.h"
#include "Ptg.h"
#include <XLS_bin/CFRecord.h>
#include <Exception/UnexpectedProgramPath.h>


namespace XLS
{;

Ptg::Ptg()
{
}


Ptg::Ptg(const WORD ptg_id_init)
:	ptg_id(ptg_id_init)
{
}


const WORD Ptg::getPtgId() const
{
	return ptg_id;
}


const size_t Ptg::getOffsetInRecord() const
{
	return offset_in_record;
}


const size_t Ptg::getSizeOfStruct() const
{
	return size_of_struct;
}


void Ptg::addFuncWrapper(AssemblerStack& ptg_stack, const std::wstring& func_name)
{
	ptg_stack.top() = func_name + L'(' + ptg_stack.top() + L')';
}


void Ptg::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{

}


void Ptg::store(CFRecord& record)
{
	offset_in_record = record.getDataSize();
	WORD full_type = getPtgId();
	BYTE low_part = static_cast<BYTE>(full_type);
	if(0x18 == low_part || 0x19 == low_part)
	{
		record << full_type;
	}
	else
	{
		record << low_part;
	}
	storeFields(record);
	size_of_struct = record.getDataSize() - offset_in_record;
}


void Ptg::load(CFRecord& record)
{
	offset_in_record = record.getRdPtr();
	BYTE short_type;
	record >> short_type;
	ptg_id = short_type;
	if(0x18 == short_type || 0x19 == short_type)
	{
		BYTE high_part;
		record >> high_part;
		ptg_id = static_cast<WORD>(short_type) + (static_cast<WORD>(high_part) << 8);
	}
	loadFields(record);
	size_of_struct = record.getRdPtr() - offset_in_record;
}



} // namespace XLS

