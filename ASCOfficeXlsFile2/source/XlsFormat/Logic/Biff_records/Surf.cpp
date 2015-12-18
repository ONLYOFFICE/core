
#include "Surf.h"

namespace XLS
{

Surf::Surf()
{
}


Surf::~Surf()
{
}


BaseObjectPtr Surf::clone()
{
	return BaseObjectPtr(new Surf(*this));
}


void Surf::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fFillSurface);
	SETBIT(flags, 1, f3DPhongShade);
	record << flags;
}


void Surf::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	
	fFillSurface	= GETBIT(flags, 0);
	f3DPhongShade	= GETBIT(flags, 1);
}

int Surf::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:wireframe")
		{
			CP_XML_ATTR (L"val" , !fFillSurface);
		}
	}

	return 0;
}

} // namespace XLS

