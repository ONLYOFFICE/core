#pragma once

#include "BiffStructure.h"
//#include <Logic/Biff_structures/Phs.h>
//#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class XORObfuscation : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(XORObfuscation)
public:
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned __int16 key;
	unsigned __int16 verificationBytes;
};

} // namespace XLS

