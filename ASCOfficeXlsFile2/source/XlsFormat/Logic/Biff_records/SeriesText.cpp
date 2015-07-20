
#include "SeriesText.h"

namespace XLS
{;

SeriesText::SeriesText()
{
}


SeriesText::~SeriesText()
{
}


BaseObjectPtr SeriesText::clone()
{
	return BaseObjectPtr(new SeriesText(*this));
}


void SeriesText::writeFields(CFRecord& record)
{
	record.reserveNunBytes(2);
	record << stText;
}


void SeriesText::readFields(CFRecord& record)
{
	record.skipNunBytes(2); // reserved
	record >> stText;
}

} // namespace XLS

