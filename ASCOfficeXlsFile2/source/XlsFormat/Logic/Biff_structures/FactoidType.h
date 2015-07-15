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

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	unsigned int id;
	PBString rgbUri;
	PBString rgbTag;
	PBString rgbDownLoadURL;
};

} // namespace OSHARED

