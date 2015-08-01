#pragma once

#include <Logic/Biff_records/BiffRecord.h>
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;

class TxOLastRun: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(TxOLastRun)
public:
	BiffStructurePtr clone();

	TxOLastRun();
	~TxOLastRun();

	static const ElementType	type = typeTxOLastRun;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

public:	
	BIFF_WORD cchText;
};

typedef boost::shared_ptr<TxOLastRun> TxOLastRunPtr;

} // namespace XLS