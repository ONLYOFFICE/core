
#include "AutoFilter12.h"

namespace XLS
{;

AutoFilter12::AutoFilter12()
{
}


AutoFilter12::~AutoFilter12()
{
}


BaseObjectPtr AutoFilter12::clone()
{
	return BaseObjectPtr(new AutoFilter12(*this));
}


void AutoFilter12::writeFields(CFRecord& record)
{
}


void AutoFilter12::readFields(CFRecord& record)
{
	unsigned short flags;
	unsigned int unused2;
	record >> frtRefHeader >> iEntry >> fHideArrow >> ft >> cft >> cCriteria >> cDateGroupings >> flags >> unused2 >> idList;

	unsigned short _iEntry = iEntry;
	unsigned int _fHideArrow = fHideArrow;
	unsigned int _ft = ft;
	unsigned int _cft = cft;
	unsigned int _cCriteria = cCriteria;
	unsigned int _cDateGroupings = cDateGroupings;
	unsigned int _idList = idList;	

	// TODO доделать
	record.skipNunBytes(record.getDataSize() - record.getRdPtr());

	std::list<CFRecordPtr>& recs = continue_records[rt_ContinueFrt12];
	size_t size = recs.size();

	while( !recs.empty() )
	{
		record.appendRawData(recs.front());
		recs.pop_front();

		//
		FrtRefHeader frtRefHeaderContinue;
		record >> frtRefHeaderContinue;
		if (frtRefHeaderContinue.rt == 0x087F)
		{		
			if ( (ft == BIFF_DWORD(0)) && (cCriteria > 0) )
			{
				AF12CriteriaPtr item(new AF12Criteria);
				item->load(record);
				rgbAF12Criteries.push_back(item);
			}
		}		
	}
}

} // namespace XLS

