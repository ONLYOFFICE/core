#include "stdafx.h"
#include "DVAXIS.h"
#include <XLS_logic/Biff_records/Axis.h>
#include <XLS_logic/Biff_records/Begin.h>
#include <XLS_logic/Biff_records/ValueRange.h>
#include <XLS_logic/Biff_unions/AXM.h>
#include <XLS_logic/Biff_unions/AXS.h>
#include <XLS_logic/Biff_unions/CRTMLFRT.h>
#include <XLS_logic/Biff_records/End.h>

#include <XLS_logic/Biff_records/CatSerRange.h>
#include <XLS_logic/Biff_records/Tick.h>
#include <XLS_logic/Biff_records/FontX.h>
#include <XLS_logic/Biff_records/AxisLine.h>
#include <XLS_logic/Biff_records/LineFormat.h>

namespace XLS
{;


DVAXIS::DVAXIS()
:	id(0)
{
}


DVAXIS::~DVAXIS()
{
}


BaseObjectPtr DVAXIS::clone()
{
	return BaseObjectPtr(new DVAXIS(*this));
}


const bool DVAXIS::loadContentRead(BinReaderProcessor& proc)
{
	bool ok = loadContent(proc);
	if(ok)
	{
		id = proc.getGlobalWorkbookInfo()->GenerateAXESId();
	}
	return ok;
}

// DVAXIS = Axis Begin [ValueRange] [AXM] AXS [CRTMLFRT] End
const bool DVAXIS::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Axis>())
	{
		return false;
	}
	proc.mandatory<Begin>();
	proc.optional<ValueRange>();
	proc.optional<AXM>();
	proc.mandatory<AXS>();
	proc.optional<CRTMLFRT>();

	// fix
	proc.optional<CatSerRange>();
	proc.optional<Tick>();
	proc.optional<FontX>();
	proc.optional<AxisLine>();
	proc.optional<LineFormat>();

	proc.optional<AxisLine>();
	proc.optional<LineFormat>();
	
	proc.mandatory<End>();

	return true;
}

void DVAXIS::toXML(MSXML2::IXMLDOMElementPtr own_tag)
{
	own_tag->setAttribute(L"id", id);
}

} // namespace XLS

