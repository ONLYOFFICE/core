#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/Cell.h>
#include <Logic/Biff_structures/CellParsedFormula.h>
#include <Logic/Biff_structures/FormulaValue.h>

namespace XLS
{

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

	static const ElementType	type = typeFormula;

	const CellRef getLocation() const;

//-----------------------------
	Cell					cell;
	FormulaValue			val;
	bool				fAlwaysCalc;
	BackwardOnlyParam<bool> fFill;
	bool				fShrFmla;
	BackwardOnlyParam<bool> fClearErrors;

	CellParsedFormula		formula;
};

typedef boost::shared_ptr<Formula> FormulaPtr;

} // namespace XLS

