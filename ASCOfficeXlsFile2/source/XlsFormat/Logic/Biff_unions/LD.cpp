
#include "LD.h"
#include <Logic/Biff_records/Legend.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/Pos.h>
#include <Logic/Biff_unions/ATTACHEDLABEL.h>
#include <Logic/Biff_unions/FRAME.h>
#include <Logic/Biff_records/CrtLayout12.h>
#include <Logic/Biff_records/FontX.h>
#include <Logic/Biff_unions/TEXTPROPS.h>
#include <Logic/Biff_unions/CRTMLFRT.h>
#include <Logic/Biff_records/End.h>

namespace XLS
{


LD::LD()
{
	m_countSeries = 1;
}


LD::~LD()
{
}


BaseObjectPtr LD::clone()
{
	return BaseObjectPtr(new LD(*this));
}


// LD = Legend Begin Pos ATTACHEDLABEL [FRAME] [CrtLayout12] [TEXTPROPS] [CRTMLFRT] CRTMLFRT
const bool LD::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Legend>())
	{
		return false;
	}
	m_Legend = elements_.back();		elements_.pop_back();
	
	proc.mandatory<Begin>();			elements_.pop_back();

	if (proc.optional<Pos>())
	{
		m_Pos = elements_.back();
		elements_.pop_back();
	}

	proc.mandatory<ATTACHEDLABEL>();

	m_ATTACHEDLABEL = elements_.back();
	elements_.pop_back();

	if (proc.optional<FRAME>())
	{
		m_FRAME = elements_.back();
		elements_.pop_back();
	}
	proc.optional<CrtLayout12>();
	
	if (proc.optional<TEXTPROPS>())
	{
		m_TEXTPROPS = elements_.back();
		elements_.pop_back();
	}
	proc.optional<CRTMLFRT>();
	proc.mandatory<End>();				elements_.pop_back();
	return true;
}

int LD::serialize (std::wostream & _stream, const std::wstring & entries)
{
	ATTACHEDLABEL	*att	= dynamic_cast<ATTACHEDLABEL*>	(m_ATTACHEDLABEL.get());
	Legend			*legend = dynamic_cast<Legend *>		(m_Legend.get());
	
	if (legend == NULL) return 0;

	legend->serialize(_stream, m_countSeries);
	
	if (!entries.empty())
		_stream << entries;

	if (m_FRAME)
		m_FRAME->serialize(_stream);

	if (att)
		att->serialize_txPr(_stream);

	return 0;
}

} // namespace XLS

