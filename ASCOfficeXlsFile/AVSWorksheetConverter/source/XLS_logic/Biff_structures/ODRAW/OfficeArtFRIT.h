#pragma once

#include "../BiffStructTagged.h"

namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{;

class OfficeArtFRIT : public XLS::BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(OfficeArtFRIT)
public:
	XLS::BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

public:
	WORD fridNew;
	WORD fridOld;
};

typedef boost::shared_ptr<OfficeArtFRIT> OfficeArtFRITPtr;


} // namespace XLS
