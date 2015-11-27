
#include "AXISPARENT.h"
#include <Logic/Biff_records/AxisParent.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/Pos.h>
#include <Logic/Biff_records/End.h>

#include <Logic/Biff_unions/AXES.h>
#include <Logic/Biff_unions/CRT.h>

namespace XLS
{


AXISPARENT::AXISPARENT()
{
}


AXISPARENT::~AXISPARENT()
{
}


BaseObjectPtr AXISPARENT::clone()
{
	return BaseObjectPtr(new AXISPARENT(*this));
}


// AXISPARENT = AxisParent Begin Pos [AXES] 1*4CRT End
const bool AXISPARENT::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<AxisParent>())
	{
		return false;
	}
	m_AxisParent = elements_.back();
	elements_.pop_back();
	
	proc.mandatory<Begin>();		elements_.pop_back();

	if (proc.optional<Pos>())
	{
		m_Pos = elements_.back();
		elements_.pop_back(); 
	}

	if (proc.optional<AXES>())
	{
		m_AXES = elements_.back();
		elements_.pop_back(); 
	}
	
	int count  = proc.repeated<CRT>(1, 4);
	while(count > 0)
	{
		m_arCRT.insert(m_arCRT.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}	

	proc.mandatory<End>();			elements_.pop_back();

	return true;
}

} // namespace XLS

