
#include "AutoFilter12.h"

namespace XLS
{

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
	_UINT16 flags;
	_UINT32 unused2;
	
	record >> frtRefHeader >> iEntry >> fHideArrow >> ft >> cft >> cCriteria >> cDateGroupings >> flags >> unused2 >> idList;

	_UINT16 _iEntry = iEntry;
	_UINT32 _fHideArrow = fHideArrow;
	_UINT32 _ft = ft;
	_UINT32 _cft = cft;
	_UINT32 _cCriteria = cCriteria;
	_UINT32 _cDateGroupings = cDateGroupings;
	_UINT32 _idList = idList;	

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

