#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/DXFN.h>
#include <Logic/Biff_structures/XFExtNoFRT.h>

namespace XLS
{;

class CFRecord;

class DXFN12 : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DXFN12)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeDXFN12;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned int cbDxf;
	DXFNPtr dxfn;
	XFExtNoFRTPtr xfext;
};

} // namespace XLS

