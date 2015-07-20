
#include "QUERYTABLE.h"
#include <Logic/Biff_records/Qsi.h>
#include <Logic/Biff_records/QsiSXTag.h>
#include <Logic/Biff_unions/DBQUERY.h>
#include <Logic/Biff_unions/DBQUERYEXT.h>
#include <Logic/Biff_unions/SXADDLQSI.h>
#include <Logic/Biff_unions/QSIR.h>
#include <Logic/Biff_unions/SORTDATA12.h>

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

