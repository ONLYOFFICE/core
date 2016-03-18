#pragma once

#include "BiffAttribute.h"

namespace XLS
{

class HideObjEnum : public BiffAttributeSimple<unsigned short>
{
public:
	BiffStructurePtr clone();

	enum
	{
		SHOWALL = 0x0000,
		SHOWPLACEHOLDER = 0x0001,
		HIDEALL = 0x0002
	};
};

} // namespace XLS

