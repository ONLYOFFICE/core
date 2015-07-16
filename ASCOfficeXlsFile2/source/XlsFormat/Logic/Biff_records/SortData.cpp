#include "precompiled_xls.h"
#include "SortData.h"

namespace XLS
{;

SortData::SortData()
{
}


SortData::~SortData()
{
}


BaseObjectPtr SortData::clone()
{
	return BaseObjectPtr(new SortData(*this));
}


void SortData::writeFields(CFRecord& record)
{
#pragma message("####################### SortData record is not implemented")
	Log::error("SortData record is not implemented.");
	//record << some_value;
}


void SortData::readFields(CFRecord& record)
{
	record.skipNunBytes(12);
#pragma message("############################ frtHeader skipped here")
	unsigned short flags;
	record >> flags;
	fCol = GETBIT(flags, 0);
	fCaseSensitive = GETBIT(flags, 1);
	fAltMethod = GETBIT(flags, 2);
	sfp = GETBITS(flags, 3, 5);
	RFX rfx_orig;
	record >> rfx_orig >> cconditions;
	rfx = static_cast<std::wstring >(rfx_orig);
	record >> idParent;
	std::list<CFRecordPtr>& recs = continue_records[rt_ContinueFrt12];
	while(!recs.empty())
	{
		#pragma message("############################ frtRefHeader skipped here")
		record.appendRawData(recs.front()->getData() + 12, recs.front()->getDataSize() - 12);
		recs.pop_front();
	}
	for(unsigned int i = 0; i < cconditions; ++i)
	{
		SortCond12Ptr sort_cond(new SortCond12);
		record >> *sort_cond;
		sortCond12Array.push_back(sort_cond);
	}

}

} // namespace XLS

