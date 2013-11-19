#include "stdafx.h"
#include "SxView.h"

namespace XLS
{;

SxView::SxView()
{
}


SxView::~SxView()
{
}


BaseObjectPtr SxView::clone()
{
	return BaseObjectPtr(new SxView(*this));
}


void SxView::writeFields(CFRecord& record)
{
#pragma message("####################### SxView record is not implemented")
	Log::error("SxView record is not implemented.");
	//record << some_value;
}


void SxView::readFields(CFRecord& record)
{
#pragma message("####################### SxView record is not implemented")
	Log::error("SxView record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

