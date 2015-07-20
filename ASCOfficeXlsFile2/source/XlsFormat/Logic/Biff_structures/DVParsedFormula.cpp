
#include "DVParsedFormula.h"
#include <Binary/CFRecord.h>

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
	unsigned short cce;
	record >> cce;
	record.skipNunBytes(2); // unused
	rgce.load(record, cce);
}



} // namespace XLS

