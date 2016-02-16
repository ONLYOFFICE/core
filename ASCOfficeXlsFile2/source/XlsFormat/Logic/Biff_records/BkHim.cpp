
#include "BkHim.h"

namespace XLS
{

BkHim::BkHim()
{
}


BkHim::~BkHim()
{
}


BaseObjectPtr BkHim::clone()
{
	return BaseObjectPtr(new BkHim(*this));
}


void BkHim::writeFields(CFRecord& record)
{

}


void BkHim::readFields(CFRecord& record)
{
	record >> cf;
	record.skipNunBytes(2);
	record >> lcb;//imageBlob size

	unsigned int sz = record.getDataSize() - record.getRdPtr();

	if (cf == 0x09)
	{
		//bitmap
	}
	else
	{
		//native
	}

	if (lcb < 1) return;
	
	std::list<CFRecordPtr>& recs = continue_records[rt_Continue];

	while (record.getRdPtr() + lcb > record.getDataSize() && !recs.empty())
	{
		record.appendRawData(recs.front());
		recs.pop_front();
	}

	if (record.checkFitReadSafe(lcb))
	{
		pData = boost::shared_array<char>(new char[lcb]);

		memcpy(pData.get(), record.getCurData<char>(), lcb);

		record.skipNunBytes(lcb);
	}
}

} // namespace XLS

