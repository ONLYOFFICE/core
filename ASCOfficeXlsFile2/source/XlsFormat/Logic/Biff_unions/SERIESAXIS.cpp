
#include "SERIESAXIS.h"
#include <Logic/Biff_records/Axis.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/CatSerRange.h>
#include <Logic/Biff_unions/AXS.h>
#include <Logic/Biff_unions/CRTMLFRT.h>
#include <Logic/Biff_records/End.h>

namespace XLS
{


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

//void SERIESAXIS::toXML(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	own_tag->setAttribute(L"id", id);
//}

} // namespace XLS

