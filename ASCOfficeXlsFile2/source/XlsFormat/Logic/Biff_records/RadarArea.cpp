
#include "RadarArea.h"

namespace XLS
{

RadarArea::RadarArea()
{
}


RadarArea::~RadarArea()
{
}


BaseObjectPtr RadarArea::clone()
{
	return BaseObjectPtr(new RadarArea(*this));
}


void RadarArea::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fRdrAxLab);
	SETBIT(flags, 1, fHasShadow);
	record << flags;
	record.reserveNunBytes(2); // unused
}


void RadarArea::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	
	fRdrAxLab	= GETBIT(flags, 0); //in tick
	fHasShadow	= GETBIT(flags, 1);
	
	record.skipNunBytes(2); // unused
}

int	RadarArea::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:radarStyle")
		{
			CP_XML_ATTR (L"val" , L"filled");
		}
	}

	return 0;
}

} // namespace XLS

