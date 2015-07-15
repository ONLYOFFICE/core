#pragma once

#include "BiffRecord.h"

namespace XLS
{;


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

//-----------------------------
	BIFF_BOOL fFillSurface;
	BIFF_BOOL f3DPhongShade;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(fFillSurface)
		//BO_ATTRIB_MARKUP_ATTRIB(f3DPhongShade)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

