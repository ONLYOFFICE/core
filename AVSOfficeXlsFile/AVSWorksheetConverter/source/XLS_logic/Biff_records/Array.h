#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/CellRangeRef.h>
#include <XLS_logic/Biff_structures/ArrayParsedFormula.h>

namespace XLS
{;


// Logical representation of Array record in BIFF8
class Array: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Array)
	BASE_OBJECT_DEFINE_CLASS_NAME(Array)
public:
	Array(const CellRef& cell_base_ref_init);
	~Array();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	Ref ref_;
	BIFF_BOOL fAlwaysCalc;
	ArrayParsedFormula formula;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(ref_)
		BO_ATTRIB_MARKUP_ATTRIB(fAlwaysCalc)
		BO_ATTRIB_MARKUP_COMPLEX(formula)
	BO_ATTRIB_MARKUP_END

};

typedef boost::shared_ptr<Array> ArrayPtr;

} // namespace XLS

