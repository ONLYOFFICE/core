#pragma once

#include "../Biff_records/BiffRecord.h"
#include "BiffString.h"


namespace XLS
{;

class AFDOperStr : public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(AFDOperStr)
public:
	BiffStructurePtr clone();

	AFDOperStr();
	AFDOperStr(bool bAutoFilter);
	~AFDOperStr();

	static const ElementType	type = typeAFDOperStr;
	
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	BIFF_BYTE cch;
	BIFF_BYTE fCompare;	

	bool m_bAutoFilter;
};

} // namespace XLS
