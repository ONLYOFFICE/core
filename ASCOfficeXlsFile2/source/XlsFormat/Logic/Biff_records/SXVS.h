#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of SXVS record in BIFF8
class SXVS: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SXVS)
	BASE_OBJECT_DEFINE_CLASS_NAME(SXVS)
public:
	SXVS();
	~SXVS();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
	
	static const ElementType	type = typeSXVS;

//-----------------------------
//	BIFF_WORD userName;

};

} // namespace XLS

