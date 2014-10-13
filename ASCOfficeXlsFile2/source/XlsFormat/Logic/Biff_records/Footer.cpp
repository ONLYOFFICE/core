#include "precompiled_xls.h"
#include "Footer.h"

namespace XLS
{;

Footer::Footer()
{
}


Footer::~Footer()
{
}


BaseObjectPtr Footer::clone()
{
	return BaseObjectPtr(new Footer(*this));
}


void Footer::writeFields(CFRecord& record)
{
	if(ast.getSize())
	{
		record << ast;
	}
}


void Footer::readFields(CFRecord& record)
{
	if(!record.isEOF())
	{
		record >> ast;
	}
}

} // namespace XLS

