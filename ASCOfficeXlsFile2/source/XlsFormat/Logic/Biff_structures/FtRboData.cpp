
#include "FtRboData.h"
#include <Binary/CFRecord.h>
//#include <Exception/AttributeDataWrong.h>

namespace XLS
{


BiffStructurePtr FtRboData::clone()
{
	return BiffStructurePtr(new FtRboData(*this));
}


void FtRboData::store(CFRecord& record)
{
	unsigned short ft = 0x0011; // reserved
	unsigned short cb = 0x0004; // reserved
	record << ft << cb;

	record << idRadNext << fFirstBtn;
}


void FtRboData::load(CFRecord& record)
{
	record.skipNunBytes(4); // reserved

	record >> idRadNext >> fFirstBtn;
}


} // namespace XLS

