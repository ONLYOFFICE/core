#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/Boolean.h>

namespace XLS
{

class CFRecord;

class FtSbs : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FtSbs)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeFtSbs;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned short ft;
	unsigned short cb;

	short iVal;
	short iMin;
	short iMax;
	short dInc;
	short dPage;
	Boolean<unsigned short> fHoriz;
	short dxScroll;

	bool fDraw;
	bool fDrawSliderOnly;
	bool fTrackElevator;
	bool fNo3d;
};

} // namespace XLS

