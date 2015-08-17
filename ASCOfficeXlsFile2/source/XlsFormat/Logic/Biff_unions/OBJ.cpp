
#include "OBJ.h"
#include <Logic/Biff_records/Obj.h>
#include <Logic/Biff_records/Continue.h>

namespace XLS
{


OBJ::OBJ()
{
}


OBJ::~OBJ()
{
}


BaseObjectPtr OBJ::clone()
{
	return BaseObjectPtr(new OBJ(*this));
}


// OBJ = Obj *Continue
const bool OBJ::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Obj>())
	{
		return false;
	}

	m_Obj = elements_.back();
	elements_.pop_back();

	return true;
}

} // namespace XLS

