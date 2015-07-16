#pragma once

#include "BiffStructure.h"
#include "Boolean.h"

namespace XLS
{;

class CFRecord;

class FtRboData : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FtRboData)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned short idRadNext;
	Boolean<unsigned short> fFirstBtn;
};

} // namespace XLS

