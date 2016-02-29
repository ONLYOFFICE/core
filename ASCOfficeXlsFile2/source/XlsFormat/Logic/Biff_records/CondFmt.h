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
	_UINT16		ccf;
	bool		fToughRecalc;
	_UINT16		nID;
	SqRefU		sqref;
	BIFF_BSTR	refBound;
};

typedef boost::shared_ptr<CondFmt> CondFmtPtr;

} // namespace XLS

