#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/DXFNum.h>
#include <Logic/Biff_structures/DXFFntD.h>
#include <Logic/Biff_structures/DXFALC.h>
#include <Logic/Biff_structures/DXFBdr.h>
#include <Logic/Biff_structures/DXFPat.h>
#include <Logic/Biff_structures/DXFProt.h>

namespace XLS
{;

class CFRecord;

class DXFN : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DXFN)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	bool alchNinch;
	bool alcvNinch;
	bool wrapNinch;
	bool trotNinch;
	bool kintoNinch;
	bool cIndentNinch;
	bool fShrinkNinch;
	bool fMergeCellNinch;
	bool lockedNinch;
	bool hiddenNinch;
	bool glLeftNinch;
	bool glRightNinch;
	bool glTopNinch;
	bool glBottomNinch;
	bool glDiagDownNinch;
	bool glDiagUpNinch;
	bool flsNinch;
	bool icvFNinch;
	bool icvBNinch;
	bool ifmtNinch;
	bool fIfntNinch;
	bool ibitAtrNum;
	bool ibitAtrFnt;
	bool ibitAtrAlc;
	bool ibitAtrBdr;
	bool ibitAtrPat;
	bool ibitAtrProt;
	bool iReadingOrderNinch;
	bool fIfmtUser;
	bool fNewBorder;
	bool fZeroInited;

	DXFNum dxfnum;
	DXFFntD dxffntd;
	DXFALC dxfalc;
	DXFBdr dxfbdr;
	DXFPat dxfpat;
	DXFProt dxfprot;
};

typedef boost::shared_ptr<DXFN> DXFNPtr;

} // namespace XLS

