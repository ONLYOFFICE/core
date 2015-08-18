

#include "CodePageOle.h"
#include <Binary/CFStream.h>

namespace OLEPS
{

CodePageOle::CodePageOle(const unsigned short value_type, XLS::CFStreamPtr stream)
{
	//ASSERT(value_type == Property::VT_I2);
	*stream >> code_page;
}

const unsigned short CodePageOle::GetCodePage()
{
	return code_page;
}


} // namespace OLEPS
