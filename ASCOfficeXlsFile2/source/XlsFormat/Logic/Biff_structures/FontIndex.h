#pragma once

#include "BiffAttribute.h"
//#include <Logic/Biff_structures/Phs.h>
//#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class FontIndex : public BiffAttributeSimple<unsigned short>
{
public:
	BiffStructurePtr clone();

	operator const _variant_t () const;
	const unsigned short	getValue() const;
	void					setValue(unsigned short val);
};

} // namespace XLS

