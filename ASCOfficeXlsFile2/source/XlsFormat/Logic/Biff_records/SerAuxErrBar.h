#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of SerAuxErrBar record in BIFF8
class SerAuxErrBar: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SerAuxErrBar)
	BASE_OBJECT_DEFINE_CLASS_NAME(SerAuxErrBar)
public:
	SerAuxErrBar();
	~SerAuxErrBar();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSerAuxErrBar;

//-----------------------------
	BIFF_BYTE sertm;
	BIFF_BYTE ebsrc;
	Boolean<unsigned char> fTeeTop;
	BIFF_DOUBLE numValue;
	BIFF_WORD cnum;

};

} // namespace XLS

