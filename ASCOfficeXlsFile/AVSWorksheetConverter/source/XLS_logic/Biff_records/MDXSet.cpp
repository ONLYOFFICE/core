#include "stdafx.h"
#include "MDXSet.h"

namespace XLS
{;

MDXSet::MDXSet()
{
}


MDXSet::~MDXSet()
{
}


BaseObjectPtr MDXSet::clone()
{
	return BaseObjectPtr(new MDXSet(*this));
}


void MDXSet::writeFields(CFRecord& record)
{
#pragma message("####################### MDXSet record is not implemented")
	Log::error("MDXSet record is not implemented.");
	//record << some_value;
}


void MDXSet::readFields(CFRecord& record)
{
	record.skipNBytes(12);
#pragma message("############################ frtHeader skipped here")
	record >> istrConnName >> tfnSrc >> sso >> istrSetDef >> cistr;
	while(!record.isEOF())
	{
		MDXStrIndexPtr element(new MDXStrIndex);
		record >> *element;
		rgistr.push_back(element);
	}
}

} // namespace XLS

