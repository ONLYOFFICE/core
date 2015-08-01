#pragma once

#include "BiffStructure.h"
//#include <Logic/Biff_structures/Phs.h>
//#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class XORObfuscation : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(XORObfuscation)
public:
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	static const ElementType	type = typeXORObfuscation;

	unsigned short key;
	unsigned short verificationBytes;
};

} // namespace XLS

