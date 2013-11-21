#pragma once

#include "BiffStructTagged.h"

namespace XLS
{;

class CFRecord;

#pragma pack(1)
class Phs : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Phs)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	WORD ifnt;
	struct Phs_Data
	{
		WORD phType : 2;
		WORD alcH : 2;
		WORD set_one : 2;
		WORD unused : 10;
	} data;
};
#pragma pack()

} // namespace XLS
