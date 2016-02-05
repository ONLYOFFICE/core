
#include "SheetExt.h"
#include <Logic/Biff_structures/FrtHeader.h>

namespace XLS
{

SheetExt::SheetExt()
{
}


SheetExt::~SheetExt()
{
}


BaseObjectPtr SheetExt::clone()
{
	return BaseObjectPtr(new SheetExt(*this));
}


void SheetExt::writeFields(CFRecord& record)
{
// 	FrtHeader frtHeader(rt_SheetExt);
// 	record << frtHeader;

#pragma message("####################### SheetExt record is not implemented")
	Log::error("SheetExt record is not implemented.");
	//record << some_value;
}


void SheetExt::readFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_SheetExt);
	record >> frtHeader;

	record >> cb;
	
	_UINT32 flags;
	record >> flags;
	
	icvPlain = static_cast<unsigned char>(GETBITS(flags, 0, 6));

	if(0x00000028 == cb)
	{
		record >> sheetExtOptional;
	}
}

int SheetExt::serialize(std::wostream & stream)
{
	if (!sheetExtOptional.bEnabled) return 0;
	
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"sheetPr")
		{	
			if (!sheetExtOptional.fCondFmtCalc)	
				CP_XML_ATTR(L"enableFormatConditionsCalculation", false);
			if (!sheetExtOptional.fNotPublished)	
				CP_XML_ATTR(L"published" ,false);

			if (sheetExtOptional.color.xclrType.type == XColorType::XCLRRGB)
			{
				CP_XML_NODE(L"tabColor")
				{
					CP_XML_ATTR(L"rgb", sheetExtOptional.color.rgb.strARGB);
				}
			}

		}
	}
	return 0;
}

} // namespace XLS

