
#include "Format.h"

#include <simple_xml_writer.h>

namespace XLS
{;

Format::Format()
{
}


Format::~Format()
{
}


BaseObjectPtr Format::clone()
{
	return BaseObjectPtr(new Format(*this));
}


void Format::writeFields(CFRecord& record)
{
	record << ifmt << stFormat;
}


void Format::readFields(CFRecord& record)
{
	record >> ifmt >> stFormat;
}
int Format::serialize(std::wostream & stream)
{
    CP_XML_WRITER(stream)    
    {
        CP_XML_NODE(L"numFmt")
        {
			CP_XML_ATTR(L"numFmtId", ifmt);
			CP_XML_ATTR(L"formatCode", stFormat.value());
		}
	}
	return 0;
}

} // namespace XLS

