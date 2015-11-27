
#include "IVAXIS.h"
#include <Logic/Biff_records/Axis.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/CatSerRange.h>
#include <Logic/Biff_records/AxcExt.h>
#include <Logic/Biff_records/CatLab.h>
#include <Logic/Biff_unions/AXS.h>
#include <Logic/Biff_unions/CRTMLFRT.h>
#include <Logic/Biff_records/End.h>

namespace XLS
{


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
	m_Axis = elements_.back();
	elements_.pop_back();

	proc.mandatory<Begin>();					elements_.pop_back();
	
	if (proc.optional<CatSerRange>())
	{
		m_CatSerRange = elements_.back();
		elements_.pop_back();
	}

	if (proc.mandatory<AxcExt>()) // 4-th place
	{
		m_AxcExt = elements_.back();
		elements_.pop_back();
	}

	if (proc.optional<CatLab>())
	{
		m_CatLab = elements_.back();
		elements_.pop_back();
	}
	if (proc.mandatory<AXS>())
	{
		m_AXS = elements_.back();
		elements_.pop_back();
	}
	proc.optional<CRTMLFRT>();
	
	// fix
	proc.optional<CRTMLFRT>();

	proc.mandatory<End>();						elements_.pop_back();

	return true;
}

} // namespace XLS

