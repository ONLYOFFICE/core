
#include "LBL.h"
#include <Logic/Biff_records/Lbl.h>
#include <Logic/Biff_records/NameCmt.h>
#include <Logic/Biff_records/NameFnGrp12.h>
#include <Logic/Biff_records/NamePublish.h>

namespace XLS
{;


LBL::LBL()
{
}


LBL::~LBL()
{
}


BaseObjectPtr LBL::clone()
{
	return BaseObjectPtr(new LBL(*this));
}


// LBL = Lbl [NameCmt] [NameFnGrp12] [NamePublish]
const bool LBL::loadContent(BinProcessor& proc)
{
	Lbl lbl;
	if(!proc.mandatory(lbl))
	{
		return false;
	}
	XLUnicodeStringNoCch name = lbl.getName();
	proc.optional(NameCmt(name));
	proc.optional<NameFnGrp12>();
	proc.optional<NamePublish>();

	return true;
}

} // namespace XLS

