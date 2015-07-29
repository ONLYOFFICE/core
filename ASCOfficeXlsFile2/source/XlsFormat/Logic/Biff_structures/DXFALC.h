#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class DXFALC : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DXFALC)
public:
	BiffStructurePtr clone();

	
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned char alc;
	bool fWrap;
	unsigned char alcv;
	bool fJustLast;
	unsigned char trot;
	unsigned char cIndent;
	bool fShrinkToFit;
	bool fMergeCell;
	unsigned char iReadingOrder;

	unsigned int iIndent;
};

} // namespace XLS

