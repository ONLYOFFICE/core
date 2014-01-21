#pragma once

#include <XLS_logic/Biff_records/BiffRecord.h>
#include <XLS_logic/Biff_structures/BiffString.h>
#include "Run.h"
#include "TxOLastRun.h"

namespace XLS
{;

class TxORuns: public BiffStructTagged
{	
	BASE_OBJECT_DEFINE_CLASS_NAME(TxORuns)
public:
	BiffStructurePtr clone();

	TxORuns();
	~TxORuns();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

public:
	int m_runCount;
	std::vector<RunPtr> rgTxoRuns;
	TxOLastRun lastRun;
};

typedef boost::shared_ptr<TxORuns> TxORunsPtr;

} // namespace XLS