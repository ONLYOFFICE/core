#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of SXStreamID record in BIFF8
class SXStreamID: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SXStreamID)
	BASE_OBJECT_DEFINE_CLASS_NAME(SXStreamID)
public:
	SXStreamID();
	~SXStreamID();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSXStreamID;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

