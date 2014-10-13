#pragma once

#include "BiffStructure.h"
#include "CFMStateItem.h"

namespace XLS
{;

class CFRecord;

class CFMultistate : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CFMultistate)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned char cStates;
	unsigned char iIconSet;

	bool fIconOnly;
	bool fReverse;

	std::vector<CFMStateItemPtr> rgStates;
};

typedef boost::shared_ptr<CFMultistate> CFMultistatePtr;

} // namespace XLS

