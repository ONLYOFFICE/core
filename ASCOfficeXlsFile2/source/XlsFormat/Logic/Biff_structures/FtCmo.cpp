#include "precompiled_xls.h"
#include "FtCmo.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr FtCmo::clone()
{
	return BiffStructurePtr(new FtCmo(*this));
}


//void FtCmo::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	own_tag->setAttribute(L"ot", ot);
//	own_tag->setAttribute(L"id", id);
//
//	own_tag->setAttribute(L"fLocked", fLocked);
//	own_tag->setAttribute(L"fDefaultSize", fDefaultSize);
//	own_tag->setAttribute(L"fPublished", fPublished);
//	own_tag->setAttribute(L"fPrint", fPrint);
//	own_tag->setAttribute(L"fDisabled", fDisabled);
//	own_tag->setAttribute(L"fUIObj", fUIObj);
//	own_tag->setAttribute(L"fRecalcObj", fRecalcObj);
//	own_tag->setAttribute(L"fRecalcObjAlways", fRecalcObjAlways);
//}
//
//
//void FtCmo::getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	ot = getStructAttribute(own_tag, L"ot");
//	id = getStructAttribute(own_tag, L"id");
//
//	fLocked = getStructAttribute(own_tag, L"fLocked");
//	fDefaultSize = getStructAttribute(own_tag, L"fDefaultSize");
//	fPublished = getStructAttribute(own_tag, L"fPublished");
//	fPrint = getStructAttribute(own_tag, L"fPrint");
//	fDisabled = getStructAttribute(own_tag, L"fDisabled");
//	fUIObj = getStructAttribute(own_tag, L"fUIObj");
//	fRecalcObj = getStructAttribute(own_tag, L"fRecalcObj");
//	fRecalcObjAlways = getStructAttribute(own_tag, L"fRecalcObjAlways");
//}
//

void FtCmo::store(CFRecord& record)
{
	unsigned __int16 ft = 0x15; // reserved
	unsigned __int16 cb = 0x12; // reserved
	record << ft << cb;

	unsigned __int16 flags = 0;
	SETBIT(flags, 0, fLocked);
	SETBIT(flags, 2, fDefaultSize);
	SETBIT(flags, 3, fPublished);
	SETBIT(flags, 4, fPrint);
	SETBIT(flags, 7, fDisabled);
	SETBIT(flags, 8, fUIObj);
	SETBIT(flags, 9, fRecalcObj);
	SETBIT(flags, 12, fRecalcObjAlways);

	record << ot << id << flags;
	record.reserveNunBytes(2); // unused8
}


void FtCmo::load(CFRecord& record)
{
	record.skipNunBytes(4); // reserved

	unsigned __int16 flags;
	record >> ot >> id >> flags;
	record.skipNunBytes(12); // unused

	fLocked = GETBIT(flags, 0);
	fDefaultSize = GETBIT(flags, 2);
	fPublished = GETBIT(flags, 3);
	fPrint = GETBIT(flags, 4);
	fDisabled = GETBIT(flags, 7);
	fUIObj = GETBIT(flags, 8);
	fRecalcObj = GETBIT(flags, 9);
	fRecalcObjAlways = GETBIT(flags, 12);
}


} // namespace XLS

