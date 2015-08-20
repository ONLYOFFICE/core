#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{

class CFRecord;

class RPHSSub : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(RPHSSub)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeRPHSSub;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	const size_t getSize() const;
	const unsigned short getRunsNumber() const;

	unsigned short crun;
	LPWideString st;

	int sz;
};

} // namespace XLS

