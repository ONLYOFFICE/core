
#include "Tick.h"

namespace XLS
{

Tick::Tick()
{
}


Tick::~Tick()
{
}


BaseObjectPtr Tick::clone()
{
	return BaseObjectPtr(new Tick(*this));
}


void Tick::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fAutoCo);
	SETBIT(flags, 1, fAutoMode);
	SETBITS(flags, 2, 4, rot);
	SETBIT(flags, 5, fAutoRot);
	SETBITS(flags, 14, 15, iReadingOrder);

	record << tktMajor << tktMinor << tlt << wBkgMode << rgb;
	record.reserveNunBytes(16); // reserved
	record << flags << icv << trot;
}


void Tick::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> tktMajor >> tktMinor >> tlt >> wBkgMode >> rgb;
	record.skipNunBytes(16); // reserved
	record >> flags >> icv >> trot;

	fAutoCo = GETBIT(flags, 0);
	fAutoMode = GETBIT(flags, 1);
	rot = GETBITS(flags, 2, 4);
	fAutoRot = GETBIT(flags, 5);
	iReadingOrder = GETBITS(flags, 14, 15);

	_rott = rot;
}

int Tick::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:majorTickMark")
		{
			switch(tktMajor)
			{
			case 0: CP_XML_ATTR(L"val", L"none");	break;
			case 1:	CP_XML_ATTR(L"val", L"in");		break;
			case 2:	CP_XML_ATTR(L"val", L"out");	break;
			case 3:	CP_XML_ATTR(L"val", L"cross");	break;
			}
			
		}
		CP_XML_NODE(L"c:minorTickMark")
		{
			switch(tktMinor)
			{
			case 0: CP_XML_ATTR(L"val", L"none");	break;
			case 1:	CP_XML_ATTR(L"val", L"in");		break;
			case 2:	CP_XML_ATTR(L"val", L"out");	break;
			case 3:	CP_XML_ATTR(L"val", L"cross");	break;
			}
			
		}
		CP_XML_NODE(L"c:tickLblPos")
		{
			switch(tlt)
			{
			case 0: CP_XML_ATTR(L"val", L"none");	break;
			case 1:	CP_XML_ATTR(L"val", L"low");	break;
			case 2:	CP_XML_ATTR(L"val", L"high");	break;
			case 3:	CP_XML_ATTR(L"val", L"nextTo");	break;
			}
		}
	}
	return 0;
}

} // namespace XLS

