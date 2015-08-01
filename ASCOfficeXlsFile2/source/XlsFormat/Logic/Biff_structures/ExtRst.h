#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/Phs.h>
#include <Logic/Biff_structures/RPHSSub.h>
#include <Logic/Biff_structures/PhRuns.h>

namespace XLS
{;

class CFRecord;

class ExtRst : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ExtRst)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeExtRst;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	const size_t getSize() const;

	unsigned short cb;
	Phs phs;
	RPHSSub rphssub;
	std::vector<PhRuns> rgphruns;
};

} // namespace XLS
