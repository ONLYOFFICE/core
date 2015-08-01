#pragma once

#include <Logic/Biff_records/BiffRecord.h>
#include <Logic/Biff_structures/BiffString.h>
#include "AFDOperStr.h"

namespace XLS
{;

class AFDOper: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(AFDOper)
public:
	BiffStructurePtr clone();

	AFDOper();
	AFDOper(bool bAutoFilter);
	~AFDOper();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	static const ElementType	type = typeAFDOper;

	BIFF_BYTE vt;
	BIFF_BYTE grbitSign;
	AFDOperStr vtValue;

	bool m_bAutoFilter;
};

} // namespace XLS