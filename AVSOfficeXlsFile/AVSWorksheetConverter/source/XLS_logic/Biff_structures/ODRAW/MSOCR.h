#pragma once

#include "../BiffStructTagged.h"

namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{;

class MSOCR : public XLS::BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(MSOCR)
public:
	XLS::BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

public:
	byte red;
	byte green;
	byte blue;
	bool fSchemeIndex;
};

typedef boost::shared_ptr<MSOCR> MSOCRPtr;


} // namespace XLS
