#include "stdafx.h"
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
	record.reserveNBytes(2);
	record << stText;
}


void SeriesText::readFields(CFRecord& record)
{
	record.skipNBytes(2); // reserved
	record >> stText;
}

} // namespace XLS

