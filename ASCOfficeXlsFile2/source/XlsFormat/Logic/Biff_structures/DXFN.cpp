
#include "DXFN.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr DXFN::clone()
{
	return BiffStructurePtr(new DXFN(*this));
}


//void DXFN::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"alchNinch", alchNinch);
//	xml_tag->setAttribute(L"alcvNinch", alcvNinch);
//	xml_tag->setAttribute(L"wrapNinch", wrapNinch);
//	xml_tag->setAttribute(L"trotNinch", trotNinch);
//	xml_tag->setAttribute(L"kintoNinch", kintoNinch);
//	xml_tag->setAttribute(L"cIndentNinch", cIndentNinch);
//	xml_tag->setAttribute(L"fShrinkNinch", fShrinkNinch);
//	xml_tag->setAttribute(L"fMergeCellNinch", fMergeCellNinch);
//	xml_tag->setAttribute(L"lockedNinch", lockedNinch);
//	xml_tag->setAttribute(L"hiddenNinch", hiddenNinch);
//	xml_tag->setAttribute(L"glLeftNinch", glLeftNinch);
//	xml_tag->setAttribute(L"glRightNinch", glRightNinch);
//	xml_tag->setAttribute(L"glTopNinch", glTopNinch);
//	xml_tag->setAttribute(L"glBottomNinch", glBottomNinch);
//	xml_tag->setAttribute(L"glDiagDownNinch", glDiagDownNinch);
//	xml_tag->setAttribute(L"glDiagUpNinch", glDiagUpNinch);
//	xml_tag->setAttribute(L"flsNinch", flsNinch);
//	xml_tag->setAttribute(L"icvFNinch", icvFNinch);
//	xml_tag->setAttribute(L"icvBNinch", icvBNinch);
//	xml_tag->setAttribute(L"ifmtNinch", ifmtNinch);
//	xml_tag->setAttribute(L"fIfntNinch", fIfntNinch);
//	xml_tag->setAttribute(L"ibitAtrNum", ibitAtrNum);
//	xml_tag->setAttribute(L"ibitAtrFnt", ibitAtrFnt);
//	xml_tag->setAttribute(L"ibitAtrAlc", ibitAtrAlc);
//	xml_tag->setAttribute(L"ibitAtrBdr", ibitAtrBdr);
//	xml_tag->setAttribute(L"ibitAtrPat", ibitAtrPat);
//	xml_tag->setAttribute(L"ibitAtrProt", ibitAtrProt);
//	xml_tag->setAttribute(L"iReadingOrderNinch", iReadingOrderNinch);
//	xml_tag->setAttribute(L"fIfmtUser", fIfmtUser);
//	xml_tag->setAttribute(L"fNewBorder", fNewBorder);
//	xml_tag->setAttribute(L"fZeroInited", fZeroInited);
//
//	if(ibitAtrNum)
//	{
//		dxfnum.toXML(xml_tag);
//	}
//	if(ibitAtrFnt)
//	{
//		dxffntd.toXML(xml_tag);
//	}
//	if(ibitAtrAlc)
//	{
//		dxfalc.toXML(xml_tag);
//	}
//	if(ibitAtrBdr)
//	{
//		dxfbdr.toXML(xml_tag);
//	}
//	if(ibitAtrPat)
//	{
//		dxfpat.toXML(xml_tag);
//	}
//	if(ibitAtrProt)
//	{
//		dxfprot.toXML(xml_tag);
//	}
//}
//
//
//void DXFN::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	alchNinch = getStructAttribute(xml_tag, L"alchNinch");
//	alcvNinch = getStructAttribute(xml_tag, L"alcvNinch");
//	wrapNinch = getStructAttribute(xml_tag, L"wrapNinch");
//	trotNinch = getStructAttribute(xml_tag, L"trotNinch");
//	kintoNinch = getStructAttribute(xml_tag, L"kintoNinch");
//	cIndentNinch = getStructAttribute(xml_tag, L"cIndentNinch");
//	fShrinkNinch = getStructAttribute(xml_tag, L"fShrinkNinch");
//	fMergeCellNinch = getStructAttribute(xml_tag, L"fMergeCellNinch");
//	lockedNinch = getStructAttribute(xml_tag, L"lockedNinch");
//	hiddenNinch = getStructAttribute(xml_tag, L"hiddenNinch");
//	glLeftNinch = getStructAttribute(xml_tag, L"glLeftNinch");
//	glRightNinch = getStructAttribute(xml_tag, L"glRightNinch");
//	glTopNinch = getStructAttribute(xml_tag, L"glTopNinch");
//	glBottomNinch = getStructAttribute(xml_tag, L"glBottomNinch");
//	glDiagDownNinch = getStructAttribute(xml_tag, L"glDiagDownNinch");
//	glDiagUpNinch = getStructAttribute(xml_tag, L"glDiagUpNinch");
//	flsNinch = getStructAttribute(xml_tag, L"flsNinch");
//	icvFNinch = getStructAttribute(xml_tag, L"icvFNinch");
//	icvBNinch = getStructAttribute(xml_tag, L"icvBNinch");
//	ifmtNinch = getStructAttribute(xml_tag, L"ifmtNinch");
//	fIfntNinch = getStructAttribute(xml_tag, L"fIfntNinch");
//	ibitAtrNum = getStructAttribute(xml_tag, L"ibitAtrNum");
//	ibitAtrFnt = getStructAttribute(xml_tag, L"ibitAtrFnt");
//	ibitAtrAlc = getStructAttribute(xml_tag, L"ibitAtrAlc");
//	ibitAtrBdr = getStructAttribute(xml_tag, L"ibitAtrBdr");
//	ibitAtrPat = getStructAttribute(xml_tag, L"ibitAtrPat");
//	ibitAtrProt = getStructAttribute(xml_tag, L"ibitAtrProt");
//	iReadingOrderNinch = getStructAttribute(xml_tag, L"iReadingOrderNinch");
//	fIfmtUser = getStructAttribute(xml_tag, L"fIfmtUser");
//	fNewBorder = getStructAttribute(xml_tag, L"fNewBorder");
//	fZeroInited = getStructAttribute(xml_tag, L"fZeroInited");
//
//	if(ibitAtrNum)
//	{
//		dxfnum.setIsUserDefined(fIfmtUser);
//		dxfnum.fromXML(xml_tag);
//	}
//	if(ibitAtrFnt)
//	{
//		dxffntd.fromXML(xml_tag);
//	}
//	if(ibitAtrAlc)
//	{
//		dxfalc.fromXML(xml_tag);
//	}
//	if(ibitAtrBdr)
//	{
//		dxfbdr.fromXML(xml_tag);
//	}
//	if(ibitAtrPat)
//	{
//		dxfpat.fromXML(xml_tag);
//	}
//	if(ibitAtrProt)
//	{
//		dxfprot.fromXML(xml_tag);
//	}
//}
//
//
void DXFN::store(CFRecord& record)
{
	unsigned int flags = 0;
	SETBIT(flags, 0, alchNinch);
	SETBIT(flags, 1, alcvNinch);
	SETBIT(flags, 2, wrapNinch);
	SETBIT(flags, 3, trotNinch);
	SETBIT(flags, 4, kintoNinch);
	SETBIT(flags, 5, cIndentNinch);
	SETBIT(flags, 6, fShrinkNinch);
	SETBIT(flags, 7, fMergeCellNinch);
	SETBIT(flags, 8, lockedNinch);
	SETBIT(flags, 9, hiddenNinch);
	SETBIT(flags, 10, glLeftNinch);
	SETBIT(flags, 11, glRightNinch);
	SETBIT(flags, 12, glTopNinch);
	SETBIT(flags, 13, glBottomNinch);
	SETBIT(flags, 14, glDiagDownNinch);
	SETBIT(flags, 15, glDiagUpNinch);
	SETBIT(flags, 16, flsNinch);
	SETBIT(flags, 17, icvFNinch);
	SETBIT(flags, 18, icvBNinch);
	SETBIT(flags, 19, ifmtNinch);
	SETBIT(flags, 20, fIfntNinch);
	SETBIT(flags, 25, ibitAtrNum);
	SETBIT(flags, 26, ibitAtrFnt);
	SETBIT(flags, 27, ibitAtrAlc);
	SETBIT(flags, 28, ibitAtrBdr);
	SETBIT(flags, 29, ibitAtrPat);
	SETBIT(flags, 30, ibitAtrProt);
	SETBIT(flags, 31, iReadingOrderNinch);
	record << flags;
	unsigned short flags2 = 0;
	SETBIT(flags2, 0, fIfmtUser);
	SETBIT(flags2, 2, fNewBorder);
	SETBIT(flags2, 15, fZeroInited);
	record << flags2;

	if(ibitAtrNum)
	{
		record << dxfnum;
	}
	if(ibitAtrFnt)
	{
		record << dxffntd;
	}
	if(ibitAtrAlc)
	{
		record << dxfalc;
	}
	if(ibitAtrBdr)
	{
		record << dxfbdr;
	}
	if(ibitAtrPat)
	{
		record << dxfpat;
	}
	if(ibitAtrProt)
	{
		record << dxfprot;
	}
}


