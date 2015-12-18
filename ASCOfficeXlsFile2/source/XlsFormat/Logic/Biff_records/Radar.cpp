
#include "Radar.h"

namespace XLS
{

Radar::Radar()
{
}


Radar::~Radar()
{
}


BaseObjectPtr Radar::clone()
{
	return BaseObjectPtr(new Radar(*this));
}


void Radar::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fRdrAxLab);
	SETBIT(flags, 1, fHasShadow);
	record << flags;
	record.reserveNunBytes(2); // unused
}


void Radar::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	
	fRdrAxLab	= GETBIT(flags, 0); //in tick
	fHasShadow	= GETBIT(flags, 1);
	
	record.skipNunBytes(2); // unused
}

int	Radar::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:radarStyle")
		{
			CP_XML_ATTR (L"val" , L"standard");
		}
	}

	return 0;
}

} // namespace XLS

