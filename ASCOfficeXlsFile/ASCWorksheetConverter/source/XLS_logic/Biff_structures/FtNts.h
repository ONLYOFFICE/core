#pragma once

#include "BiffStructTagged.h"
#include <XLS_logic/Biff_structures/Boolean.h>

namespace XLS
{;

class CFRecord;

class FtNts : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FtNts)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	_bstr_t guid;
	Boolean<WORD> fSharedNote;
};

} // namespace XLS

