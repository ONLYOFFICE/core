#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class DropDownObjId : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DropDownObjId)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeDropDownObjId;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	ObjId id;
};

typedef boost::shared_ptr<DropDownObjId> DropDownObjIdPtr;

} // namespace XLS

