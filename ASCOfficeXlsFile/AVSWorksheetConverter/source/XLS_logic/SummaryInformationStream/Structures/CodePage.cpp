#include "StdAfx.h"
#include "CodePage.h"
#include "XLS_bin\CFStream.h"

namespace OLEPS
{;

CodePage::CodePage(const WORD value_type, XLS::CFStreamPtr stream)
{
	ASSERT(value_type == Property::VT_I2);
	*stream >> code_page;
}

const WORD CodePage::GetCodePage()
{
	return code_page;
}


} // namespace OLEPS
