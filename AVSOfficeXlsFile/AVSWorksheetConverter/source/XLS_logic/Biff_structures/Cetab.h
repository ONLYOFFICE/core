#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

#pragma pack(1)
class Cetab : public BiffStructure_NoVtbl
{
public:
	void toXML(MSXML2::IXMLDOMElementPtr parent);
	WORD getHighBit() const;
	void clrHighBit();
	const std::wstring getFuncName() const;

private:
	WORD cetab;
};
#pragma pack() // restore

} // namespace XLS

