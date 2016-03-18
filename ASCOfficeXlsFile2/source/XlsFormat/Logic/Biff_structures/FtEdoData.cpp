
#include "FtEdoData.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr FtEdoData::clone()
{
	return BiffStructurePtr(new FtEdoData(*this));
}


void FtEdoData::store(CFRecord& record)
{
	unsigned short ft = 0x0010; // reserved
	unsigned short cb = 0x0008; // reserved
	record << ft << cb;

	record << ivtEdit << fMultiLine << fVScroll << id;
}


void FtEdoData::load(CFRecord& record)
{
	record.skipNunBytes(4); // reserved

	record >> ivtEdit >> fMultiLine >> fVScroll >> id;
}


} // namespace XLS

