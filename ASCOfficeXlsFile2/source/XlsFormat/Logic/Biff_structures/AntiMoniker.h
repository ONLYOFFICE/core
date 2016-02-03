#pragma once

#include "BiffStructure.h"

namespace OSHARED
{

class CFRecord;

class AntiMoniker : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(AntiMoniker)
public:
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeAntiMoniker;

	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	_UINT32 count;
};

} // namespace OSHARED

