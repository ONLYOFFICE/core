#pragma once

#include "BiffRecord.h"

namespace XLS
{;


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
private:
	BIFF_BYTE sertm;
	BIFF_BYTE ebsrc;
	Boolean<unsigned char> fTeeTop;
	BIFF_DOUBLE numValue;
	BIFF_WORD cnum;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(sertm)
		BO_ATTRIB_MARKUP_ATTRIB(ebsrc)
		BO_ATTRIB_MARKUP_ATTRIB(fTeeTop)
		if(0x04 != ebsrc && 0x05 != ebsrc)
		{
			BO_ATTRIB_MARKUP_ATTRIB(numValue)
		}
		BO_ATTRIB_MARKUP_ATTRIB(cnum)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

