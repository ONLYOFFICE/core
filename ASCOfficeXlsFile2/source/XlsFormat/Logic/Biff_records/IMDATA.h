#pragma once

#include "BiffRecordContinued.h"

namespace XLS
{
// Logical representation of IMDATA record in BIFF5
class IMDATA: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(IMDATA)
	BASE_OBJECT_DEFINE_CLASS_NAME(IMDATA)
public:
	IMDATA();
	~IMDATA();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeIMDATA;

	_UINT16 cf;
	_UINT16 env;
	_UINT32 lcb;

	boost::shared_array<char> pData;
};
}