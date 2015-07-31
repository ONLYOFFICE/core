#pragma once

#include "OfficeArtRecord.h"
#include "OfficeArtBStoreContainerFileBlock.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{;

class OfficeArtBStoreContainer : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtBStoreContainer)
public:
	OfficeArtBStoreContainer();
	XLS::BiffStructurePtr clone();
	
	~OfficeArtBStoreContainer()
	{
		for (size_t i = 0; i < rgfb.size(); i++)
		{
			delete rgfb[i];
		}
	}
	
	static const XLS::ElementType	type = XLS::typeOfficeArtBStoreContainer;

	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

	// overriden
	const unsigned short GetInstanceToStore();

	std::vector<OfficeArtBStoreContainerFileBlock*> rgfb;
};

typedef boost::shared_ptr<OfficeArtBStoreContainer> OfficeArtBStoreContainerPtr;


} // namespace XLS
