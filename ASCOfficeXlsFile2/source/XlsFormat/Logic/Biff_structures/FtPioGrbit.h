#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class FtPioGrbit : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FtPioGrbit)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeFtPioGrbit;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	bool fAutoPict;
	bool fDde;
	bool fPrintCalc;
	bool fIcon;
	bool fCtl;
	bool fPrstm;
	bool fCamera;
	bool fDefaultSize;
	bool fAutoLoad;
};

} // namespace XLS

