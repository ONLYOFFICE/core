#pragma once

#include <Logic/Biff_records/BiffRecord.h>
#include <Logic/Biff_structures/BiffString.h>
#include "Run.h"
#include "TxOLastRun.h"

namespace XLS
{

class TxORuns: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(TxORuns)
public:
	BiffStructurePtr clone();

	TxORuns();
	~TxORuns();

	static const ElementType	type = typeTxORuns;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	int m_runCount;
	std::vector<RunPtr> rgTxoRuns;
	TxOLastRun lastRun;

	void set_records(std::list<CFRecordPtr>* recs_)
	{
		recs = recs_;
	}

	std::list<CFRecordPtr>* recs;
};

typedef boost::shared_ptr<TxORuns> TxORunsPtr;

} // namespace XLS