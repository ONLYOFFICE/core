
#include "CHART.h"
#include <Logic/Biff_records/BOF.h>
#include <Logic/ChartSheetSubstream.h>
#include <Logic/Biff_records/Continue.h>

namespace XLS
{

CHART::~CHART()
{
}

// CHART = CHARTSHEET *Continue

BaseObjectPtr CHART::clone()
{
	return BaseObjectPtr(new CHART(*this));
}

const bool CHART::loadContent(BinProcessor& proc)
{
	unsigned short ss_type;
	if(!proc.getNextSubstreamType(ss_type) || BOF::st_Chart != ss_type)
	{
		return false;
	}
	if(!proc.mandatory<ChartSheetSubstream>())
	{
		return false;
	}

	m_ChartSheetSubstream = elements_.back();
	elements_.pop_back();

	int count = proc.repeated<Continue>(0, 0);

	while (count > 0)
	{
		Continue* c = dynamic_cast<Continue*>(elements_.back().get());
		if (c)
		{
			CFRecordPtr r = CFRecordPtr(new CFRecord(CFRecordType::ANY_TYPE, proc.getGlobalWorkbookInfo()));
			r->appendRawData(c->m_pData, c->m_iDataSize);
			mso_drawing_->storeRecordAndDecideProceeding(r);

			sp_enabled = true;
			
		}
		elements_.pop_back();
		count--;
	}

//	reader.SeekNextSubstream();
	return true;
}

} // namespace XLS

