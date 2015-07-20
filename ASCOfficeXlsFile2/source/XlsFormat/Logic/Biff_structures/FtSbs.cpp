
#include "FtSbs.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr FtSbs::clone()
{
	return BiffStructurePtr(new FtSbs(*this));
}


//void FtSbs::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	own_tag->setAttribute(L"iVal", iVal);
//	own_tag->setAttribute(L"iMin", iMin);
//	own_tag->setAttribute(L"iMax", iMax);
//	own_tag->setAttribute(L"dInc", dInc);
//	own_tag->setAttribute(L"dPage", dPage);
//	own_tag->setAttribute(L"fHoriz", fHoriz);
//	own_tag->setAttribute(L"dxScroll", dxScroll);
//
//	own_tag->setAttribute(L"fDraw", fDraw);
//	own_tag->setAttribute(L"fDrawSliderOnly", fDrawSliderOnly);
//	own_tag->setAttribute(L"fTrackElevator", fTrackElevator);
//	own_tag->setAttribute(L"fNo3d", fNo3d);
//}
//
//
//void FtSbs::getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	iVal = getStructAttribute(own_tag, L"iVal");
//	iMin = getStructAttribute(own_tag, L"iMin");
//	iMax = getStructAttribute(own_tag, L"iMax");
//	dInc = getStructAttribute(own_tag, L"dInc");
//	dPage = getStructAttribute(own_tag, L"dPage");
//	fHoriz = static_cast<bool>(getStructAttribute(own_tag, L"fHoriz"));
//	dxScroll = getStructAttribute(own_tag, L"dxScroll");
//
//	fDraw = getStructAttribute(own_tag, L"fDraw");
//	fDrawSliderOnly = getStructAttribute(own_tag, L"fDrawSliderOnly");
//	fTrackElevator = getStructAttribute(own_tag, L"fTrackElevator");
//	fNo3d = getStructAttribute(own_tag, L"fNo3d");
//}
//
//
void FtSbs::store(CFRecord& record)
{
	unsigned short ft = 0x0C; // reserved
	unsigned short cb = 0x14; // reserved
	record << ft << cb;

	record.reserveNunBytes(4); // unused1

	unsigned short flags = 0;
	SETBIT(flags, 0, fDraw);
	SETBIT(flags, 1, fDrawSliderOnly);
	SETBIT(flags, 2, fTrackElevator);
	SETBIT(flags, 3, fNo3d);

	record << iVal << iMin << iMax << dInc << dPage << fHoriz << dxScroll << flags;
}


void FtSbs::load(CFRecord& record)
{
	//record.skipNunBytes(4); // reserved

	record >> ft;
	record >> cb;

	record.skipNunBytes(4); // unused1

	unsigned short flags;

	record >> iVal >> iMin >> iMax >> dInc >> dPage >> fHoriz >> dxScroll >> flags;

	fDraw = GETBIT(flags, 0);
	fDrawSliderOnly = GETBIT(flags, 1);
	fTrackElevator = GETBIT(flags, 2);
	fNo3d = GETBIT(flags, 3);
}


} // namespace XLS

