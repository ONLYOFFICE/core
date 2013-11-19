#pragma once

#include "BiffRecord.h"
#include "Palette.h"

namespace XLS
{;

// Logical representation of ClrtClient record in BIFF8
class ClrtClient: public Palette // Absolutely the same as 'Palette'
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ClrtClient)
	BASE_OBJECT_DEFINE_CLASS_NAME(ClrtClient)
public:
	ClrtClient() : Palette(3) {};
};

} // namespace XLS

