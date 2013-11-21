#include "stdafx.h"
#include "MACROSORTANDFILTER.h"
#include <XLS_logic/Biff_records/Sort.h>
#include <XLS_logic/Biff_records/DropDownObjIds.h>
#include <XLS_logic/Biff_unions/SORTDATA12.h>
#include <XLS_logic/Biff_unions/AUTOFILTER.h>

namespace XLS
{;


MACROSORTANDFILTER::MACROSORTANDFILTER()
{
}


MACROSORTANDFILTER::~MACROSORTANDFILTER()
{
}


BaseObjectPtr MACROSORTANDFILTER::clone()
{
	return BaseObjectPtr(new MACROSORTANDFILTER(*this));
}


// MACROSORTANDFILTER = [Sort] [SORTDATA12] [DropDownObjIds] [AUTOFILTER]
const bool MACROSORTANDFILTER::loadContent(BinProcessor& proc)
{
	proc.optional<Sort>();
	proc.optional<SORTDATA12>();
	proc.optional<DropDownObjIds>();
	proc.optional<AUTOFILTER>();
	return true;
}

} // namespace XLS

