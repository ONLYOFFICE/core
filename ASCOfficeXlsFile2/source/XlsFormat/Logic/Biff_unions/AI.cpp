
#include "AI.h"
#include <Logic/Biff_records/BRAI.h>
#include <Logic/Biff_records/SeriesText.h>

//#include <Logic/Biff_unions/PAGESETUP.h>

namespace XLS
{


AI::AI()
{
}


AI::~AI()
{
}


BaseObjectPtr AI::clone()
{
	return BaseObjectPtr(new AI(*this));
}


// AI = BRAI [SeriesText]
const bool AI::loadContent(BinProcessor& proc)
{
	// fix
	if(!proc.optional<BRAI>())
	{
		return false;
	}
	m_BRAI = elements_.back();
	elements_.pop_back();

	if (proc.optional<SeriesText>())
	{
		m_SeriesText = elements_.back();
		elements_.pop_back();
	}

	return true;
}

} // namespace XLS

