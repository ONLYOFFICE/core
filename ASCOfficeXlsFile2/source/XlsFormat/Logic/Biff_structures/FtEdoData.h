#pragma once

#include "BiffStructure.h"
#include "Boolean.h"

namespace XLS
{;

class CFRecord;

class FtEdoData : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FtEdoData)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned __int16 ivtEdit;
	Boolean<unsigned __int16> fMultiLine;
	unsigned __int16 fVScroll;
	unsigned __int16 id;
};

} // namespace XLS

