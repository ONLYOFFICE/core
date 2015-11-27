#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Pos record in BIFF8
class Pos: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Pos)
	BASE_OBJECT_DEFINE_CLASS_NAME(Pos)
public:
	Pos();
	~Pos();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	int serialize(std::wostream & stream_);

	static const ElementType	type = typePos;

//-----------------------------
	BIFF_WORD mdTopLt;
	BIFF_WORD mdBotRt;
	BIFF_SHORT x1;
	BIFF_SHORT y1;
	BIFF_SHORT x2;
	BIFF_SHORT y2;

};

} // namespace XLS

