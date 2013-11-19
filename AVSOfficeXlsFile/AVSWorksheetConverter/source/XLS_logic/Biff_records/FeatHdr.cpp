#include "stdafx.h"
#include "FeatHdr.h"
#include <XLS_logic/Biff_structures/FrtHeader.h>

namespace XLS
{;

FeatHdr::FeatHdr(const bool is_called_from_Globals)
:	is_contained_in_Globals(is_called_from_Globals)
{
}


FeatHdr::~FeatHdr()
{
}


BaseObjectPtr FeatHdr::clone()
{
	return BaseObjectPtr(new FeatHdr(*this));
}


void FeatHdr::writeFields(CFRecord& record)
{
#pragma message("####################### FeatHdr record is not implemented")
	Log::error("FeatHdr record is not implemented.");
	//record << some_value;
}


void FeatHdr::readFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_FeatHdr);
	record >> frtHeader;
	record >> isf;
	record.skipNBytes(1); // reserved
	record >> cbHdrData;
	if(cbHdrData)
	{
		switch(isf)
		{
			case SharedFeatureType::ISFPROTECTION:
				record >> protection;
				break;
			case SharedFeatureType::ISFFACTOID:
				if(is_contained_in_Globals)
				{
					record >> prop;
				}
				break;
		}
	}

}

} // namespace XLS

