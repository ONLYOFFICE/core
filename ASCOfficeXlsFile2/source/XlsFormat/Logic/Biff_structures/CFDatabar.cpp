
#include "CFDatabar.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr CFDatabar::clone()
{
	return BiffStructurePtr(new CFDatabar(*this));
}

void CFDatabar::store(CFRecord& record)
{

}

void CFDatabar::load(CFRecord& record)
{
	record.skipNunBytes(3); // unused
	
	unsigned char flags;
	record >> flags;

	fRightToLeft	= GETBIT(flags, 0);
	fShowValue		= GETBIT(flags, 1);

	record >> iPercentMin >> iPercentMax;

	record >> color;

	cfvoDB1.load(record);
	cfvoDB2.load(record);
}


} // namespace XLS

