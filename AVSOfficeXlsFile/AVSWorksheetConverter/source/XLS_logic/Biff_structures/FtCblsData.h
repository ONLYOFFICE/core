#pragma once

#include "BiffStructTagged.h"

namespace XLS
{;

class CFRecord;

class FtCblsData : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FtCblsData)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	WORD fChecked;
	WORD accel;
	bool fNo3d;
};

} // namespace XLS

