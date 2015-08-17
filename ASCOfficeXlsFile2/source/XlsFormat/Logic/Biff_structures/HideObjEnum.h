#pragma once

#include "BiffAttribute.h"

namespace XLS
{

class HideObjEnum : public BiffAttributeSimple<unsigned short>
{
public:
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent, const std::wstring & attrib_name);
	//virtual const bool fromXML(BiffStructurePtr & parent, const std::wstring & attrib_name);

	enum
	{
		SHOWALL = 0x0000,
		SHOWPLACEHOLDER = 0x0001,
		HIDEALL = 0x0002
	};
};

} // namespace XLS

