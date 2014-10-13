#pragma once

#include "BiffStructure.h"
#include "Boolean.h"
#include "CFVO.h"

namespace XLS
{;

class CFRecord;

class CFMStateItem : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CFMStateItem)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	CFVO cfvo;
	Boolean<unsigned char> fEqual;
};

typedef boost::shared_ptr<CFMStateItem> CFMStateItemPtr;

} // namespace XLS

