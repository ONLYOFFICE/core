#pragma once

#include "BiffStructTagged.h"
#include "Boolean.h"

namespace XLS
{;

class CFRecord;

class FtGboData : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FtGboData)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	WORD accel;
	bool fNo3d;
};

} // namespace XLS

