#pragma once

#include "BiffRecord.h"

namespace XLS
{


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

	static const ElementType	type = typeDVal;

//-----------------------------
	bool fWnClosed;
	BIFF_DWORD xLeft;
	BIFF_DWORD yTop;
	BIFF_LONG idObj;
	BIFF_DWORD idvMac;

};

} // namespace XLS

