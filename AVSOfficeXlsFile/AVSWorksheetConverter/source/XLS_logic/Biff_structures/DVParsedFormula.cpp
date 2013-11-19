#include "stdafx.h"
#include "DVParsedFormula.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


DVParsedFormula::DVParsedFormula()
:	ParsedFormula(CellRef())
{
}


BiffStructurePtr DVParsedFormula::clone()
{
	return BiffStructurePtr(new DVParsedFormula(*this));
}


void DVParsedFormula::store(CFRecord& record)
{
#pragma message("####################### DVParsedFormula record has no BiffStructure::store() implemented")
	Log::error(" Error!!! DVParsedFormula record has no BiffStructure::store() implemented.");
	//record << something;
}


void DVParsedFormula::load(CFRecord& record)
{
	WORD cce;
	record >> cce;
	record.skipNBytes(2); // unused
	rgce.load(record, cce);
}



} // namespace XLS

