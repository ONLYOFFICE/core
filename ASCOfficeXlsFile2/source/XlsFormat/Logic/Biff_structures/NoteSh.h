#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{

class CFRecord;

class NoteSh : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(NoteSh)
public:
	NoteSh();
	NoteSh(CFRecord& record);
	BiffStructurePtr clone();

	static const ElementType	type = typeNoteSh;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	Rw row;
	Col col;
	bool fShow;
	bool fRwHidden;
	bool fColHidden;
	ObjId idObj;
	XLUnicodeString stAuthor;

};

} // namespace XLS

