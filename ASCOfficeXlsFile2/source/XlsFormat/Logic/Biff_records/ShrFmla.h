#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/CellRangeRef.h>
#include <Logic/Biff_structures/SharedParsedFormula.h>

namespace XLS
{


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
		
	static const ElementType	type = typeShrFmla;

//-----------------------------
	RefU								ref_;
	BackwardOnlyParam<unsigned char>	cUse;
	SharedParsedFormula					formula;

};

typedef boost::shared_ptr<ShrFmla> ShrFmlaPtr;

} // namespace XLS

