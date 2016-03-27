
#include "PtgArea.h"
#include "CellRangeRef.h"
#include "PtgParam.h"
#include <Binary/CFRecord.h>

namespace XLS
{


PtgArea::PtgArea()
{
}


PtgArea::PtgArea(const std::wstring& word, const PtgDataType data_type)
:	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5)),
	area(word)
{
}


BiffStructurePtr PtgArea::clone()
{
	return BiffStructurePtr(new PtgArea(*this));
}


void PtgArea::storeFields(CFRecord& record)
{
	record << area;
}


void PtgArea::loadFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x600)
	{
		unsigned char	colFirst, colLast;
		_UINT16			rwFirst, rwLast;

		record >> rwFirst >> rwLast >> colFirst >> colLast;

		area.rowFirstRelative	= rwFirst & 0x8000;
		area.columnLastRelative	= rwFirst & 0x4000;
		
		area.columnFirst		= colFirst;
		area.rowFirst			= rwFirst & 0x3FFF;

		area.rowLastRelative	= rwLast & 0x8000;
		area.columnLastRelative	= rwLast & 0x4000;
		
		area.columnLast			= colLast;
		area.rowLast			= rwLast & 0x3FFF;
	}
	else
		record >> area;
}


void PtgArea::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	RgceArea range(area);
	if(!ptg_stack.empty())
	{
		// Check whether we should process PtgElfRadical's value
		PtgParam param(ptg_stack.top());
		if(param.getType() == PtgParam::ptELF_RADICAL)
		{
			ptg_stack.pop();
			if(range.getColumnFirst() == range.getColumnLast())
			{
				range.setColumnRelativity(0 != param.getFirstParam());
			}
			if(range.getRowFirst() == range.getRowLast())
			{
				range.setRowRelativity(0 != param.getFirstParam());
			}
		}
	}
	ptg_stack.push(range.toString());
}

} // namespace XLS

