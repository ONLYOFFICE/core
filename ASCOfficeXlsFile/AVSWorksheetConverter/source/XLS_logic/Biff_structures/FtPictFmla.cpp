#include "stdafx.h"
#include "FtPictFmla.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


FtPictFmla::FtPictFmla()
:	fmla(true)
{
}

BiffStructurePtr FtPictFmla::clone()
{
	return BiffStructurePtr(new FtPictFmla(*this));
}


void FtPictFmla::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
{
	fmla.toXML(own_tag);
	own_tag->setAttribute(L"lPosInCtlStm", lPosInCtlStm);
	own_tag->setAttribute(L"cbBufInCtlStm", cbBufInCtlStm);
	key.toXML(own_tag);
}


void FtPictFmla::getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
{
	fmla.fromXML(own_tag);
	lPosInCtlStm = getStructAttribute(own_tag, L"lPosInCtlStm");
	cbBufInCtlStm = getStructAttribute(own_tag, L"cbBufInCtlStm");
	key.fromXML(own_tag);
}


void FtPictFmla::store(CFRecord& record, FtPioGrbit& pictFlags)
{
	WORD ft = 0x0009; // reserved
	record << ft;
	record.registerDelayedDataReceiver(NULL, sizeof(WORD)/*cbFmla*/);
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
	record.skipNBytes(4); // reserved

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

