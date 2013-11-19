#include "stdafx.h"
#include "CELL.h"
#include <XLS_logic/Biff_records/Blank.h>
#include <XLS_logic/Biff_unions/FORMULA.h>
#include <XLS_logic/Biff_records/MulBlank.h>
#include <XLS_logic/Biff_records/RK.h>
#include <XLS_logic/Biff_records/MulRk.h>
#include <XLS_logic/Biff_records/BoolErr.h>
#include <XLS_logic/Biff_records/Number.h>
#include <XLS_logic/Biff_records/LabelSst.h>

namespace XLS
{;


CELL::CELL(std::vector<CellRef>& shared_formulas_locations_ref) : shared_formulas_locations_ref_(shared_formulas_locations_ref)
{
}


CELL::~CELL()
{
}


BaseObjectPtr CELL::clone()
{
	return BaseObjectPtr(new CELL(*this));
}


// CELL = FORMULA / Blank / MulBlank / RK / MulRk / BoolErr / Number / LabelSst
const bool CELL::loadContent(BinProcessor& proc)
{
	FORMULA formula_union(shared_formulas_locations_ref_);
	Blank blank;
	MulBlank mulblank;
	RK rk;
	MulRk mulrk;
	BoolErr boolerr;
	Number number;
	LabelSst labelsst;
	if(proc.optional(formula_union))
	{
		RowNumber = formula_union.getLocation().getRow();
	}
	else if(proc.optional(blank))
	{
		RowNumber = blank.getLocation().getRow();
	}
	else if(proc.optional(mulblank))
	{
		RowNumber = mulblank.GetRow();
	}
	else if(proc.optional(rk))
	{
		RowNumber = rk.getLocation().getRow();
	}
	else if(proc.optional(mulrk))
	{
		RowNumber = mulrk.GetRow();
	}
	else if(proc.optional(boolerr))
	{
		RowNumber = boolerr.getLocation().getRow();
	}
	else if(proc.optional(number))
	{
		RowNumber = number.getLocation().getRow();
	}
	else if(proc.optional(labelsst))
	{
		RowNumber = labelsst.getLocation().getRow();
	}
	else
	{
		return false;
	}
	return true;
}

void CELL::toXML(MSXML2::IXMLDOMElementPtr own_tag)
{
	static _bstr_t row_name("rw");
	own_tag->setAttribute(row_name, RowNumber);
}

} // namespace XLS

