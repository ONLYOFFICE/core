#pragma once

#include <Logic/Biff_structures/FrtHeader.h>
#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of ShapePropsStream record in BIFF8
class ShapePropsStream: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ShapePropsStream)
	BASE_OBJECT_DEFINE_CLASS_NAME(ShapePropsStream)
public:
	ShapePropsStream();
	~ShapePropsStream();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
	
	static const ElementType	type = typeShapePropsStream;

	FrtHeader		frtHeader;
	unsigned short  wObjContext;
	unsigned int	dwChecksum;

	std::string		xml_;


};

} // namespace XLS

