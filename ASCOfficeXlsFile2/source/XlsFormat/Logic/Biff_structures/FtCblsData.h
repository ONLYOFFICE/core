#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class FtCblsData : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FtCblsData)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned __int16 fChecked;
	unsigned __int16 accel;
	bool fNo3d;
};

} // namespace XLS

