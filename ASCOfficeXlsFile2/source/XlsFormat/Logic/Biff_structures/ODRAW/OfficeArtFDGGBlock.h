#pragma once

#include "OfficeArtRecord.h"
#include "OfficeArtIDCL.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{;

class OfficeArtFDGGBlock : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtFDGGBlock)
public:
	OfficeArtFDGGBlock();
	XLS::BiffStructurePtr clone();

	
	
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

public:
	unsigned int spidMax;
	unsigned int cspSaved;
	unsigned int cdgSaved;
	std::vector<OfficeArtIDCL> Rgidcl;
};

typedef boost::shared_ptr<OfficeArtFDGGBlock> OfficeArtFDGGBlockPtr;


} // namespace XLS
