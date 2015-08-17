
#include "Lel.h"

namespace XLS
{

Lel::Lel()
{
}


Lel::~Lel()
{
}


BaseObjectPtr Lel::clone()
{
	return BaseObjectPtr(new Lel(*this));
}


void Lel::writeFields(CFRecord& record)
{
	record << stFormulaName;
}


void Lel::readFields(CFRecord& record)
{
	record >> stFormulaName;
}

} // namespace XLS

