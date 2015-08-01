#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class RRD : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(RRD)
public:
	RRD();
	RRD(CFRecord& record);
	BiffStructurePtr clone();

	static const ElementType	type = typeRRD;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned int cbMemory;
	long revid;
	RevisionType revt;
	bool fAccepted;
	bool fUndoAction;
	bool fDelAtEdgeOfSort;
	TabId tabid;

};

} // namespace XLS

