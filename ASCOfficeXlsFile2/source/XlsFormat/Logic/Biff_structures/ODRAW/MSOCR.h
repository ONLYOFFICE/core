#pragma once

#include "../BiffStructure.h"

namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{;

class MSOCR : public XLS::BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(MSOCR)
public:
	XLS::BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

public:
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	bool fSchemeIndex;
};

typedef boost::shared_ptr<MSOCR> MSOCRPtr;


} // namespace XLS
