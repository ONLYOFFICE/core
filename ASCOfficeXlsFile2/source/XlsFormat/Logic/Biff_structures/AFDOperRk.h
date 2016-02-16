#pragma once

#include <Logic/Biff_records/BiffRecord.h>

namespace XLS
{

class AFDOperRk: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(AFDOperRk)
public:
	BiffStructurePtr clone();
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);	

	static const ElementType	type = typeAFDOperRk;

	BIFF_DWORD rk;
};

} // namespace XLS