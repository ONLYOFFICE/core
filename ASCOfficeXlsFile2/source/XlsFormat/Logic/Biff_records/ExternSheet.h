#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/XTI.h>

namespace XLS
{;


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
	ForwardOnlyParam<unsigned __int16> cXTI;
	BiffStructurePtrVector rgXTI;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(cXTI)
		//BO_ATTRIB_MARKUP_VECTOR_COMPLEX(rgXTI, XTI)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

