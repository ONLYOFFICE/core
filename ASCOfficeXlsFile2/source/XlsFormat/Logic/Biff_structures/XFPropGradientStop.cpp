
#include "XFPropGradientStop.h"
#include <Binary/CFRecord.h>

#include <simple_xml_writer.h>

namespace XLS
{


BiffStructurePtr XFPropGradientStop::clone()
{
	return BiffStructurePtr(new XFPropGradientStop(*this));
}


void XFPropGradientStop::store(CFRecord& record)
{
#pragma message("####################### XFPropGradientStop record has no BiffStructure::store() implemented")
	Log::error(" Error!!! XFPropGradientStop record has no BiffStructure::store() implemented.");
	//record << something;
}


void XFPropGradientStop::load(CFRecord& record)
{
	record.skipNunBytes(2); // unused
	record >> numPosition >> color;
}


int XFPropGradientStop::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"stop")
		{	
			CP_XML_ATTR(L"position", numPosition);
			color.serialize(CP_XML_STREAM());
		}
	}
	return 0;
}



} // namespace XLS
