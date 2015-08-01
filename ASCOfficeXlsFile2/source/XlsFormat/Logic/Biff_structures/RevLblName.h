#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class RevLblName : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(RevLblName)
public:
	RevLblName();
	RevLblName(CFRecord& record);
	BiffStructurePtr clone();

	static const ElementType	type = typeRevLblName;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	const std::wstring toString() const;

	std::wstring  st;
};

} // namespace XLS

