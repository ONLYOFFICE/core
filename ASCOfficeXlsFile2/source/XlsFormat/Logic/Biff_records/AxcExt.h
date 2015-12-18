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

	int serialize(std::wostream & _stream);

//-----------------------------
	BIFF_WORD catMin;
	BIFF_WORD catMax;
	BIFF_WORD catMajor;
	BIFF_WORD duMajor;
	BIFF_WORD catMinor;
	BIFF_WORD duMinor;
	BIFF_WORD duBase;
	BIFF_WORD catCrossDate;

	bool fAutoMin;
	bool fAutoMax;
	bool fAutoMajor;
	bool fAutoMinor;
	bool fDateAxis;
	bool fAutoBase;
	bool fAutoCross;
	bool fAutoDate;

};

} // namespace XLS

