//
#include "AreaFormat.h"

namespace XLS
{

AreaFormat::AreaFormat()
{
}


AreaFormat::~AreaFormat()
{
}


BaseObjectPtr AreaFormat::clone()
{
	return BaseObjectPtr(new AreaFormat(*this));
}


void AreaFormat::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fAuto);
	SETBIT(flags, 1, fInvertNeg);
	record << rgbFore << rgbBack << fls << flags << icvFore << icvBack;
}


void AreaFormat::readFields(CFRecord& record)
{
	unsigned short flags;
	
	record >> rgbFore >> rgbBack >> fls >> flags >> icvFore >> icvBack;
	
	fAuto		= GETBIT(flags, 0);
	fInvertNeg	= GETBIT(flags, 1);
}

int AreaFormat::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		if (fls == (_UINT16)0)
		{
			CP_XML_NODE(L"a:noFill");
		}
		else
		{
			CP_XML_NODE(L"a:solidFill")
			{
				CP_XML_NODE(L"a:srgbClr")
				{
					CP_XML_ATTR(L"val",  rgbFore.strRGB);		
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

