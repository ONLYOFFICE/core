
#include "Label.h"

namespace XLS
{

Label::Label()
{
}


Label::~Label()
{
}


BaseObjectPtr Label::clone()
{
	return BaseObjectPtr(new Label(*this));
}


void Label::writeFields(CFRecord& record)
{
	record << cell << st;
}


void Label::readFields(CFRecord& record)
{
	record >> cell >> st;
}


} // namespace XLS

