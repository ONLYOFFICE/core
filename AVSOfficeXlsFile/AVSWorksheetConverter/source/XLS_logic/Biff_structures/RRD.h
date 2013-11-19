#pragma once

#include "BiffStructure.h"
#include <XLS_logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class RRD : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(RRD)
public:
	RRD();
	RRD(CFRecord& record);
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	DWORD cbMemory;
	long revid;
	RevisionType revt;
	bool fAccepted;
	bool fUndoAction;
	bool fDelAtEdgeOfSort;
	TabId tabid;

};

} // namespace XLS

