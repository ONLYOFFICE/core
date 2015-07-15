#pragma once

#include "BiffStructure.h"
#include "Boolean.h"

namespace XLS
{;

class CFRecord;

class FtGboData : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FtGboData)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned __int16 accel;
	bool fNo3d;
};

} // namespace XLS

