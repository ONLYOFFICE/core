#pragma once

#include <Logic/Biff_records/BiffRecord.h>

namespace XLS
{

class AFDOperBoolErr: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(AFDOperBoolErr)
public:
	BiffStructurePtr clone();
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);	

	static const ElementType	type = typeAFDOperBoolErr;

	BIFF_SHORT bes;
};

} // namespace XLS