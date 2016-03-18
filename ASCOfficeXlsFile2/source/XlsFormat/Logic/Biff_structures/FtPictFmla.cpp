
#include "FtPictFmla.h"
#include <Binary/CFRecord.h>

namespace XLS
{


FtPictFmla::FtPictFmla()
:	fmla(true)
{
}

BiffStructurePtr FtPictFmla::clone()
{
	return BiffStructurePtr(new FtPictFmla(*this));
}



void FtPictFmla::store(CFRecord& record, FtPioGrbit& pictFlags)
{
	unsigned short ft = 0x0009; // reserved
	record << ft;
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned short)/*cbFmla*/);
	size_t start_ptr = record.getDataSize();

	fmla.store(record);

	if(fmla.fmla.HasPtgTbl())
	{
		record << lPosInCtlStm;
	}
	if(pictFlags.fPrstm)
	{
		record << cbBufInCtlStm;
	}
	if(pictFlags.fPrstm)
	{
		key.store(record);
	}

	size_t data_size = record.getDataSize() - start_ptr;
	record.registerDelayedDataSource(data_size, rt_Obj);
}


void FtPictFmla::load(CFRecord& record, FtPioGrbit& pictFlags)
{
	record.skipNunBytes(4); // reserved

	fmla.load(record);

	if(fmla.fmla.HasPtgTbl())
	{
		record >> lPosInCtlStm;
	}
	record >> lPosInCtlStm;
	if(pictFlags.fPrstm)
	{
		record >> cbBufInCtlStm;
	}
	if(pictFlags.fPrstm)
	{
		key.load(record);
	}
}


} // namespace XLS

