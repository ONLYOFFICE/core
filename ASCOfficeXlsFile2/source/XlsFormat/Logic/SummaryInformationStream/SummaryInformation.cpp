

#include "SummaryInformation.h"
#include "Binary/CFStream.h"


namespace OLEPS
{

SummaryInformation::SummaryInformation(XLS::CFStreamPtr stream)
:	property_set_stream(stream)
{
}

const unsigned short SummaryInformation::GetCodePage()
{
	return property_set_stream.GetCodePage();
}

} // namespace OLEPS
