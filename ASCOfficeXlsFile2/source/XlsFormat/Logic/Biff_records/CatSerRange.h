#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of CatSerRange record in BIFF8
class CatSerRange: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CatSerRange)
	BASE_OBJECT_DEFINE_CLASS_NAME(CatSerRange)
public:
	CatSerRange();
	~CatSerRange();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_SHORT catCross;
	BIFF_SHORT catLabel;
	BIFF_SHORT catMark;
	BIFF_BOOL fBetween;
	BIFF_BOOL fMaxCross;
	BIFF_BOOL fReverse;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(catCross)
		BO_ATTRIB_MARKUP_ATTRIB(catLabel)
		BO_ATTRIB_MARKUP_ATTRIB(catMark)
		BO_ATTRIB_MARKUP_ATTRIB(fBetween)
		BO_ATTRIB_MARKUP_ATTRIB(fMaxCross)
		BO_ATTRIB_MARKUP_ATTRIB(fReverse)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

