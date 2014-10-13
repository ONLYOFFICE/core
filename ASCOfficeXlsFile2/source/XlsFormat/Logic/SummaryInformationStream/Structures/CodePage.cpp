#include "precompiled_xls.h"

#include "CodePage.h"
#include "Binary\CFStream.h"

namespace OLEPS
{;

CodePage::CodePage(const unsigned __int16 value_type, XLS::CFStreamPtr stream)
{
	//ASSERT(value_type == Property::VT_I2);
	*stream >> code_page;
}

const unsigned __int16 CodePage::GetCodePage()
{
	return code_page;
}


} // namespace OLEPS
