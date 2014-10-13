#include "precompiled_xls.h"
#include "CF.h"

namespace XLS
{;

CF::CF(const CellRef& cell_base_ref)
:	rgce1(cell_base_ref),
	rgce2(cell_base_ref)
{
}


CF::~CF()
{
}


BaseObjectPtr CF::clone()
{
	return BaseObjectPtr(new CF(*this));
}


void CF::writeFields(CFRecord& record)
{
	record << ct << cp;
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned __int16)/*cce*/);
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned __int16)/*cce*/);
	record << rgbdxf;
	rgce1.store(record);
	rgce2.store(record);
	record.registerDelayedDataSource(rgce1.getCCE(), rt_CF);
	record.registerDelayedDataSource(rgce2.getCCE(), rt_CF);
}


void CF::readFields(CFRecord& record)
{
	unsigned __int16 cce1;
	unsigned __int16 cce2;
	record >> ct >> cp >> cce1 >> cce2;
	record >> rgbdxf;
	rgce1.load(record, cce1);
	rgce2.load(record, cce2);
}

} // namespace XLS

