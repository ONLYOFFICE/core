#pragma once

#include "BiffStructTagged.h"
#include <XLS_logic/Biff_structures/Boolean.h>

namespace XLS
{;

class CFRecord;

class FtSbs : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FtSbs)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

public:
	WORD ft;
	WORD cb;

private:
	short iVal;
	short iMin;
	short iMax;
	short dInc;
	short dPage;
	Boolean<WORD> fHoriz;
	short dxScroll;

	bool fDraw;
	bool fDrawSliderOnly;
	bool fTrackElevator;
	bool fNo3d;
};

} // namespace XLS

