#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/LongRGB.h>

namespace XLS
{


// Logical representation of Palette record in BIFF8
class Palette: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Palette)
	BASE_OBJECT_DEFINE_CLASS_NAME(Palette)
public:
	Palette();
	Palette(const unsigned short ccv_init);

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	int serialize(std::wostream & stream);

	static const ElementType type = typePalette;

//-----------------------------
	BiffStructurePtrVector rgColor;
	unsigned short ccv;

};

} // namespace XLS

