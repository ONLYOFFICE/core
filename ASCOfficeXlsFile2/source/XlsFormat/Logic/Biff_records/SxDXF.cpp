
#include "SxDXF.h"

namespace XLS
{

SxDXF::SxDXF()
{
}


SxDXF::~SxDXF()
{
}


BaseObjectPtr SxDXF::clone()
{
	return BaseObjectPtr(new SxDXF(*this));
}


void SxDXF::writeFields(CFRecord& record)
{
#pragma message("####################### SxDXF record is not implemented")
	Log::error("SxDXF record is not implemented.");
	//record << some_value;
}


void SxDXF::readFields(CFRecord& record)
{
#pragma message("####################### SxDXF record is not implemented")
	Log::error("SxDXF record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

