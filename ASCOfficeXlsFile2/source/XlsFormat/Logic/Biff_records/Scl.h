#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Scl record in BIFF8
class Scl: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Scl)
	BASE_OBJECT_DEFINE_CLASS_NAME(Scl)
public:
	Scl();
	~Scl();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeScl;

//-----------------------------
	BIFF_SHORT nscl;
	BIFF_SHORT dscl;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(nscl)
		//BO_ATTRIB_MARKUP_ATTRIB(dscl)
		////BO_ATTRIB_MARKUP_ATTRIB(Fraction)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

