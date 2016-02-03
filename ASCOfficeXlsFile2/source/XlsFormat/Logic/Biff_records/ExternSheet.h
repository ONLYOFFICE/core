#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/XTI.h>

namespace XLS
{


// Logical representation of ExternSheet record in BIFF8
class ExternSheet: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ExternSheet)
	BASE_OBJECT_DEFINE_CLASS_NAME(ExternSheet)
public:
	ExternSheet();
	~ExternSheet();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeExternSheet;

//-----------------------------
	ForwardOnlyParam<_UINT16> cXTI;
	BiffStructurePtrVector rgXTI;
};

} // namespace XLS

