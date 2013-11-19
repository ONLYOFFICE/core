#pragma once

#include "BiffStructTagged.h"
#include <XLS_logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class Stxp : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Stxp)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	long twpHeight;
	Ts ts;
	short bls;
	short sss;
	BYTE uls;
	BYTE bFamily;
	BYTE bCharSet;
};

} // namespace XLS

