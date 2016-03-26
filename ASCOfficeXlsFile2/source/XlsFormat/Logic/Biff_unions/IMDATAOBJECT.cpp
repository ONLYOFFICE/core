
#include "IMDATAOBJECT.h"
#include <Logic/Biff_records/IMDATA.h>
#include <Logic/Biff_records/Continue.h>

namespace XLS
{


IMDATAOBJECT::~IMDATAOBJECT()
{
}


BaseObjectPtr IMDATAOBJECT::clone()
{
	return BaseObjectPtr(new IMDATAOBJECT(*this));
}


// IMDATAOBJECT = IMDATA *Continue // Biff5
const bool IMDATAOBJECT::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<IMDATA>())
	{
		return false;
	}
	m_IMDATA = elements_.back();
	elements_.pop_back();

	return true;
}

} // namespace XLS

