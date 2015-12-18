//
#include "Area.h"

namespace XLS
{

Area::Area()
{
}


Area::~Area()
{
}


BaseObjectPtr Area::clone()
{
	return BaseObjectPtr(new Area(*this));
}


void Area::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fStacked);
	SETBIT(flags, 1, f100);
	SETBIT(flags, 2, fHasShadow);
	record << flags;
}


void Area::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	fStacked = GETBIT(flags, 0);
	f100 = GETBIT(flags, 1);
	fHasShadow = GETBIT(flags, 2);
}

int	Area::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:grouping")
		{
			if (f100)			CP_XML_ATTR (L"val" , L"percentStacked");			
			else if (fStacked)	CP_XML_ATTR (L"val" , L"stacked");
			else				CP_XML_ATTR (L"val" , L"standard");
		}
	}
	return 0;
}

} // namespace XLS

