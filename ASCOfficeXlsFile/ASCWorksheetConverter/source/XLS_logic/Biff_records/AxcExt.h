#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of AxcExt record in BIFF8
class AxcExt: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(AxcExt)
	BASE_OBJECT_DEFINE_CLASS_NAME(AxcExt)
public:
	AxcExt();
	~AxcExt();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD catMin;
	BIFF_WORD catMax;
	BIFF_WORD catMajor;
	BIFF_WORD duMajor;
	BIFF_WORD catMinor;
	BIFF_WORD duMinor;
	BIFF_WORD duBase;
	BIFF_WORD catCrossDate;

	BIFF_BOOL fAutoMin;
	BIFF_BOOL fAutoMax;
	BIFF_BOOL fAutoMajor;
	BIFF_BOOL fAutoMinor;
	BIFF_BOOL fDateAxis;
	BIFF_BOOL fAutoBase;
	BIFF_BOOL fAutoCross;
	BIFF_BOOL fAutoDate;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(catMin)
		BO_ATTRIB_MARKUP_ATTRIB(catMax)
		BO_ATTRIB_MARKUP_ATTRIB(catMajor)
		BO_ATTRIB_MARKUP_ATTRIB(duMajor)
		BO_ATTRIB_MARKUP_ATTRIB(catMinor)
		BO_ATTRIB_MARKUP_ATTRIB(duMinor)
		BO_ATTRIB_MARKUP_ATTRIB(duBase)
		BO_ATTRIB_MARKUP_ATTRIB(catCrossDate)
		BO_ATTRIB_MARKUP_ATTRIB(fAutoMin)
		BO_ATTRIB_MARKUP_ATTRIB(fAutoMax)
		BO_ATTRIB_MARKUP_ATTRIB(fAutoMajor)
		BO_ATTRIB_MARKUP_ATTRIB(fAutoMinor)
		BO_ATTRIB_MARKUP_ATTRIB(fDateAxis)
		BO_ATTRIB_MARKUP_ATTRIB(fAutoBase)
		BO_ATTRIB_MARKUP_ATTRIB(fAutoCross)
		BO_ATTRIB_MARKUP_ATTRIB(fAutoDate)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

