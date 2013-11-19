#include "stdafx.h"
#include "HFPicture.h"
#include <XLS_logic/Biff_structures/FrtHeader.h>

namespace XLS
{;

HFPicture::HFPicture()
{
}


HFPicture::~HFPicture()
{
}


BaseObjectPtr HFPicture::clone()
{
	return BaseObjectPtr(new HFPicture(*this));
}


void HFPicture::writeFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_HFPicture);
	record << frtHeader;

#pragma message("####################### HFPicture record is not implemented")
	Log::error("HFPicture record is not implemented.");
	//record << some_value;
}


void HFPicture::readFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_HFPicture);
	record >> frtHeader;

	WORD flags;
	record >> flags;
	fIsDrawing = GETBIT(flags, 0);
	fIsDrawingGroup = GETBIT(flags, 0);
	fContinue = GETBIT(flags, 0);

#pragma message("############################ OfficeArtDggContainer containers are not implemented yet")
	Log::info("OfficeArtDggContainer containers are not implemented yet");
	//record >> rgDrawing;
}

} // namespace XLS

