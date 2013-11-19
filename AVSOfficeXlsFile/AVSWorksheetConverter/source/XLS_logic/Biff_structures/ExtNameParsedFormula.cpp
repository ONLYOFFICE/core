#include "stdafx.h"
#include "ExtNameParsedFormula.h"
#include <XLS_bin/CFRecord.h>
#include "ExtPtgRef3D.h"
#include "ExtPtgArea3D.h"
#include "ExtPtgRefErr3D.h"
#include "ExtPtgAreaErr3D.h"
#include "ExtPtgErr.h"
#include <Exception/UnexpectedProgramPath.h>

namespace XLS
{;


ExtNameParsedFormula::ExtNameParsedFormula()
:	ParsedFormula(CellRef())
{
}


BiffStructurePtr ExtNameParsedFormula::clone()
{
	return BiffStructurePtr(new ExtNameParsedFormula(*this));
}


void ExtNameParsedFormula::store(CFRecord& record)
{
#pragma message("####################### ExtNameParsedFormula record has no BiffStructure::store() implemented")
	Log::error(" Error!!! ExtNameParsedFormula record has no BiffStructure::store() implemented.");
	//record << something;
}


void ExtNameParsedFormula::load(CFRecord& record) // Maybe this class shouldn't be a ParsedFormulas's derived?
{
	WORD cb;
	record >> cb;
	if(cb)
	{
		BYTE extPtg;
		record >> extPtg;
		switch(extPtg)
		{
			case 0x3A:
				val = PtgPtr(new ExtPtgRef3D(CellRef()));
				break;
			case 0x3B:
				val = PtgPtr(new ExtPtgArea3D(CellRef()));
				break;
			case 0x3C:
				val = PtgPtr(new ExtPtgRefErr3D);
				break;
			case 0x3D:
				val = PtgPtr(new ExtPtgAreaErr3D);
				break;
			case 0x1C:
				val = PtgPtr(new ExtPtgErr);
				break;
		}
		val->load(record);
		rgce.addPtg(val);
	}
}


} // namespace XLS

