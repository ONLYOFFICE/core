
#include "TxORuns.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr TxORuns::clone()
{
	return BiffStructurePtr(new TxORuns(*this));
}

TxORuns::TxORuns()
{		
	m_runCount = 0;
	recs = NULL;
}

TxORuns::~TxORuns()
{
	recs = NULL;
}


void TxORuns::store(CFRecord& record)
{	
}

void TxORuns::load(CFRecord& record)
{
	for (int i = 0; i < m_runCount; i++)
	{
		while (record.getRdPtr() + 8 > record.getDataSize())
		{
			if ((recs) && (!recs->empty()))
			{
				record.appendRawData(recs->front());
				recs->pop_front();
			}
			else return;
		}
		RunPtr run(new Run);
		run->load(record);
		rgTxoRuns.push_back(run);
	}
	while (record.getRdPtr() + 8 > record.getDataSize())
	{
		if ((recs) && (!recs->empty()))
		{
			record.appendRawData(recs->front());
			recs->pop_front();
		}
		else return;
	}
	lastRun.load(record);
}


} // namespace XLS

