#pragma once

#include "../BiffStructure.h"

namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{;

class OfficeArtIDCL : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtIDCL)
public:
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeOfficeArtIDCL;
	
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

public:
	unsigned int dgid;
	unsigned int cspidCur;
};

typedef boost::shared_ptr<OfficeArtIDCL> OfficeArtIDCLPtr;


} // namespace XLS
