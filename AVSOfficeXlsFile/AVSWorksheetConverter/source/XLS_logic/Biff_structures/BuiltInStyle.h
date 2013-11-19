#pragma once

#include "BiffStructTagged.h"
namespace XLS
{;

class CFRecord;

//#pragma pack(1)
class BuiltInStyle : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(BuiltInStyle)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	BYTE istyBuiltIn;
	BYTE iLevel;
};
//#pragma pack() // restore

} // namespace XLS
