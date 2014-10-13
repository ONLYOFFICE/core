#include "precompiled_xls.h"
#include "VerticalPageBreaks.h"

namespace XLS
{;

VerticalPageBreaks::VerticalPageBreaks()
{
}


VerticalPageBreaks::~VerticalPageBreaks()
{
}


BaseObjectPtr VerticalPageBreaks::clone()
{
	return BaseObjectPtr(new VerticalPageBreaks(*this));
}


void VerticalPageBreaks::writeFields(CFRecord& record)
{
	record << cbrk;
	for(BiffStructurePtrVector::const_iterator it = rgbrk.begin(), itEnd = rgbrk.end(); it != itEnd; ++it)
	{
		record << **it;
	}
}


void VerticalPageBreaks::readFields(CFRecord& record)
{
	record >> cbrk;
	for (int i = 0; i < cbrk ; ++i)
	{
		VertBrkPtr vb(new VertBrk);
		record >> *vb;
		rgbrk.push_back(vb);
	}
}

} // namespace XLS

