#pragma once

#include "BiffStructure.h"
//#include <Logic/Biff_structures/Phs.h>
#include <Logic/Biff_structures/BiffString.h>

namespace OSHARED
{;

class CFRecord;

class FactoidType : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FactoidType)
public:
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeFactoidType;

	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	unsigned int id;
	PBString rgbUri;
	PBString rgbTag;
	PBString rgbDownLoadURL;
};

} // namespace OSHARED

