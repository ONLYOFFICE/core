#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of ObNoMacros record in BIFF8
class ObNoMacros: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ObNoMacros)
	BASE_OBJECT_DEFINE_CLASS_NAME(ObNoMacros)
public:
	ObNoMacros();
	~ObNoMacros();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeObNoMacros;


};

} // namespace XLS