void DXFN::load(CFRecord& record)
{
	unsigned int flags;
	record >> flags;
	alchNinch = GETBIT(flags, 0);
	alcvNinch = GETBIT(flags, 1);
	wrapNinch = GETBIT(flags, 2);
	trotNinch = GETBIT(flags, 3);
	kintoNinch = GETBIT(flags, 4);
	cIndentNinch = GETBIT(flags, 5);
	fShrinkNinch = GETBIT(flags, 6);
	fMergeCellNinch = GETBIT(flags, 7);
	lockedNinch = GETBIT(flags, 8);
	hiddenNinch = GETBIT(flags, 9);
	glLeftNinch = GETBIT(flags, 10);
	glRightNinch = GETBIT(flags, 11);
	glTopNinch = GETBIT(flags, 12);
	glBottomNinch = GETBIT(flags, 13);
	glDiagDownNinch = GETBIT(flags, 14);
	glDiagUpNinch = GETBIT(flags, 15);
	flsNinch = GETBIT(flags, 16);
	icvFNinch = GETBIT(flags, 17);
	icvBNinch = GETBIT(flags, 18);
	ifmtNinch = GETBIT(flags, 19);
	fIfntNinch = GETBIT(flags, 20);
	ibitAtrNum = GETBIT(flags, 25);
	ibitAtrFnt = GETBIT(flags, 26);
	ibitAtrAlc = GETBIT(flags, 27);
	ibitAtrBdr = GETBIT(flags, 28);
	ibitAtrPat = GETBIT(flags, 29);
	ibitAtrProt = GETBIT(flags, 30);
	iReadingOrderNinch = GETBIT(flags, 31);
	unsigned short flags2;
	record >> flags2;
	fIfmtUser = GETBIT(flags2, 0);
	fNewBorder = GETBIT(flags2, 2);
	fZeroInited = GETBIT(flags2, 15);

	if(ibitAtrNum)
	{
		dxfnum.setIsUserDefined(fIfmtUser);
		record >> dxfnum;
	}
	if(ibitAtrFnt)
	{
		record >> dxffntd;
	}
	if(ibitAtrAlc)
	{
		record >> dxfalc;
	}
	if(ibitAtrBdr)
	{
		record >> dxfbdr;
	}
	if(ibitAtrPat)
	{
		record >> dxfpat;
	}
	if(ibitAtrProt)
	{
		record >> dxfprot;
	}
}


} // namespace XLS

