#pragma once

#include <Logic/Biff_records/BiffRecord.h>
#include <Logic/Biff_structures/BiffString.h>
#include "AFDOper.h"

namespace XLS
{;

class AF12Criteria: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(AF12Criteria)
public:
	BiffStructurePtr clone();

	AF12Criteria();
	~AF12Criteria();

	
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

public:
	AFDOper doper;
	XLUnicodeStringNoCch str;
	BIFF_BSTR _str;
};

typedef boost::shared_ptr<AF12Criteria> AF12CriteriaPtr;

} // namespace XLS