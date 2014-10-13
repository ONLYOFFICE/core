#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class SheetId : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SheetId)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned __int16 id;
};

typedef boost::shared_ptr<SheetId> SheetIdPtr;

} // namespace XLS

