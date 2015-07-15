#pragma once

#include "BiffRecord.h"

namespace XLS
{;


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

	static const ElementType	type = typePos;

//-----------------------------
	BIFF_WORD mdTopLt;
	BIFF_WORD mdBotRt;
	BIFF_SHORT x1;
	BIFF_SHORT y1;
	BIFF_SHORT x2;
	BIFF_SHORT y2;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(mdTopLt)
		//BO_ATTRIB_MARKUP_ATTRIB(mdBotRt)
		//BO_ATTRIB_MARKUP_ATTRIB(x1)
		//BO_ATTRIB_MARKUP_ATTRIB(y1)
		//BO_ATTRIB_MARKUP_ATTRIB(x2)
		//BO_ATTRIB_MARKUP_ATTRIB(y2)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

