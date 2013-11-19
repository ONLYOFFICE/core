#pragma once

#include "BiffStructTagged.h"
#include "ObjFmla.h"

namespace XLS
{;

class CFRecord;

class FtMacro : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FtMacro)
public:
	FtMacro();
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	ObjFmla fmla;
};

} // namespace XLS

