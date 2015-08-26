
#include "OBJ.h"
#include <Logic/Biff_records/Obj.h>
#include <Logic/Biff_records/Continue.h>

namespace XLS
{
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
	Obj Obj_(mso_drawing_);

	if(!proc.mandatory(Obj_))
	{
		return false;
	}

	m_Obj = elements_.back();
	elements_.pop_back();

	return true;
}

} // namespace XLS

