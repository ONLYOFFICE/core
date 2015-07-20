
#include "FrtFontList.h"

namespace XLS
{;

FrtFontList::FrtFontList()
{
}


FrtFontList::~FrtFontList()
{
}


BaseObjectPtr FrtFontList::clone()
{
	return BaseObjectPtr(new FrtFontList(*this));
}


void FrtFontList::writeFields(CFRecord& record)
{
#pragma message("####################### FrtFontList record is not implemented")
	Log::error("FrtFontList record is not implemented.");
	//record << some_value;
}


void FrtFontList::readFields(CFRecord& record)
{
#pragma message("####################### FrtFontList record is not implemented")
	Log::error("FrtFontList record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

