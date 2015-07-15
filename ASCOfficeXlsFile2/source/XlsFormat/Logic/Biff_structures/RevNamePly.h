#pragma once

#include "BiffStructure.h"
#include "BiffString.h"
#include "RevLblName.h"

namespace XLS
{;

class CFRecord;

class RevNamePly : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(RevNamePly)
public:
	RevNamePly();
	RevNamePly(CFRecord& record);
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	RevSheetName sheet;
	RevLblName name;
};

} // namespace XLS

