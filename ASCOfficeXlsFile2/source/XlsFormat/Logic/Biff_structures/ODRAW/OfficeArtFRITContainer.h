#pragma once

#include "OfficeArtRecord.h"
#include "OfficeArtFRIT.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{;

class OfficeArtFRITContainer : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtFRITContainer)
public:
	OfficeArtFRITContainer();
	XLS::BiffStructurePtr clone();

	
	
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

	// overriden
	const unsigned short GetInstanceToStore();

public:
	std::vector<OfficeArtFRIT> rgfrit;
};

typedef boost::shared_ptr<OfficeArtFRITContainer> OfficeArtFRITContainerPtr;


} // namespace XLS
