#include "precompiled_xls.h"

#include "SummaryInformation.h"
#include "Binary\CFStream.h"


namespace OLEPS
{;

SummaryInformation::SummaryInformation(XLS::CFStreamPtr stream)
:	property_set_stream(stream)
{
}

const unsigned __int16 SummaryInformation::GetCodePage()
{
	return property_set_stream.GetCodePage();
}

} // namespace OLEPS
