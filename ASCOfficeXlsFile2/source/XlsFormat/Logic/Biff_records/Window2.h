#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/CellRef.h>

namespace XLS
{


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

	static const ElementType	type = typeWindow2;

//-----------------------------
	bool fDspFmlaRt;
	bool fDspGridRt;
	bool fDspRwColRt;
	bool fFrozenRt;
	bool fDspZerosRt;
	bool fDefaultHdr;
	bool fRightToLeft;
	bool fDspGuts;
	bool fFrozenNoSplit;
	bool fSelected;
	bool fPaged;
	bool fSLV;

	bool is_contained_in_chart_substream;

	R_RwU		rwTop;
	ColU		colLeft;

	Icv			icvHdr;
	
	BIFF_WORD	wScaleSLV;
	BIFF_WORD	wScaleNormal;
	BIFF_BSTR	topLeftCell;
};

} // namespace XLS

