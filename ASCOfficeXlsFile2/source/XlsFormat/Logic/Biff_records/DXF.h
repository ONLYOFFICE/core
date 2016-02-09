#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/FrtRefHeaderU.h>
#include <Logic/Biff_structures/XFProps.h>

namespace XLS
{


// Logical representation of DXF record in BIFF8
class DXF: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DXF)
	BASE_OBJECT_DEFINE_CLASS_NAME(DXF)
public:
	DXF();
	~DXF();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeDXF;

	int serialize(std::wostream & stream);
	
	FrtRefHeaderU	frtRefHeaderU;
	XFProps			xfprops;
	
};

} // namespace XLS

