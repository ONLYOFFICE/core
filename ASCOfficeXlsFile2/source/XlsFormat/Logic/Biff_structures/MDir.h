#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class MDir : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(MDir)
public:
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	long imdt;
	unsigned __int16 mdd;
};

typedef boost::shared_ptr<MDir> MDirPtr;

} // namespace XLS
