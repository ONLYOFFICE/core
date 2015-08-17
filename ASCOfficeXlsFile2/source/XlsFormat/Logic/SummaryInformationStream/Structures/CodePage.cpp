

#include "CodePage.h"
#include <Binary/CFStream.h>

namespace OLEPS
{

CodePage::CodePage(const unsigned short value_type, XLS::CFStreamPtr stream)
{
	//ASSERT(value_type == Property::VT_I2);
	*stream >> code_page;
}

const unsigned short CodePage::GetCodePage()
{
	return code_page;
}


} // namespace OLEPS
