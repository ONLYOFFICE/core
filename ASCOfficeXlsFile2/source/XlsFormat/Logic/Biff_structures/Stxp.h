#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class Stxp : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Stxp)
public:
	BiffStructurePtr clone();

	
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	long twpHeight;
	Ts ts;
	short bls;
	short sss;
	unsigned char uls;
	unsigned char bFamily;
	unsigned char bCharSet;
};

} // namespace XLS

