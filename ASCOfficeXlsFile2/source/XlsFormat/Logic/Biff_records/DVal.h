#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of DVal record in BIFF8
class DVal: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DVal)
	BASE_OBJECT_DEFINE_CLASS_NAME(DVal)
public:
	DVal();
	~DVal();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_BOOL fWnClosed;
	BIFF_DWORD xLeft;
	BIFF_DWORD yTop;
	BIFF_LONG idObj;
	BIFF_DWORD idvMac;

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fWnClosed)
		BO_ATTRIB_MARKUP_ATTRIB(xLeft)
		BO_ATTRIB_MARKUP_ATTRIB(yTop)
		BO_ATTRIB_MARKUP_ATTRIB(idObj)
		BO_ATTRIB_MARKUP_ATTRIB(idvMac)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

