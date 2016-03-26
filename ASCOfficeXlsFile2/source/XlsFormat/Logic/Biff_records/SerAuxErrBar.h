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
	unsigned char	sertm;
	unsigned char	ebsrc;
	unsigned char	fTeeTop;
	BIFF_DOUBLE		numValue;
	_UINT16			cnum;

};

} // namespace XLS

