
#include "IMDATA.h"

namespace XLS
{

IMDATA::IMDATA()
{
}


IMDATA::~IMDATA()
{
}


BaseObjectPtr IMDATA::clone()
{
	return BaseObjectPtr(new IMDATA(*this));
}


void IMDATA::writeFields(CFRecord& record)
{

}


void IMDATA::readFields(CFRecord& record)
{
	record >> cf;
	record >> env;
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

