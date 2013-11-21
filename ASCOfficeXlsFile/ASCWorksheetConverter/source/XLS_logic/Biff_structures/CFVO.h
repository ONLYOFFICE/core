#pragma once

#include "BiffStructTagged.h"
#include "CFVOParsedFormula.h"

namespace XLS
{;

class CFRecord;

class CFVO : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CFVO)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	BYTE cfvoType;
	CFVOParsedFormula fmla;
	double numValue;
};

typedef boost::shared_ptr<CFVO> CFVOPtr;

} // namespace XLS

