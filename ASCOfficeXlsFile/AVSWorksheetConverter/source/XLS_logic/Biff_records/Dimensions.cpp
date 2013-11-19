#include "stdafx.h"
#include "Dimensions.h"
#include <XLS_logic/Biff_structures/CellRangeRef.h>

namespace XLS
{;

Dimensions::Dimensions()
{
}


Dimensions::~Dimensions()
{
}


BaseObjectPtr Dimensions::clone()
{
	return BaseObjectPtr(new Dimensions(*this));
}


void Dimensions::writeFields(CFRecord& record)
{
	if(_bstr_t("") != ref_ && _bstr_t("A1") != ref_)
	{
		CellRangeRef ref_ref(ref_);
		rwMic = ref_ref.getRowFirst();
		rwMac = ref_ref.getRowLast() + 1;// zero-based index of the row after the last row in the sheet that contains a used cell
		colMic = static_cast<WORD>(ref_ref.getColumnFirst());
		colMac = static_cast<WORD>(ref_ref.getColumnLast()) + 1; // zero-based index of the column after the last column in the sheet that contains a used cell
	}
	else
	{
		rwMic = 0;
		rwMac = 0;
		colMic = 0;
		colMac = 0;
	}
	record << rwMic << rwMac << colMic << colMac;
	record.reserveNBytes(2); // reserved
}


void Dimensions::readFields(CFRecord& record)
{
	record >> rwMic >> rwMac >> colMic >> colMac;
	record.skipNBytes(2); // reserved
	if(rwMac && colMac)
	{
		ref_ = static_cast<_bstr_t>(CellRangeRef(CellRef(rwMic, colMic, true, true), CellRef(rwMac - 1, colMac - 1, true, true)).toString(false).c_str());
	}
	else
	{
		ref_ = _bstr_t("");
	}
}

} // namespace XLS

