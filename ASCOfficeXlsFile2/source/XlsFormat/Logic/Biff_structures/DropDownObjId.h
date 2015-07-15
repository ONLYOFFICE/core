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

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	ObjId id;
};

typedef boost::shared_ptr<DropDownObjId> DropDownObjIdPtr;

} // namespace XLS

