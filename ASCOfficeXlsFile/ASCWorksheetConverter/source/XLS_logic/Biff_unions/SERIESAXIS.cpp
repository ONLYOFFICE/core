#include "stdafx.h"
#include "SERIESAXIS.h"
#include <XLS_logic/Biff_records/Axis.h>
#include <XLS_logic/Biff_records/Begin.h>
#include <XLS_logic/Biff_records/CatSerRange.h>
#include <XLS_logic/Biff_unions/AXS.h>
#include <XLS_logic/Biff_unions/CRTMLFRT.h>
#include <XLS_logic/Biff_records/End.h>

namespace XLS
{;


SERIESAXIS::SERIESAXIS()
{
}


SERIESAXIS::~SERIESAXIS()
{
}


BaseObjectPtr SERIESAXIS::clone()
{
	return BaseObjectPtr(new SERIESAXIS(*this));
}


const bool SERIESAXIS::loadContentRead(BinReaderProcessor& proc)
{
	bool ok = loadContent(proc);
	if(ok)
	{
		id = proc.getGlobalWorkbookInfo()->GenerateAXESId();
	}
	return ok;
}

// SERIESAXIS = Axis Begin [CatSerRange] AXS [CRTMLFRT] End
const bool SERIESAXIS::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Axis>())
	{
		return false;
	}
	proc.mandatory<Begin>();
	proc.optional<CatSerRange>();
	proc.mandatory<AXS>();
	proc.optional<CRTMLFRT>();
	proc.mandatory<End>();

	return true;
}

void SERIESAXIS::toXML(MSXML2::IXMLDOMElementPtr own_tag)
{
	own_tag->setAttribute(L"id", id);
}

} // namespace XLS

