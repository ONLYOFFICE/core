
#include "SeriesText.h"

namespace XLS
{

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

	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		ShortXLAnsiString s;
		record >> s;
		stText = s;
	}
	else
		record >> stText;
}

} // namespace XLS

