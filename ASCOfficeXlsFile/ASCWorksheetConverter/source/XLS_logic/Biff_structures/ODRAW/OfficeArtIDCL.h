#pragma once

#include "../BiffStructTagged.h"

namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{;

class OfficeArtIDCL : public XLS::BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(OfficeArtIDCL)
public:
	XLS::BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

public:
	DWORD dgid;
	DWORD cspidCur;
};

typedef boost::shared_ptr<OfficeArtIDCL> OfficeArtIDCLPtr;


} // namespace XLS
