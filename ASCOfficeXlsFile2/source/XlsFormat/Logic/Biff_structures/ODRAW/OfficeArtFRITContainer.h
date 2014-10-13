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
	BASE_OBJECT_DEFINE_CLASS_NAME(OfficeArtFRITContainer)
public:
	OfficeArtFRITContainer();
	XLS::BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

	// overriden
	const unsigned __int16 GetInstanceToStore();

public:
	std::vector<OfficeArtFRIT> rgfrit;
};

typedef boost::shared_ptr<OfficeArtFRITContainer> OfficeArtFRITContainerPtr;


} // namespace XLS
