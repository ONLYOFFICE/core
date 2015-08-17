#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/Boolean.h>

namespace XLS
{

class CFRecord;

class FtNts : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FtNts)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeFtNts;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	std::wstring  guid;
	Boolean<unsigned short> fSharedNote;
};

} // namespace XLS

