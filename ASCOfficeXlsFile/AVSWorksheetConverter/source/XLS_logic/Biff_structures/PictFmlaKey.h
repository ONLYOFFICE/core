#pragma once

#include "BiffStructTagged.h"
#include "ObjFmla.h"
#include <XLS_logic/Biff_structures/FtPioGrbit.h>

namespace XLS
{;

class CFRecord;

class PictFmlaKey : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PictFmlaKey)
public:
	PictFmlaKey();
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	ObjFmla fmlaLinkedCell;
	ObjFmla fmlaListFillRange;
};

} // namespace XLS

