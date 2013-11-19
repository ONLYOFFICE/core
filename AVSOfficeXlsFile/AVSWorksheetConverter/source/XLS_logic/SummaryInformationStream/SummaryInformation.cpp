#include "StdAfx.h"
#include "SummaryInformation.h"
#include "XLS_bin\CFStream.h"


namespace OLEPS
{;

SummaryInformation::SummaryInformation(XLS::CFStreamPtr stream)
:	property_set_stream(stream)
{
}

const WORD SummaryInformation::GetCodePage()
{
	return property_set_stream.GetCodePage();
}

} // namespace OLEPS
