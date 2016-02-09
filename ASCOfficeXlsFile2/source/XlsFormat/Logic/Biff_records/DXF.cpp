
#include "DXF.h"

namespace XLS
{

DXF::DXF()
{
}


DXF::~DXF()
{
}


BaseObjectPtr DXF::clone()
{
	return BaseObjectPtr(new DXF(*this));
}


void DXF::writeFields(CFRecord& record)
{
#pragma message("####################### DXF record is not implemented")
	Log::error("DXF record is not implemented.");
	//record << some_value;
}


void DXF::readFields(CFRecord& record)
{
	record >> frtRefHeaderU;
	unsigned short flags;
	
	record >> flags >> xfprops;
	
	xfprops.fNewBorder = GETBIT(flags, 1);
}

int DXF::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"dxf")
		{
			xfprops.serialize(CP_XML_STREAM());
		}
	}
	return 0;
}

} // namespace XLS

