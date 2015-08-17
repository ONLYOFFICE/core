#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of SIIndex record in BIFF8
class SIIndex: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SIIndex)
	BASE_OBJECT_DEFINE_CLASS_NAME(SIIndex)
public:
	SIIndex();
	~SIIndex();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
	
	static const ElementType	type = typeSIIndex;

//-----------------------------
	BIFF_WORD numIndex;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(numIndex)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

