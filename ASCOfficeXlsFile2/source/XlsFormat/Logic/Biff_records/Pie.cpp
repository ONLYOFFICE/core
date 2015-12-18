
#include "Pie.h"

namespace XLS
{

Pie::Pie()
{
}


Pie::~Pie()
{
}


BaseObjectPtr Pie::clone()
{
	return BaseObjectPtr(new Pie(*this));
}


void Pie::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fHasShadow);
	SETBIT(flags, 1, fShowLdrLines);
	
	record >> anStart >> pcDonut >> flags;
}


void Pie::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> anStart >> pcDonut >> flags;
	
	fHasShadow = GETBIT(flags, 0);
	fShowLdrLines = GETBIT(flags, 1);
}

int	Pie::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		if (anStart > 0)
		{
			CP_XML_NODE(L"c:firstSliceAng")
			{
				CP_XML_ATTR (L"val" , anStart);
			}
		}
		if (pcDonut > 0 && pcDonut != 10)
		{
			CP_XML_NODE(L"c:holeSize")
			{
				CP_XML_ATTR (L"val" , pcDonut);
			}
		}

	}
	return 0;
}

} // namespace XLS

