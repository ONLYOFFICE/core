#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of ObProj record in BIFF8
class ObProj: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ObProj)
	BASE_OBJECT_DEFINE_CLASS_NAME(ObProj)
public:
	ObProj();
	~ObProj();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeObProj;


};

} // namespace XLS

