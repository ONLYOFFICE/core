#pragma once

#include "BiffStructTagged.h"
#include "Boolean.h"

namespace XLS
{;

class CFRecord;

class FtEdoData : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FtEdoData)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	WORD ivtEdit;
	Boolean<WORD> fMultiLine;
	WORD fVScroll;
	WORD id;
};

} // namespace XLS

