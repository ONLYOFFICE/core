#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of IFmtRecord record in BIFF8
class IFmtRecord: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(IFmtRecord)
	BASE_OBJECT_DEFINE_CLASS_NAME(IFmtRecord)
public:
	IFmtRecord();
	~IFmtRecord();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeIFmtRecord;

//-----------------------------
	BIFF_WORD ifmt;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(ifmt)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

