#include "stdafx.h"
#include "PicF.h"

namespace XLS
{;

PicF::PicF()
{
}


PicF::~PicF()
{
}


BaseObjectPtr PicF::clone()
{
	return BaseObjectPtr(new PicF(*this));
}


void PicF::writeFields(CFRecord& record)
{
#pragma message("####################### PicF record is not implemented")
	Log::error("PicF record is not implemented.");
	//record << some_value;
}


void PicF::readFields(CFRecord& record)
{
#pragma message("####################### PicF record is not implemented")
	Log::error("PicF record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

