#include "precompiled_xls.h"
#include "SORTANDFILTER.h"
#include <Logic/Biff_records/Sort.h>
#include <Logic/Biff_unions/SORTDATA12.h>
#include <Logic/Biff_records/FilterMode.h>
#include <Logic/Biff_records/DropDownObjIds.h>
#include <Logic/Biff_unions/AUTOFILTER.h>

namespace XLS
{;


SORTANDFILTER::SORTANDFILTER()
{
}


SORTANDFILTER::~SORTANDFILTER()
{
}


BaseObjectPtr SORTANDFILTER::clone()
{
	return BaseObjectPtr(new SORTANDFILTER(*this));
}


// SORTANDFILTER = [Sort] [SORTDATA12] [FilterMode] [DropDownObjIds] [AUTOFILTER]
const bool SORTANDFILTER::loadContent(BinProcessor& proc)
{
	bool res1 = proc.optional<Sort>();
	bool res2 = proc.optional<SORTDATA12>();
	bool res3 = proc.optional<FilterMode>();
	bool res4 = proc.optional<DropDownObjIds>();
	bool res5 = proc.optional<AUTOFILTER>();

	return res1 || res2 || res3 || res4 || res5;
}

} // namespace XLS

