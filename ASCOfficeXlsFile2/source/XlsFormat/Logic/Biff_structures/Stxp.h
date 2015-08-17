#pragma once

#include "BiffStructure.h"
#include "BitMarkedStructs.h"

namespace XLS
{;

class CFRecord;

class Stxp : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Stxp)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeStxp;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	long twpHeight;
	Ts ts;
	short bls;
	short sss;
	unsigned char uls;
	unsigned char bFamily;
	unsigned char bCharSet;
};

} // namespace XLS

