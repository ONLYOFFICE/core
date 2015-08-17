#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/SerAr.h>
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{

class CFRecord;

class MOper : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(MOper)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeMOper;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	ColunByteU colLast;
	RwU rowLast;
	std::vector<SerArPtr> extOper;
};

} // namespace XLS

