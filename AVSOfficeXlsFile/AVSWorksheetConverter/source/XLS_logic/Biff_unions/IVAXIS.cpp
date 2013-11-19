#include "stdafx.h"
#include "IVAXIS.h"
#include <XLS_logic/Biff_records/Axis.h>
#include <XLS_logic/Biff_records/Begin.h>
#include <XLS_logic/Biff_records/CatSerRange.h>
#include <XLS_logic/Biff_records/AxcExt.h>
#include <XLS_logic/Biff_records/CatLab.h>
#include <XLS_logic/Biff_unions/AXS.h>
#include <XLS_logic/Biff_unions/CRTMLFRT.h>
#include <XLS_logic/Biff_records/End.h>

namespace XLS
{;


IVAXIS::IVAXIS()
:	id(0)
{
}


IVAXIS::~IVAXIS()
{
}


BaseObjectPtr IVAXIS::clone()
{
	return BaseObjectPtr(new IVAXIS(*this));
}


const bool IVAXIS::loadContentRead(BinReaderProcessor& proc)
{
	bool ok = loadContent(proc);
	if(ok)
	{
		id = proc.getGlobalWorkbookInfo()->GenerateAXESId();
	}
	return ok;
}

// IVAXIS = Axis Begin [CatSerRange] AxcExt [CatLab] AXS [CRTMLFRT] End
const bool IVAXIS::loadContent(BinProcessor& proc)
{
	// This is the way to distinguish IVAXIS and DVAXIS
	if(!proc.checkNextRecord(rt_AxcExt, 4) && !proc.checkNextRecord(rt_CatSerRange, 4))
	{
		return false;
	}
	if(!proc.mandatory<Axis>())
	{
		return false;
	}
	proc.mandatory<Begin>();
	proc.optional<CatSerRange>();
	proc.mandatory<AxcExt>(); // 4-th place
	proc.optional<CatLab>();
	proc.mandatory<AXS>();
	proc.optional<CRTMLFRT>();
	
	// fix
	proc.optional<CRTMLFRT>();

	proc.mandatory<End>();

	return true;
}

void IVAXIS::toXML(MSXML2::IXMLDOMElementPtr own_tag)
{
	own_tag->setAttribute(L"id", id);
}

} // namespace XLS

