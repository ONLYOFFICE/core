#pragma once

#include "BiffRecord.h"

namespace XLS
{


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

	int serialize(std::wostream & _stream, int sz);

	static const ElementType	type = typeLegend;

//-----------------------------
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

};

} // namespace XLS

