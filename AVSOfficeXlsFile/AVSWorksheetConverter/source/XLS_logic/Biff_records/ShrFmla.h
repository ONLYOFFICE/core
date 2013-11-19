#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/CellRangeRef.h>
#include <XLS_logic/Biff_structures/SharedParsedFormula.h>

namespace XLS
{;


// Logical representation of ShrFmla record in BIFF8
class ShrFmla: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ShrFmla)
	BASE_OBJECT_DEFINE_CLASS_NAME(ShrFmla)
public:
	ShrFmla(const CellRef& cell_base_ref_init);
	~ShrFmla();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	RefU ref_;
	BackwardOnlyParam<BYTE> cUse;
	SharedParsedFormula formula;

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(ref_)
		BO_ATTRIB_MARKUP_ATTRIB(cUse)
		BO_ATTRIB_MARKUP_COMPLEX(formula)
	BO_ATTRIB_MARKUP_END

};

typedef boost::shared_ptr<ShrFmla> ShrFmlaPtr;

} // namespace XLS

