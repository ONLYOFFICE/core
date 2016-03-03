#include "Format.h"

#include <utils.h>

namespace XLS
{

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
	stFormat = xml::utils::replace_xml_to_text(stFormat.value());
}

int Format::serialize(std::wostream & stream)
{
    CP_XML_WRITER(stream)    
    {
        CP_XML_NODE(L"numFmt")
        {
			CP_XML_ATTR(L"numFmtId", ifmt);
			CP_XML_ATTR(L"formatCode", /*xml::utils::replace_text_to_xml*/(stFormat.value()));
		}
	}
	return 1;
}

} // namespace XLS

