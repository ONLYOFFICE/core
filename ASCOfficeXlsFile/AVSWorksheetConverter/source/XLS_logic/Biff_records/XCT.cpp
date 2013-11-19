#include "stdafx.h"
#include "XCT.h"

namespace XLS
{;

XCT::XCT()
{
}


XCT::~XCT()
{
}


BaseObjectPtr XCT::clone()
{
	return BaseObjectPtr(new XCT(*this));
}


void XCT::writeFields(CFRecord& record)
{
#pragma message("####################### XCT record is not implemented")
	Log::error("XCT record is not implemented.");
	//record << some_value;
}


void XCT::readFields(CFRecord& record)
{
	record >> ccrn;
	itab_exist = false;
	if(!record.isEOF())
	{
		itab_exist = true;
		record >> itab;
	}
}

} // namespace XLS

