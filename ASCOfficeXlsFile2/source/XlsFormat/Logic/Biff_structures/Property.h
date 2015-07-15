#pragma once

#include "BiffStructure.h"

namespace OSHARED
{;

class CFRecord;

class Property : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Property)
public:
	XLS::BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	unsigned int keyIndex;
	unsigned int valueIndex;
};

} // namespace OSHARED

