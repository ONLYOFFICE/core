#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of ShapePropsStream record in BIFF8
class ShapePropsStream: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ShapePropsStream)
	BASE_OBJECT_DEFINE_CLASS_NAME(ShapePropsStream)
public:
	ShapePropsStream();
	~ShapePropsStream();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
	
	static const ElementType	type = typeShapePropsStream;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

