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
	unsigned short getHighBit() const;
	void clrHighBit();
	const std::wstring getFuncName() const;

private:
	unsigned short cetab;
};
#pragma pack() // restore

} // namespace XLS

