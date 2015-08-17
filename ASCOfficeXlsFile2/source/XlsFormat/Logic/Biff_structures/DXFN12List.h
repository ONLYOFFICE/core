#pragma once

#include <Logic/Biff_records/BiffRecord.h>
#include <Logic/Biff_structures/BiffString.h>
#include <Logic/Biff_structures/DXFN.h>
#include <Logic/Biff_structures/XFExtNoFRT.h>

namespace XLS
{

class DXFN12List: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DXFN12List)
public:
	BiffStructurePtr clone();

	DXFN12List();
	~DXFN12List();

	static const ElementType	type = typeDXFN12List;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	DXFN dxfn;
	XFExtNoFRT xfext;
};

typedef boost::shared_ptr<DXFN12List> DXFN12ListPtr;

} // namespace XLS