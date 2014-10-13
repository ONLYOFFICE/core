#include "precompiled_xls.h"
#include "PIVOTFORMAT.h"
#include <Logic/Biff_records/SxFormat.h>
#include <Logic/Biff_unions/PIVOTRULE.h>
#include <Logic/Biff_records/SxDXF.h>

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

