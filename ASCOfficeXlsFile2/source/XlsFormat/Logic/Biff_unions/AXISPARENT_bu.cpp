
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

	int count = proc.repeated<AXES>(0, 0);
	if (count > 0)
	{
		m_AXES = elements_.back();
		elements_.pop_back(); 
	}
	
	count  = proc.repeated<CRT>(1, 4);

	while(count > 0 && !elements_.empty())
	{
		m_arCRT.insert(m_arCRT.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}	

	if (proc.mandatory<End>())			elements_.pop_back();

	return true;
}

void AXISPARENT::concatinate_second (BaseObjectPtr & addit)
{
	AXISPARENT * second = dynamic_cast<AXISPARENT *>(addit.get());

	if (second == NULL) return;

	for (int i = 0; i < second->m_arCRT.size(); i++)
	{
		CRT* crt = dynamic_cast<CRT*>(second->m_arCRT[i].get());
		crt->m_indAXISPARENT = 1;

		m_arCRT.push_back(second->m_arCRT[i]);
	}

}


} // namespace XLS

