
#include "HorizontalPageBreaks.h"

namespace XLS
{

HorizontalPageBreaks::HorizontalPageBreaks()
{
}


HorizontalPageBreaks::~HorizontalPageBreaks()
{
}


BaseObjectPtr HorizontalPageBreaks::clone()
{
	return BaseObjectPtr(new HorizontalPageBreaks(*this));
}


void HorizontalPageBreaks::writeFields(CFRecord& record)
{
	record << cbrk;
	for(BiffStructurePtrVector::const_iterator it = rgbrk.begin(), itEnd = rgbrk.end(); it != itEnd; ++it)
	{
		record << **it;
	}
}


void HorizontalPageBreaks::readFields(CFRecord& record)
{
	record >> cbrk;
	for (int i = 0; i < cbrk ; i++)
	{
		HorzBrkPtr hb(new HorzBrk);
		record >> *hb;
		rgbrk.push_back(hb);
	}
}

} // namespace XLS

