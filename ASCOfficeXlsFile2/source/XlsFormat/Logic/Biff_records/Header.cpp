#include "precompiled_xls.h"
#include "Header.h"

namespace XLS
{;

Header::Header()
{
}


Header::~Header()
{
}


BaseObjectPtr Header::clone()
{
	return BaseObjectPtr(new Header(*this));
}


void Header::writeFields(CFRecord& record)
{
	if(ast.getSize())
	{
		record << ast;
	}
}


void Header::readFields(CFRecord& record)
{
	if(!record.isEOF())
	{
		record >> ast;
	}
}

} // namespace XLS

