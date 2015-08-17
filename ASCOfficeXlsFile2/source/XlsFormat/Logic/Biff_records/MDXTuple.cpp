
#include "MDXTuple.h"

namespace XLS
{

MDXTuple::MDXTuple()
{
}


MDXTuple::~MDXTuple()
{
}


BaseObjectPtr MDXTuple::clone()
{
	return BaseObjectPtr(new MDXTuple(*this));
}


void MDXTuple::writeFields(CFRecord& record)
{
#pragma message("####################### MDXTuple record is not implemented")
	Log::error("MDXTuple record is not implemented.");
	//record << some_value;
}


void MDXTuple::readFields(CFRecord& record)
{
	record.skipNunBytes(12);
#pragma message("############################ frtHeader skipped here")
	record >> istrConnName >> tfnSrc >> cistr;
	while(!record.isEOF())
	{
		MDXStrIndexPtr element(new MDXStrIndex);
		record >> *element;
		rgistr.push_back(element);
	}
}

} // namespace XLS

