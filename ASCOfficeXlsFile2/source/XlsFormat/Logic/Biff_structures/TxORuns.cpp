
#include "TxORuns.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr TxORuns::clone()
{
	return BiffStructurePtr(new TxORuns(*this));
}

TxORuns::TxORuns()
{		
	m_runCount = 0;
}

TxORuns::~TxORuns()
{
}


void TxORuns::store(CFRecord& record)
{	
}

void TxORuns::load(CFRecord& record)
{
	for (int i = 0; i < m_runCount; i++)
	{
		RunPtr run(new Run);
		run->load(record);
		rgTxoRuns.push_back(run);
	}
	lastRun.load(record);
}


} // namespace XLS

