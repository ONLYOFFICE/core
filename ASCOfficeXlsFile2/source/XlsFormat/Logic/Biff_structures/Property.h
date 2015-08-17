#pragma once

#include "BiffStructure.h"

namespace OSHARED
{

class CFRecord;

class Property : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Property)
public:
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeProperty;

	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	unsigned int keyIndex;
	unsigned int valueIndex;
};

} // namespace OSHARED

