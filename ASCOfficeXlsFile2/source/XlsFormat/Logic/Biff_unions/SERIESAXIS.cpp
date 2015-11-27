
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
	m_Axis = elements_.back();
	elements_.pop_back();

	proc.mandatory<Begin>();				elements_.pop_back();
	
	if (proc.optional<CatSerRange>())
	{
		m_CatSerRange = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.mandatory<AXS>())
	{
		m_AXS = elements_.back();
		elements_.pop_back();
	}

	proc.optional<CRTMLFRT>();
	proc.mandatory<End>();					elements_.pop_back();

	return true;
}

} // namespace XLS

