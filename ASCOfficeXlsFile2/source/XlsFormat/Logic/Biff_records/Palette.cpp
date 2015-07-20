
#include "Palette.h"

namespace XLS
{;

Palette::Palette()
:	ccv(56)
{
}


Palette::Palette(const unsigned short ccv_init)
:	ccv(ccv_init)
{
}


BaseObjectPtr Palette::clone()
{
	return BaseObjectPtr(new Palette(*this));
}


void Palette::writeFields(CFRecord& record)
{
	record << ccv;
	for(BiffStructurePtrVector::const_iterator it = rgColor.begin(), itEnd = rgColor.end(); it != itEnd; ++it)
	{
		record << **it;
	}
}


void Palette::readFields(CFRecord& record)
{
	unsigned short ccv;
	record >> ccv;
	for(int i = 0; i < ccv; ++i)
	{
		LongRGBPtr rgb(new LongRGB);
		record >> *rgb;
		rgColor.push_back(rgb);
	}
}

} // namespace XLS

