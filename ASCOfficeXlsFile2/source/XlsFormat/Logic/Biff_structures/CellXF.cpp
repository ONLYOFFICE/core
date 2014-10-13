#include "precompiled_xls.h"
#include "CellXF.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


CellXF::CellXF(size_t& cell_xf_current_id, size_t& style_xf_current_id)
:	cell_xf_current_id_(cell_xf_current_id), style_xf_current_id_(style_xf_current_id)
{
}


BiffStructurePtr CellXF::clone()
{
	return BiffStructurePtr(new CellXF(*this));
}


//void CellXF::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"alc", alc);
//	xml_tag->setAttribute(L"fWrap", fWrap);
//	xml_tag->setAttribute(L"alcV", alcV);
//	xml_tag->setAttribute(L"fJustLast", fJustLast);
//	xml_tag->setAttribute(L"trot", trot);
//	xml_tag->setAttribute(L"cIndent", cIndent);
//	xml_tag->setAttribute(L"fShrinkToFit", fShrinkToFit);
//	xml_tag->setAttribute(L"iReadOrder", iReadOrder);
//	xml_tag->setAttribute(L"fAtrNum", fAtrNum);
//	xml_tag->setAttribute(L"fAtrFnt", fAtrFnt);
//	xml_tag->setAttribute(L"fAtrAlc", fAtrAlc);
//	xml_tag->setAttribute(L"fAtrBdr", fAtrBdr);
//	xml_tag->setAttribute(L"fAtrPat", fAtrPat);
//	xml_tag->setAttribute(L"fAtrProt", fAtrProt);
//
//	xml_tag->setAttribute(L"dgLeft", border.dgLeft);
//	xml_tag->setAttribute(L"dgRight", border.dgRight);
//	xml_tag->setAttribute(L"dgTop", border.dgTop);
//	xml_tag->setAttribute(L"dgBottom", border.dgBottom);
//	xml_tag->setAttribute(L"icvLeft", border.icvLeft);
//	xml_tag->setAttribute(L"icvRight", border.icvRight);
//	xml_tag->setAttribute(L"grbitDiag", border.grbitDiag);
//
//	xml_tag->setAttribute(L"icvTop", border.icvTop);
//	xml_tag->setAttribute(L"icvBottom", border.icvBottom);
//	xml_tag->setAttribute(L"icvDiag", border.icvDiag);
//	xml_tag->setAttribute(L"dgDiag", border.dgDiag);
//	xml_tag->setAttribute(L"fHasXFExt", fHasXFExt);
//	xml_tag->setAttribute(L"fls", fill.fls);
//
//	xml_tag->setAttribute(L"icvFore", fill.icvFore);
//	xml_tag->setAttribute(L"icvBack", fill.icvBack);
//	xml_tag->setAttribute(L"fsxButton", fsxButton);
//
//	xml_tag->setAttribute(L"xf_current_id", cell_xf_current_id_ + style_xf_current_id_);
//	xml_tag->setAttribute(L"cell_xf_current_id", cell_xf_current_id_++);
//
//	xml_tag->setAttribute(L"border_x_id", border_x_id);
//	xml_tag->setAttribute(L"fill_x_id", fill_x_id);
//}
//
//
//void CellXF::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	alc = getStructAttribute(xml_tag, L"alc");
//	fWrap = getStructAttribute(xml_tag, L"fWrap");
//	alcV = getStructAttribute(xml_tag, L"alcV");
//	fJustLast = getStructAttribute(xml_tag, L"fJustLast");
//	trot = getStructAttribute(xml_tag, L"trot");
//	cIndent = getStructAttribute(xml_tag, L"cIndent");
//	fShrinkToFit = getStructAttribute(xml_tag, L"fShrinkToFit");
//	iReadOrder = getStructAttribute(xml_tag, L"iReadOrder");
//	fAtrNum = getStructAttribute(xml_tag, L"fAtrNum");
//	fAtrFnt = getStructAttribute(xml_tag, L"fAtrFnt");
//	fAtrAlc = getStructAttribute(xml_tag, L"fAtrAlc");
//	fAtrBdr = getStructAttribute(xml_tag, L"fAtrBdr");
//	fAtrPat = getStructAttribute(xml_tag, L"fAtrPat");
//	fAtrProt = getStructAttribute(xml_tag, L"fAtrProt");
//
//	border.dgLeft = getStructAttribute(xml_tag, L"dgLeft");
//	border.dgRight = getStructAttribute(xml_tag, L"dgRight");
//	border.dgTop = getStructAttribute(xml_tag, L"dgTop");
//	border.dgBottom = getStructAttribute(xml_tag, L"dgBottom");
//	border.icvLeft = getStructAttribute(xml_tag, L"icvLeft");
//	border.icvRight = getStructAttribute(xml_tag, L"icvRight");
//	border.grbitDiag = getStructAttribute(xml_tag, L"grbitDiag");
//
//	border.icvTop = getStructAttribute(xml_tag, L"icvTop");
//	border.icvBottom = getStructAttribute(xml_tag, L"icvBottom");
//	border.icvDiag = getStructAttribute(xml_tag, L"icvDiag");
//	border.dgDiag = getStructAttribute(xml_tag, L"dgDiag");
//	fHasXFExt = getStructAttribute(xml_tag, L"fHasXFExt");
//	fill.fls = getStructAttribute(xml_tag, L"fls");
//
//	fill.icvFore = getStructAttribute(xml_tag, L"icvFore");
//	fill.icvBack = getStructAttribute(xml_tag, L"icvBack");
//	fsxButton = getStructAttribute(xml_tag, L"fsxButton");
//}
//
//
void CellXF::store(CFRecord& record)
{
	unsigned int flags = 0;
	SETBITS(flags, 0, 2, alc);
	SETBIT(flags, 3, fWrap);
	SETBITS(flags, 4, 6, alcV);
	SETBIT(flags, 7, fJustLast);
	SETBITS(flags, 8, 15, trot);
	SETBITS(flags, 16, 19, cIndent);
	SETBIT(flags, 20, fShrinkToFit);
	SETBITS(flags, 22, 23, iReadOrder);
	SETBIT(flags, 26, fAtrNum);
	SETBIT(flags, 27, fAtrFnt);
	SETBIT(flags, 28, fAtrAlc);
	SETBIT(flags, 29, fAtrBdr);
	SETBIT(flags, 30, fAtrPat);
	SETBIT(flags, 31, fAtrProt);
	record << flags;

	flags = 0;
	SETBITS(flags, 0, 3, border.dgLeft);
	SETBITS(flags, 4, 7, border.dgRight);
	SETBITS(flags, 8, 11, border.dgTop);
	SETBITS(flags, 12, 15, border.dgBottom);
	SETBITS(flags, 16, 22, border.icvLeft);
	SETBITS(flags, 23, 29, border.icvRight);
	SETBITS(flags, 30, 31, border.grbitDiag);
	record << flags;

	flags = 0;
	SETBITS(flags, 0, 6, border.icvTop);
	SETBITS(flags, 7, 13, border.icvBottom);
	SETBITS(flags, 14, 20, border.icvDiag);
	SETBITS(flags, 21, 24, border.dgDiag);
	SETBIT(flags, 25, fHasXFExt);
	SETBITS(flags, 26, 31, fill.fls);
	record << flags;

	unsigned __int16 flags2 = 0;
	SETBITS(flags2, 0, 6, fill.icvFore);
	SETBITS(flags2, 7, 13, fill.icvBack);
	SETBIT(flags2, 14, fsxButton);
	record << flags2;
}


