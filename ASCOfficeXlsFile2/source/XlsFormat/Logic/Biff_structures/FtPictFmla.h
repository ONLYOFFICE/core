#pragma once

#include "BiffStructure.h"
#include "ObjFmla.h"
#include "PictFmlaKey.h"
#include "FtPioGrbit.h"

namespace XLS
{

class CFRecord;

class FtPictFmla : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FtPictFmla)
public:
	FtPictFmla();
	BiffStructurePtr clone();

	virtual void load(CFRecord& record, FtPioGrbit& pictFlags);
	virtual void store(CFRecord& record, FtPioGrbit& pictFlags);

	static const ElementType	type = typeFtPictFmla;

	virtual void load(CFRecord& record){}
	virtual void store(CFRecord& record){}

	ObjFmla fmla;
	unsigned int lPosInCtlStm;
	unsigned int cbBufInCtlStm;
	PictFmlaKey key;
};

} // namespace XLS

