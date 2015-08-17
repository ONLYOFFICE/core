
#include "MSODRAWINGGROUP.h"
#include <Logic/Biff_records/MsoDrawingGroup.h>
#include <Logic/Biff_records/Continue.h>

namespace XLS
{


MSODRAWINGGROUP::MSODRAWINGGROUP(const bool is_inside_chart_sheet)
:	is_inside_chart_sheet_(is_inside_chart_sheet)
{
}


MSODRAWINGGROUP::~MSODRAWINGGROUP()
{
}


BaseObjectPtr MSODRAWINGGROUP::clone()
{
	return BaseObjectPtr(new MSODRAWINGGROUP(*this));
}


// MSODRAWINGGROUP = MsoDrawingGroup *Continue
const bool MSODRAWINGGROUP::loadContent(BinProcessor& proc)
{
    MsoDrawingGroup mso_drawing_group(is_inside_chart_sheet_);
    if(!proc.mandatory(mso_drawing_group))
	{		
		return false;
	}
	m_MsoDrawingGroup = elements_.back();
	elements_.pop_back();

	proc.repeated<Continue>(0, 0);

	return true;
}

} // namespace XLS

