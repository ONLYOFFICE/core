#include "stdafx.h"
#include "CompressPictures.h"
#include <XLS_logic/Biff_structures/FrtHeader.h>

namespace XLS
{;

CompressPictures::CompressPictures()
{
}


CompressPictures::~CompressPictures()
{
}


BaseObjectPtr CompressPictures::clone()
{
	return BaseObjectPtr(new CompressPictures(*this));
}


void CompressPictures::writeFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_CompressPictures);
	record << frtHeader;
	record << fAutoCompressPictures;
}


void CompressPictures::readFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_CompressPictures);
	record >> frtHeader;
	record >> fAutoCompressPictures;
}

} // namespace XLS

