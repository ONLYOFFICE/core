#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class FtCmo : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FtCmo)
public:
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	static const ElementType	type = typeFtCmo;

	unsigned short ot;
	unsigned short id;

	bool fLocked;
	bool fDefaultSize;
	bool fPublished;
	bool fPrint;
	bool fDisabled;
	bool fUIObj;
	bool fRecalcObj;
	bool fRecalcObjAlways;

};

} // namespace XLS

