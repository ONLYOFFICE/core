#include "stdafx.h"
#include "PIVOTFORMAT.h"
#include <XLS_logic/Biff_records/SxFormat.h>
#include <XLS_logic/Biff_unions/PIVOTRULE.h>
#include <XLS_logic/Biff_records/SxDXF.h>

namespace XLS
{;


PIVOTFORMAT::PIVOTFORMAT()
{
}


PIVOTFORMAT::~PIVOTFORMAT()
{
}


BaseObjectPtr PIVOTFORMAT::clone()
{
	return BaseObjectPtr(new PIVOTFORMAT(*this));
}


// PIVOTFORMAT = SxFormat PIVOTRULE [SxDXF]
const bool PIVOTFORMAT::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SxFormat>())
	{
		return false;
	}
	proc.mandatory<PIVOTRULE>();
	proc.optional<SxDXF>();

	return true;
}

} // namespace XLS

