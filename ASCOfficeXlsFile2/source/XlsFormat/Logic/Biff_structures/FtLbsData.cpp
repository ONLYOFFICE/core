
#include "FtLbsData.h"
#include <Binary/CFRecord.h>

namespace XLS
{


FtLbsData::FtLbsData()
:	fmla(false)
{
}

BiffStructurePtr FtLbsData::clone()
{
	return BiffStructurePtr(new FtLbsData(*this));
}

void FtLbsData::store(CFRecord& record, const unsigned short ot)
{
	unsigned short ft = 0x0013; // reserved
	record << ft;
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned short)/*cbFmla*/);
	size_t start_ptr = record.getDataSize();

	fmla.store(record);

	unsigned short flags = 0;
	SETBIT(flags, 0, fUseCB);
	SETBIT(flags, 1, fValidPlex);
	SETBIT(flags, 2, fValidIds);
	SETBIT(flags, 3, fNo3d);
	SETBITS(flags, 4, 5, wListSelType);
	SETBITS(flags, 8, 15, lct);

	record << cLines << iSel << flags << idEdit;
	if(dropData)
	{
		dropData->store(record);
	}

	if(fValidPlex)
	{
		record << rgLines;
	}

	if (0 != wListSelType)
	{
		record << bsels;
	}

	size_t data_size = record.getDataSize() - start_ptr;
	record.registerDelayedDataSource(data_size, rt_Obj);
}


void FtLbsData::load(CFRecord& record, const unsigned short ot)
{
	record.skipNunBytes(4); // reserved

	fmla.load(record);

	unsigned short flags;
	record >> cLines >> iSel >> flags >> idEdit;

	fUseCB = GETBIT(flags, 0);
	fValidPlex = GETBIT(flags, 1);
	fValidIds = GETBIT(flags, 2);
	fNo3d = GETBIT(flags, 3);
	wListSelType = GETBITS(flags, 4, 5);
	lct = GETBITS(flags, 8, 15);

	if(0x0014 == ot)
	{
		dropData.reset(new LbsDropData);
		dropData->load(record);
	}
	if(fValidPlex)
	{
		for(int i = 0; i < cLines; ++i)
		{
			XLUnicodeString str;
			record >> str;
			rgLines.push_back(str);
		}
	}

	if (0 != wListSelType)
	{
		for(int i = 0; i < cLines; ++i)
		{
			Boolean<unsigned char> bsel;
			record >> bsel;
			bsels.push_back(bsel);
		}
	}




}


} // namespace XLS

