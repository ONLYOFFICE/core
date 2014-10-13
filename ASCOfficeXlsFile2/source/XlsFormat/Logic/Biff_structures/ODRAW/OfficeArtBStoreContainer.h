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
	BASE_OBJECT_DEFINE_CLASS_NAME(OfficeArtBStoreContainer)
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

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

	// overriden
	const unsigned __int16 GetInstanceToStore();

public:	
	std::vector<OfficeArtBStoreContainerFileBlock*> rgfb;
};

typedef boost::shared_ptr<OfficeArtBStoreContainer> OfficeArtBStoreContainerPtr;


} // namespace XLS
