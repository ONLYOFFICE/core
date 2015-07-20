
#include "DefaultText.h"

namespace XLS
{;

DefaultText::DefaultText()
{
}


DefaultText::~DefaultText()
{
}


BaseObjectPtr DefaultText::clone()
{
	return BaseObjectPtr(new DefaultText(*this));
}


void DefaultText::writeFields(CFRecord& record)
{
	record << id;
}


void DefaultText::readFields(CFRecord& record)
{
	record >> id;
}

} // namespace XLS

