#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/Boolean.h>

namespace XLS
{;

class CFRecord;

class FtSbs : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FtSbs)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

public:
	unsigned __int16 ft;
	unsigned __int16 cb;

private:
	short iVal;
	short iMin;
	short iMax;
	short dInc;
	short dPage;
	Boolean<unsigned __int16> fHoriz;
	short dxScroll;

	bool fDraw;
	bool fDrawSliderOnly;
	bool fTrackElevator;
	bool fNo3d;
};

} // namespace XLS

