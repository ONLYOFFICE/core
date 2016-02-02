#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Surf record in BIFF8
class Surf: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Surf)
	BASE_OBJECT_DEFINE_CLASS_NAME(Surf)
public:
	Surf();
	~Surf();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSurf;

	int serialize(std::wostream & _stream);

//-----------------------------
	bool fFillSurface;
	bool f3DPhongShade;
};

} // namespace XLS

