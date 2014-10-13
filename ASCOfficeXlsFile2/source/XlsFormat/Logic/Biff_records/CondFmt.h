#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/SqRefU.h>

namespace XLS
{;


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
	const CellRef getLocation() const;

private:
	BIFF_WORD ccf;
	BIFF_BOOL fToughRecalc;
	BIFF_WORD nID;
	SqRefU sqref;
	BIFF_BSTR refBound;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(ccf)
		BO_ATTRIB_MARKUP_ATTRIB(fToughRecalc)
		BO_ATTRIB_MARKUP_ATTRIB(nID)
		BO_ATTRIB_MARKUP_ATTRIB(refBound)
		BO_ATTRIB_MARKUP_COMPLEX(sqref)
	BO_ATTRIB_MARKUP_END

};

typedef boost::shared_ptr<CondFmt> CondFmtPtr;

} // namespace XLS

