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
	BASE_OBJECT_DEFINE_CLASS_NAME(OfficeArtFDGGBlock)
public:
	OfficeArtFDGGBlock();
	XLS::BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

public:
	DWORD spidMax;
	DWORD cspSaved;
	DWORD cdgSaved;
	std::vector<OfficeArtIDCL> Rgidcl;
};

typedef boost::shared_ptr<OfficeArtFDGGBlock> OfficeArtFDGGBlockPtr;


} // namespace XLS
