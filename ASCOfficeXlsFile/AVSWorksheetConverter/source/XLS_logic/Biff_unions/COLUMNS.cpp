#include "stdafx.h"
#include "COLUMNS.h"
#include <XLS_logic/Biff_records/DefColWidth.h>
#include <XLS_logic/Biff_records/ColInfo.h>

namespace XLS
{;


COLUMNS::COLUMNS()
{
}


COLUMNS::~COLUMNS()
{
}


BaseObjectPtr COLUMNS::clone()
{
	return BaseObjectPtr(new COLUMNS(*this));
}


// COLUMNS = DefColWidth *255ColInfo
const bool COLUMNS::loadContent(BinProcessor& proc)
{
// 	if(!proc.mandatory<DefColWidth>())
// 	{
// 		return false;
// 	}
	bool def_ok = proc.optional<DefColWidth>(); // OpenOffice Calc stored files workaround (DefColWidth is mandatory according to [MS-XLS])
	bool col_ok = proc.repeated<ColInfo>(0, 255);

	return def_ok || col_ok;
}

} // namespace XLS

