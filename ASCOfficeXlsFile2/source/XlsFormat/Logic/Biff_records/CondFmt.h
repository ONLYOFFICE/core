#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/SqRefU.h>

namespace XLS
{


// Logical representation of CondFmt record in BIFF8
class CondFmt: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CondFmt)
	BASE_OBJECT_DEFINE_CLASS_NAME(CondFmt)
public:
	CondFmt();
	~CondFmt();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeCondFmt;

	const CellRef getLocation() const;

//-----------------------------
	BIFF_WORD	ccf;
	bool		fToughRecalc;
	BIFF_WORD	nID;
	SqRefU		sqref;
	BIFF_BSTR	refBound;
};

typedef boost::shared_ptr<CondFmt> CondFmtPtr;

} // namespace XLS

