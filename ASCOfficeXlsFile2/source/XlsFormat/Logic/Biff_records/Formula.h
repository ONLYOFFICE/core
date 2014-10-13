#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/Cell.h>
#include <Logic/Biff_structures/CellParsedFormula.h>
#include <Logic/Biff_structures/FormulaValue.h>

namespace XLS
{;

// Logical representation of Formula record in BIFF8
class Formula: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Formula)
	BASE_OBJECT_DEFINE_CLASS_NAME(Formula)
public:
	Formula();
	~Formula();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
	const CellRef getLocation() const;

private:
	Cell cell;
	FormulaValue val;
	BIFF_BOOL fAlwaysCalc;
	BackwardOnlyParam<bool> fFill;
	BIFF_BOOL fShrFmla;
	BackwardOnlyParam<bool> fClearErrors;

	CellParsedFormula formula;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(cell)
		BO_ATTRIB_MARKUP_ATTRIB(val)
		BO_ATTRIB_MARKUP_ATTRIB(fAlwaysCalc)
		BO_ATTRIB_MARKUP_ATTRIB(fFill)
		BO_ATTRIB_MARKUP_ATTRIB(fShrFmla)
		BO_ATTRIB_MARKUP_ATTRIB(fClearErrors)

		BO_ATTRIB_MARKUP_COMPLEX(formula)
	BO_ATTRIB_MARKUP_END

};

typedef boost::shared_ptr<Formula> FormulaPtr;

} // namespace XLS

