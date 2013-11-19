#include "stdafx.h"
#include "FtPioGrbit.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr FtPioGrbit::clone()
{
	return BiffStructurePtr(new FtPioGrbit(*this));
}


void FtPioGrbit::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
{
	own_tag->setAttribute(L"fAutoPict", fAutoPict);
	own_tag->setAttribute(L"fDde", fDde);
	own_tag->setAttribute(L"fPrintCalc", fPrintCalc);
	own_tag->setAttribute(L"fIcon", fIcon);
	own_tag->setAttribute(L"fCtl", fCtl);
	own_tag->setAttribute(L"fPrstm", fPrstm);
	own_tag->setAttribute(L"fCamera", fCamera);
	own_tag->setAttribute(L"fDefaultSize", fDefaultSize);
	own_tag->setAttribute(L"fAutoLoad", fAutoLoad);
}


void FtPioGrbit::getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
{
	fAutoPict = getStructAttribute(own_tag, L"fAutoPict");
	fDde = getStructAttribute(own_tag, L"fDde");
	fPrintCalc = getStructAttribute(own_tag, L"fPrintCalc");
	fIcon = getStructAttribute(own_tag, L"fIcon");
	fCtl = getStructAttribute(own_tag, L"fCtl");
	fPrstm = getStructAttribute(own_tag, L"fPrstm");
	fCamera = getStructAttribute(own_tag, L"fCamera");
	fDefaultSize = getStructAttribute(own_tag, L"fDefaultSize");
	fAutoLoad = getStructAttribute(own_tag, L"fAutoLoad");
}


void FtPioGrbit::store(CFRecord& record)
{
	WORD ft = 0x08; // reserved
	WORD cb = 0x02; // reserved
	record << ft << cb;

	WORD flags = 0;
	SETBIT(flags, 0, fAutoPict);
	SETBIT(flags, 1, fDde);
	SETBIT(flags, 2, fPrintCalc);
	SETBIT(flags, 3, fIcon);
	SETBIT(flags, 4, fCtl);
	SETBIT(flags, 5, fPrstm);
	SETBIT(flags, 7, fCamera);
	SETBIT(flags, 8, fDefaultSize);
	SETBIT(flags, 9, fAutoLoad);

	record << flags;
}


void FtPioGrbit::load(CFRecord& record)
{
	//record.skipNBytes(4); // reserved
	WORD ft, cb;
	record >> ft;
	record >> cb;

	if (record.getDataSize() == record.getRdPtr())
	{
		record.RollRdPtrBack(4);
		return;
	}

	WORD flags;
	record >> flags;
	fAutoPict = GETBIT(flags, 0);
	fDde = GETBIT(flags, 1);
	fPrintCalc = GETBIT(flags, 2);
	fIcon = GETBIT(flags, 3);
	fCtl = GETBIT(flags, 4);
	fPrstm = GETBIT(flags, 5);
	fCamera = GETBIT(flags, 7);
	fDefaultSize = GETBIT(flags, 8);
	fAutoLoad = GETBIT(flags, 9);
}


} // namespace XLS

