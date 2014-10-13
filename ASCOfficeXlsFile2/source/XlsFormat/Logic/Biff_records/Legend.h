#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Legend record in BIFF8
class Legend: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Legend)
	BASE_OBJECT_DEFINE_CLASS_NAME(Legend)
public:
	Legend();
	~Legend();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_DWORD x;
	BIFF_DWORD y;
	BIFF_DWORD dx;
	BIFF_DWORD dy;
	BIFF_BYTE wSpace;

	BIFF_BOOL fAutoPosition;
	BIFF_BOOL fAutoPosX;
	BIFF_BOOL fAutoPosY;
	BIFF_BOOL fVert;
	BIFF_BOOL fWasDataTable;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(x)
		BO_ATTRIB_MARKUP_ATTRIB(y)
		BO_ATTRIB_MARKUP_ATTRIB(dx)
		BO_ATTRIB_MARKUP_ATTRIB(dy)
		BO_ATTRIB_MARKUP_ATTRIB(wSpace)

		BO_ATTRIB_MARKUP_ATTRIB(fAutoPosition)
		BO_ATTRIB_MARKUP_ATTRIB(fAutoPosX)
		BO_ATTRIB_MARKUP_ATTRIB(fAutoPosY)
		BO_ATTRIB_MARKUP_ATTRIB(fVert)
		BO_ATTRIB_MARKUP_ATTRIB(fWasDataTable)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

