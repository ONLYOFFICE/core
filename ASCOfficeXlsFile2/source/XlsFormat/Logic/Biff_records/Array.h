#pragma once

#include "BiffRecord.h"
#include "../Biff_structures/CellRangeRef.h"
#include "../Biff_structures/ArrayParsedFormula.h"

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
	
	static const ElementType	type = typeArray;

	Ref ref_;
	BIFF_BOOL fAlwaysCalc;
	ArrayParsedFormula formula;

};

typedef boost::shared_ptr<Array> ArrayPtr;

} // namespace XLS

