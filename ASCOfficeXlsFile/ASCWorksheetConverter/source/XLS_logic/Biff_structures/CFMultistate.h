#pragma once

#include "BiffStructTagged.h"
#include "CFMStateItem.h"

namespace XLS
{;

class CFRecord;

class CFMultistate : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CFMultistate)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	BYTE cStates;
	BYTE iIconSet;

	bool fIconOnly;
	bool fReverse;

	std::vector<CFMStateItemPtr> rgStates;
};

typedef boost::shared_ptr<CFMultistate> CFMultistatePtr;

} // namespace XLS

