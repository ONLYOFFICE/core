#pragma once

#include "BiffStructure.h"
#include "ObjFmla.h"

namespace XLS
{;

class CFRecord;

class ObjLinkFmla : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ObjLinkFmla)
public:
	ObjLinkFmla();
	BiffStructurePtr clone();

	static const ElementType	type = typeObjLinkFmla;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record, const unsigned short ot);

	virtual void store(CFRecord& record){}

	ObjFmla fmla;
};

} // namespace XLS

