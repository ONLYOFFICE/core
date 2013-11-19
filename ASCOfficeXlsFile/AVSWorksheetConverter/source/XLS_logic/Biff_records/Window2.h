#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/CellRef.h>

namespace XLS
{;


// Logical representation of Window2 record in BIFF8
class Window2: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Window2)
	BASE_OBJECT_DEFINE_CLASS_NAME(Window2)
public:
	Window2();
	~Window2();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_BOOL fDspFmlaRt;
	BIFF_BOOL fDspGridRt;
	BIFF_BOOL fDspRwColRt;
	BIFF_BOOL fFrozenRt;
	BIFF_BOOL fDspZerosRt;
	BIFF_BOOL fDefaultHdr;
	BIFF_BOOL fRightToLeft;
	BIFF_BOOL fDspGuts;
	BIFF_BOOL fFrozenNoSplit;
	BIFF_BOOL fSelected;
	BIFF_BOOL fPaged;
	BIFF_BOOL fSLV;

	bool is_contained_in_chart_substream;

	R_RwU rwTop;
	ColU colLeft;
	Icv icvHdr;
	BIFF_WORD wScaleSLV;
	BIFF_WORD wScaleNormal;
	BIFF_BSTR topLeftCell;


public:
	BO_ATTRIB_MARKUP_BEGIN
		is_contained_in_chart_substream = L"ChartSheetSubstream" == proc.getParent()->GetparentNode()->GetnodeName();
		BO_ATTRIB_MARKUP_ATTRIB(fSelected)
		if(!is_contained_in_chart_substream)
		{
			BO_ATTRIB_MARKUP_ATTRIB(fDspFmlaRt)
			BO_ATTRIB_MARKUP_ATTRIB(fDspGridRt)
			BO_ATTRIB_MARKUP_ATTRIB(fDspRwColRt)
			BO_ATTRIB_MARKUP_ATTRIB(fFrozenRt)
			BO_ATTRIB_MARKUP_ATTRIB(fDspZerosRt)
			BO_ATTRIB_MARKUP_ATTRIB(fDefaultHdr)
			BO_ATTRIB_MARKUP_ATTRIB(fRightToLeft)
			BO_ATTRIB_MARKUP_ATTRIB(fDspGuts)
			BO_ATTRIB_MARKUP_ATTRIB(fFrozenNoSplit)
			BO_ATTRIB_MARKUP_ATTRIB(fPaged)
			BO_ATTRIB_MARKUP_ATTRIB(fSLV)

// 			BO_ATTRIB_MARKUP_ATTRIB_NAME(rwTop.rw, "rwTop")
// 			BO_ATTRIB_MARKUP_ATTRIB_NAME(colLeft.col, "colLeft")
			BO_ATTRIB_MARKUP_ATTRIB(topLeftCell)
			BO_ATTRIB_MARKUP_ATTRIB(icvHdr)
			BO_ATTRIB_MARKUP_ATTRIB(wScaleSLV)
			BO_ATTRIB_MARKUP_ATTRIB(wScaleNormal)
		}

	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

