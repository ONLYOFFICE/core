
#include "AFDOperStr.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr AFDOperStr::clone()
{
	return BiffStructurePtr(new AFDOperStr(*this));
}

AFDOperStr::AFDOperStr()
{
	m_bAutoFilter = true;
}

AFDOperStr::AFDOperStr(bool bAutoFilter)
{
	m_bAutoFilter = bAutoFilter;
}


AFDOperStr::~AFDOperStr()
{
}

void AFDOperStr::store(CFRecord& record)
{	
}


void AFDOperStr::load(CFRecord& record)
{	
	if (m_bAutoFilter)		// AutoFilter
		record.skipNunBytes(4);

	record >> cch >> fCompare;

	unsigned char _cch = cch;
	unsigned char _fCompare = fCompare;

	record.skipNunBytes(1);	// reserved1
	record.skipNunBytes(1);	// unused2

	if (!m_bAutoFilter)		// AutoFilter12
		record.skipNunBytes(4);
}


} // namespace XLS

