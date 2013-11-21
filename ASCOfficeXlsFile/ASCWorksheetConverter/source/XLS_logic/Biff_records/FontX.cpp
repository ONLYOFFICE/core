#include "stdafx.h"
#include "FontX.h"

namespace XLS
{;

FontX::FontX()
{
}


FontX::~FontX()
{
}


BaseObjectPtr FontX::clone()
{
	return BaseObjectPtr(new FontX(*this));
}


void FontX::writeFields(CFRecord& record)
{
	record << iFont;
}


void FontX::readFields(CFRecord& record)
{
	record >> iFont;
}

} // namespace XLS

