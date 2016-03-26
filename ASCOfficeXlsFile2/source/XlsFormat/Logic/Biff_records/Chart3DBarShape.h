#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Chart3DBarShape record in BIFF8
class Chart3DBarShape: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Chart3DBarShape)
	BASE_OBJECT_DEFINE_CLASS_NAME(Chart3DBarShape)
public:
	Chart3DBarShape();
	~Chart3DBarShape();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	int serialize(std::wostream & _stream);

	static const ElementType	type = typeChart3DBarShape;

//-----------------------------
	unsigned char	riser;
	unsigned char	taper;

};

} // namespace XLS

