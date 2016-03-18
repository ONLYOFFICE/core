
#include "MOper.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr MOper::clone()
{
	return BiffStructurePtr(new MOper(*this));
}


void MOper::store(CFRecord& record)
{
#pragma message("####################### MOper record has no BiffStructure::store() implemented")
	Log::error(" Error!!! MOper record has no BiffStructure::store() implemented.");
	//record << something;
}


void MOper::load(CFRecord& record)
{
	record >> colLast >> rowLast;
	for(int i = 0; i < (colLast + 1) * (rowLast + 1); ++i)
	{
		unsigned char rec_type;
		record >> rec_type;
		SerArPtr ser(SerAr::createSerAr(rec_type));
		record >> *ser;
		extOper.push_back(ser);
	}
}


} // namespace XLS

