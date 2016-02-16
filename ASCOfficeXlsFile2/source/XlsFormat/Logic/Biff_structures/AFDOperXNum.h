#pragma once

#include <Logic/Biff_records/BiffRecord.h>

namespace XLS
{

class AFDOperXNum: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(AFDOperXNum)
public:
	BiffStructurePtr clone();
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);	

	static const ElementType	type = typeAFDOperXNum;

	double val;
};

} // namespace XLS