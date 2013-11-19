#pragma once

#include "BiffStructure.h"
#include <XLS_logic/Biff_structures/BitMarkedStructs.h>
#include <XLS_logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class NoteSh : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(NoteSh)
public:
	NoteSh();
	NoteSh(CFRecord& record);
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	Rw row;
	Col col;
	bool fShow;
	bool fRwHidden;
	bool fColHidden;
	ObjId idObj;
	XLUnicodeString stAuthor;

};

} // namespace XLS

