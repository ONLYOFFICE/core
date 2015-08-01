#pragma once

#include "BiffStructure.h"
#include "ObjFmla.h"
#include "LbsDropData.h"
#include "BiffString.h"
#include "Boolean.h"

namespace XLS
{;

class CFRecord;

class FtLbsData : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FtLbsData)
public:
	FtLbsData();
	BiffStructurePtr clone();

	static const ElementType	type = typeFtLbsData;

	virtual void load(CFRecord& record, const unsigned short ot);
	virtual void store(CFRecord& record, const unsigned short ot);

	virtual void load(CFRecord& record){}
	virtual void store(CFRecord& record){}

	ObjFmla fmla;
	unsigned short cLines;
	unsigned short iSel;

	bool fUseCB;
	bool fValidPlex;
	bool fValidIds;
	bool fNo3d;
	unsigned char wListSelType;
	unsigned char lct;

	unsigned short idEdit;

	LbsDropDataPtr dropData;
	std::vector<XLUnicodeString> rgLines;
	std::vector<Boolean<unsigned char>> bsels;

};

} // namespace XLS

