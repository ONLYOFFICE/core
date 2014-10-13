#include "precompiled_xls.h"
#include "SXSRC.h"
#include <Logic/Biff_unions/DREF.h>
#include <Logic/Biff_unions/SXTBL.h>
#include <Logic/Biff_unions/DBQUERY.h>

namespace XLS
{;


SXSRC::SXSRC()
{
}


SXSRC::~SXSRC()
{
}


BaseObjectPtr SXSRC::clone()
{
	return BaseObjectPtr(new SXSRC(*this));
}


// SXSRC = DREF / SXTBL / DBQUERY
const bool SXSRC::loadContent(BinProcessor& proc)
{
	if(!proc.optional<DREF>())
	{
		if(!proc.optional<SXTBL>())
		{
			if(!proc.optional<DBQUERY>())
			{
				return false;
			}
		}
	}
	return true;
}

} // namespace XLS

