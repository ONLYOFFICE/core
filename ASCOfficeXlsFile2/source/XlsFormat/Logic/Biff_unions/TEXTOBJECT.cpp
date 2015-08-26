
#include "TEXTOBJECT.h"
#include <Logic/Biff_records/TxO.h>
#include <Logic/Biff_records/Continue.h>
#include <Logic/Biff_records/MsoDrawing.h>

namespace XLS
{


TEXTOBJECT::~TEXTOBJECT()
{
}


BaseObjectPtr TEXTOBJECT::clone()
{
	return BaseObjectPtr(new TEXTOBJECT(*this));
}


// TEXTOBJECT = TxO *Continue
const bool TEXTOBJECT::loadContent(BinProcessor& proc)
{
	TxO TxO_(mso_drawing_);
	if(!proc.mandatory(TxO_))
	{
		return false;
	}
	m_TxO = elements_.back();
	elements_.pop_back();
	//proc.repeated<Continue>(0, 0);

	return true;
}

} // namespace XLS

