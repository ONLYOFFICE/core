
#include "DVAXIS.h"
#include <Logic/Biff_records/Axis.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/ValueRange.h>
#include <Logic/Biff_unions/AXM.h>
#include <Logic/Biff_unions/AXS.h>
#include <Logic/Biff_unions/CRTMLFRT.h>
#include <Logic/Biff_records/End.h>

#include <Logic/Biff_records/CatSerRange.h>
#include <Logic/Biff_records/Tick.h>
#include <Logic/Biff_records/FontX.h>
#include <Logic/Biff_records/AxisLine.h>
#include <Logic/Biff_records/LineFormat.h>

namespace XLS
{


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
	m_Axis = elements_.back();
	elements_.pop_back();

	proc.mandatory<Begin>();				elements_.pop_back();
	
	if (proc.optional<ValueRange>())
	{
		m_ValueRange = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<AXM>())
	{
		m_AXM = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.mandatory<AXS>())
	{
		m_AXS = elements_.back();
		elements_.pop_back();
	}

	proc.optional<CRTMLFRT>();

	if (proc.optional<CatSerRange>())
	{
		m_CatSerRange = elements_.back();
		elements_.pop_back();
	}

	if (proc.optional<Tick>())
	{
		m_Tick = elements_.back();
		elements_.pop_back();
	}

	if (proc.optional<FontX>())
	{
		m_FontX = elements_.back();
		elements_.pop_back();
	}

	if (proc.optional<AxisLine>())
	{
		m_AxisLine1 = elements_.back();
		elements_.pop_back();
	}

	if (proc.optional<LineFormat>())
	{
		m_LineFormat1 = elements_.back();
		elements_.pop_back();
	}

	if (proc.optional<AxisLine>())
	{
		m_AxisLine2 = elements_.back();
		elements_.pop_back();
	}

	if (proc.optional<LineFormat>())
	{
		m_LineFormat2 = elements_.back();
		elements_.pop_back();
	}
	
	proc.mandatory<End>();						elements_.pop_back();

	return true;
}
} // namespace XLS

