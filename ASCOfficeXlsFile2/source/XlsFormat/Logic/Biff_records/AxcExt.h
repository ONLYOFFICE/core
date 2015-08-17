#pragma once

#include "BiffRecord.h"

namespace XLS
{


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

	static const ElementType	type = typeAxcExt;

//-----------------------------
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

};

} // namespace XLS

