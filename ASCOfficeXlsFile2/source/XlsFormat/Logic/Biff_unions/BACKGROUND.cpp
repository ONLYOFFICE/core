
#include "BACKGROUND.h"
#include <Logic/Biff_records/BkHim.h>
#include <Logic/Biff_records/Continue.h>

namespace XLS
{


BACKGROUND::BACKGROUND()
{
}


BACKGROUND::~BACKGROUND()
{
}


BaseObjectPtr BACKGROUND::clone()
{
	return BaseObjectPtr(new BACKGROUND(*this));
}


// BACKGROUND = BkHim *Continue
const bool BACKGROUND::loadContent(BinProcessor& proc)
{
	pGlobalWorkbookInfoPtr = proc.getGlobalWorkbookInfo();

	if(!proc.mandatory<BkHim>())
	{
		return false;
	}
	m_BkHim = elements_.back();
	elements_.pop_back();

	int count = proc.repeated<Continue>(0, 0);

	return true;
}
int BACKGROUND::serialize(std::wostream & _stream)
{
	return 0;
}

} // namespace XLS

