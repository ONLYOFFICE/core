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
	_UINT32	x;
	_UINT32	y;
	_UINT32	dx;
	_UINT32	dy;
	unsigned char	wSpace;

	bool fAutoPosition;
	bool fAutoPosX;
	bool fAutoPosY;
	bool fVert;
	bool fWasDataTable;

};

} // namespace XLS

