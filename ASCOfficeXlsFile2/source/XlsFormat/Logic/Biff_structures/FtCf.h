#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class FtCf : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FtCf)
public:
	FtCf() 
	{
		cf = 0xFFFF;
	}

	BiffStructurePtr clone();
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned __int16 cf;
};

} // namespace XLS

