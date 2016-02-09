
#include "XFPropColor.h"
#include <Binary/CFRecord.h>

#include <simple_xml_writer.h>

namespace XLS
{


BiffStructurePtr XFPropColor::clone()
{
	return BiffStructurePtr(new XFPropColor(*this));
}


void XFPropColor::store(CFRecord& record)
{
#pragma message("####################### XFPropColor record has no BiffStructure::store() implemented")
	Log::error(" Error!!! XFPropColor record has no BiffStructure::store() implemented.");
	//record << something;
}


void XFPropColor::load(CFRecord& record)
{
	node_name = L"color";

	unsigned char flags;
	record >> flags;
	
	fValidRGBA	= GETBIT(flags, 0);
	xclrType	= GETBITS(flags, 1, 7);
	
	record >> icv >> nTintShade >> dwRgba;
}

int XFPropColor::serialize(std::wostream & stream)
{
	if (xclrType > 3 )return 0;//not set

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(node_name)
		{
			switch(xclrType)
			{
				case 0: CP_XML_ATTR(L"auto",	1);				break;
				case 1: CP_XML_ATTR(L"indexed", icv);			break;
				case 2:	CP_XML_ATTR(L"rgb",		dwRgba.strRGB);	break;
				case 3: CP_XML_ATTR(L"theme",	icv);			break;
			}
			if (nTintShade != 0)
			{
				CP_XML_ATTR(L"tint", nTintShade);
			}
		}
	}
	return 0;
}


} // namespace XLS
