#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

#pragma pack(1)
class Cetab : public BiffStructure_NoVtbl
{
public:
	//void toXML(BiffStructurePtr & parent);
	unsigned __int16 getHighBit() const;
	void clrHighBit();
	const std::wstring getFuncName() const;

private:
	unsigned __int16 cetab;
};
#pragma pack() // restore

} // namespace XLS

