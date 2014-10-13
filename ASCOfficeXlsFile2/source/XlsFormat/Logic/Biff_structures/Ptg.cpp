#include "precompiled_xls.h"
#include "Ptg.h"
#include <Binary/CFRecord.h>
//#include <Exception/UnexpectedProgramPath.h>


namespace XLS
{;

Ptg::Ptg()
{
}


Ptg::Ptg(const unsigned __int16 ptg_id_init)
:	ptg_id(ptg_id_init)
{
}


const unsigned __int16 Ptg::getPtgId() const
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


void Ptg::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{

}


void Ptg::store(CFRecord& record)
{
	offset_in_record = record.getDataSize();
	unsigned __int16 full_type = getPtgId();
	unsigned char low_part = static_cast<unsigned char>(full_type);
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
	unsigned char short_type;
	record >> short_type;
	ptg_id = short_type;
	if(0x18 == short_type || 0x19 == short_type)
	{
		unsigned char high_part;
		record >> high_part;
		ptg_id = static_cast<unsigned __int16>(short_type) + (static_cast<unsigned __int16>(high_part) << 8);
	}
	loadFields(record);
	size_of_struct = record.getRdPtr() - offset_in_record;
}



} // namespace XLS

