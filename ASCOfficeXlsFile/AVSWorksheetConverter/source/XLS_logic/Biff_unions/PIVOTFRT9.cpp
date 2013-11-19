#include "stdafx.h"
#include "PIVOTFRT9.h"
#include <XLS_logic/Biff_records/QsiSXTag.h>
#include <XLS_logic/Biff_unions/DBQUERYEXT.h>
#include <XLS_logic/Biff_unions/PIVOTVIEWEX.h>
#include <XLS_logic/Biff_records/SXViewEx9.h>

namespace XLS
{;


PIVOTFRT9::PIVOTFRT9()
{
}


PIVOTFRT9::~PIVOTFRT9()
{
}


BaseObjectPtr PIVOTFRT9::clone()
{
	return BaseObjectPtr(new PIVOTFRT9(*this));
}


// PIVOTFRT9 = QsiSXTag [DBQUERYEXT] [PIVOTVIEWEX] SXViewEx9
const bool PIVOTFRT9::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<QsiSXTag>())
	{
		return false;
	}
	proc.optional<DBQUERYEXT>();
	proc.optional<PIVOTVIEWEX>();
	proc.mandatory<SXViewEx9>();

	return true;
}

} // namespace XLS

