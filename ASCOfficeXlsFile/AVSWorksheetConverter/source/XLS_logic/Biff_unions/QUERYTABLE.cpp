#include "stdafx.h"
#include "QUERYTABLE.h"
#include <XLS_logic/Biff_records/Qsi.h>
#include <XLS_logic/Biff_records/QsiSXTag.h>
#include <XLS_logic/Biff_unions/DBQUERY.h>
#include <XLS_logic/Biff_unions/DBQUERYEXT.h>
#include <XLS_logic/Biff_unions/SXADDLQSI.h>
#include <XLS_logic/Biff_unions/QSIR.h>
#include <XLS_logic/Biff_unions/SORTDATA12.h>

namespace XLS
{;


QUERYTABLE::QUERYTABLE()
{
}


QUERYTABLE::~QUERYTABLE()
{
}


BaseObjectPtr QUERYTABLE::clone()
{
	return BaseObjectPtr(new QUERYTABLE(*this));
}


// QUERYTABLE = Qsi DBQUERY QsiSXTag DBQUERYEXT [SXADDLQSI] [QSIR] [SORTDATA12]
const bool QUERYTABLE::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Qsi>())
	{
		return false;
	}
	proc.mandatory<DBQUERY>();
	proc.mandatory<QsiSXTag>();
	proc.mandatory<DBQUERYEXT>();
	proc.optional<SXADDLQSI>();
	proc.optional<QSIR>();
	proc.optional<SORTDATA12>();

	return true;
}

} // namespace XLS

