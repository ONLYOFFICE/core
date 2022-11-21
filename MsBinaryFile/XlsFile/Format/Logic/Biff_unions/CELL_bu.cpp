/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "CELL.h"
#include "FORMULA.h"

#include "../Biff_records/Blank.h"
#include "../Biff_records/MulBlank.h"
#include "../Biff_records/MulRk.h"
#include "../Biff_records/RK.h"
#include "../Biff_records/BoolErr.h"
#include "../Biff_records/Number.h"
#include "../Biff_records/LabelSst.h"
#include "../Biff_records/Label.h" // глючный файл SchetPrintForm.xls

namespace XLS
{
CELL::CELL(std::vector<CellRangeRef>& shared_formulas_locations_ref) : shared_formulas_locations_ref_(shared_formulas_locations_ref)
{
	ColumnNumber	= -1;
	RowNumber		= -1;
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
	
	Blank		blank;
	MulBlank	mulblank;
	RK			rk;
	MulRk		mulrk;
	BoolErr		boolerr;
	Number		number;
	LabelSst	labelsst;
	Label		label;

	Label_BIFF2		label_BIFF2;
	Number_BIFF2	number_BIFF2;
	Blank_BIFF2		blank_BIFF2;
	Integer_BIFF2	integer_BIFF2;

	if(proc.optional(formula_union))
	{
		RowNumber		= formula_union.getLocation().getRow();
		ColumnNumber	= formula_union.getLocation().getColumn();
	}
	else if(proc.optional(blank))
	{
		RowNumber		= blank.getLocation().getRow();
		ColumnNumber	= blank.getLocation().getColumn();
	}
	else if(proc.optional(mulblank))
	{
		RowNumber		= mulblank.GetRow();
		ColumnNumber	= mulblank.GetColumn();
	}
	else if(proc.optional(rk))
	{
		RowNumber		= rk.getLocation().getRow();
		ColumnNumber	= rk.getLocation().getColumn();
	}
	else if(proc.optional(mulrk))
	{
		RowNumber		= mulrk.GetRow();
		ColumnNumber	= mulrk.GetColumn();
	}
	else if(proc.optional(boolerr))
	{
		RowNumber		= boolerr.getLocation().getRow();
		ColumnNumber	= boolerr.getLocation().getColumn();
	}
	else if(proc.optional(number))
	{
		RowNumber		= number.getLocation().getRow();
		ColumnNumber	= number.getLocation().getColumn();
	}
	else if(proc.optional(label))//
	{
		RowNumber		= label.cell.getLocation().getRow();
		ColumnNumber	= label.cell.getLocation().getColumn();
	}
	else if(proc.optional(labelsst))
	{
		RowNumber		= labelsst.getLocation().getRow();
		ColumnNumber	= labelsst.getLocation().getColumn();
	}
//----------------------------------------------------------------------------
	else if (proc.optional(label_BIFF2))
	{
		RowNumber = label_BIFF2.cell.getLocation().getRow();
		ColumnNumber = label_BIFF2.cell.getLocation().getColumn();
	}
	else if (proc.optional(number_BIFF2))
	{
		RowNumber = number_BIFF2.cell.getLocation().getRow();
		ColumnNumber = number_BIFF2.cell.getLocation().getColumn();
	}
	else if (proc.optional(blank_BIFF2))
	{
		RowNumber = blank_BIFF2.cell.getLocation().getRow();
		ColumnNumber = blank_BIFF2.cell.getLocation().getColumn();
	}
	else if (proc.optional(integer_BIFF2))
	{
		RowNumber = integer_BIFF2.cell.getLocation().getRow();
		ColumnNumber = integer_BIFF2.cell.getLocation().getColumn();
	}
	else
	{
		return false;
	}
	return true;
}

int CELL::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		for (std::list<XLS::BaseObjectPtr>::iterator it = elements_.begin(); it != elements_.end(); ++it)
		{
			it->get()->serialize(stream);
		}
	}
	return 0;
}

} // namespace XLS