void CellXF::load(CFRecord& record)
{
	unsigned int flags1;
	unsigned int flags2;
	unsigned int flags3;
	unsigned __int16 flags4;
	record >> flags1 >> flags2 >> flags3 >> flags4;
	alc = static_cast<unsigned char>(GETBITS(flags1, 0, 2));
	fWrap = GETBIT(flags1, 3);
	alcV = static_cast<unsigned char>(GETBITS(flags1, 4, 6));
	fJustLast = GETBIT(flags1, 7);
	trot = static_cast<unsigned char>(GETBITS(flags1, 8, 15));
	cIndent = static_cast<unsigned char>(GETBITS(flags1, 16, 19));
	fShrinkToFit = GETBIT(flags1, 20);
	iReadOrder = static_cast<unsigned char>(GETBITS(flags1, 22, 23));
	fAtrNum = GETBIT(flags1, 26);
	fAtrFnt = GETBIT(flags1, 27);
	fAtrAlc = GETBIT(flags1, 28);
	fAtrBdr = GETBIT(flags1, 29);
	fAtrPat = GETBIT(flags1, 30);
	fAtrProt = GETBIT(flags1, 31);

	border.dgLeft = static_cast<unsigned char>(GETBITS(flags2, 0, 3));
	border.dgRight = static_cast<unsigned char>(GETBITS(flags2, 4, 7));
	border.dgTop = static_cast<unsigned char>(GETBITS(flags2, 8, 11));
	border.dgBottom = static_cast<unsigned char>(GETBITS(flags2, 12, 15));
	border.dgDiag = static_cast<unsigned char>(GETBITS(flags3, 21, 24));

	border.icvLeft = 0 != border.dgLeft ? static_cast<unsigned char>(GETBITS(flags2, 16, 22)) : 0;
	border.icvRight = 0 != border.dgRight ? static_cast<unsigned char>(GETBITS(flags2, 23, 29)) : 0;
	border.icvTop = 0 != border.dgTop ? static_cast<unsigned char>(GETBITS(flags3, 0, 6)) : 0;
	border.icvBottom = 0 != border.dgBottom ? static_cast<unsigned char>(GETBITS(flags3, 7, 13)) : 0;
	border.icvDiag = 0 != border.dgDiag ? static_cast<unsigned char>(GETBITS(flags3, 14, 20)) : 0;
	
	border.grbitDiag = static_cast<unsigned char>(GETBITS(flags2, 30, 31));

	fHasXFExt = GETBIT(flags3, 25);
	fill.fls = static_cast<unsigned char>(GETBITS(flags3, 26, 31));

	fill.icvFore = GETBITS(flags4, 0, 6);
	fill.icvBack = GETBITS(flags4, 7, 13);
	fsxButton = GETBIT(flags4, 14);

	border_x_id = record.getGlobalWorkbookInfo()->RegisterBorderId(border);
	fill_x_id = record.getGlobalWorkbookInfo()->RegisterFillId(fill);
}


} // namespace XLS

