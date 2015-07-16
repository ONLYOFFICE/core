#include "precompiled_xls.h"
#include "CF12.h"
#include <Logic/Biff_structures/CFMultistate.h>

namespace XLS
{;

CF12::CF12(const CellRef& cell_base_ref)
:	rgce1(cell_base_ref),
	rgce2(cell_base_ref),
	fmlaActive(cell_base_ref)
{
}


CF12::~CF12()
{
}


BaseObjectPtr CF12::clone()
{
	return BaseObjectPtr(new CF12(*this));
}


void CF12::writeFields(CFRecord& record)
{
#pragma message("####################### CF12 record is not implemented")
	Log::error("CF12 record is not implemented.");
	//record << some_value;
}


void CF12::readFields(CFRecord& record)
{
	record >> frtRefHeader;
	record >> ct >> cp;
	unsigned short cce1;
	unsigned short cce2;
	record >> cce1 >> cce2;
	record >> dxf;
	rgce1.load(record, cce1);
	rgce2.load(record, cce2);
	fmlaActive.load(record);
	unsigned char flags;
	record >> flags;
	fStopIfTrue = GETBIT(flags, 1);
	record >> ipriority >> icfTemplate;
	unsigned char cbTemplateParm;
	record >> cbTemplateParm;
	record >> rgbTemplateParms;
	switch(ct)
	{
		case 0x03:
#pragma message("####################### CFGradient structure is not implemented")
			Log::info("CFGradient structure is not implemented.");
			//rgbCT = BiffStructurePtr(new CFGradient);
			//rgbCT->load(record);
			break;
		case 0x04:
#pragma message("####################### CFDatabar structure is not implemented")
			Log::info("CFDatabar structure is not implemented.");
			//rgbCT = BiffStructurePtr(new CFDatabar);
			//rgbCT->load(record);
			break;
		case 0x05:
#pragma message("####################### CFFilter structure is not implemented")
			Log::info("CFFilter structure is not implemented.");
			//rgbCT = BiffStructurePtr(new CFFilter);
			//rgbCT->load(record);
			break;
		case 0x06:
			rgbCT = BiffStructurePtr(new CFMultistate);
			rgbCT->load(record);
			break;
	}

}

} // namespace XLS

