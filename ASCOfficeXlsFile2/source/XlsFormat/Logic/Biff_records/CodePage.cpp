
#include "CodePage.h"

namespace XLS
{

CodePage::CodePage()
{
}


CodePage::~CodePage()
{
}


BaseObjectPtr CodePage::clone()
{
	return BaseObjectPtr(new CodePage(*this));
}


void CodePage::writeFields(CFRecord& record)
{
	record << cv;
}


void CodePage::readFields(CFRecord& record)
{
	record >> cv;
}

} // namespace XLS

