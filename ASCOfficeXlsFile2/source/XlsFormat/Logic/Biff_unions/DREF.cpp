#include "precompiled_xls.h"
#include "DREF.h"
#include <Logic/Biff_records/DConName.h>
#include <Logic/Biff_records/DConBin.h>
#include <Logic/Biff_records/DConRef.h>

namespace XLS
{;


DREF::DREF()
{
}


DREF::~DREF()
{
}


BaseObjectPtr DREF::clone()
{
	return BaseObjectPtr(new DREF(*this));
}


// DREF = DConName / DConBin / DConRef
const bool DREF::loadContent(BinProcessor& proc)
{
	if(!proc.optional<DConName>())
	{
		if(!proc.optional<DConBin>())
		{
			if(!proc.optional<DConRef>())
			{
				return false;
			}
		}
	}
	return true;
}

} // namespace XLS

