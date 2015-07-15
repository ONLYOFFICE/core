#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/CondFmtStructure.h>
#include <Logic/Biff_structures/FrtRefHeaderU.h>

namespace XLS
{;


// Logical representation of CondFmt12 record in BIFF8
class CondFmt12: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CondFmt12)
	BASE_OBJECT_DEFINE_CLASS_NAME(CondFmt12)
public:
	CondFmt12();
	~CondFmt12();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeCondFmt12;


	const CellRef getLocation() const;
//-----------------------------
	FrtRefHeaderU frtRefHeaderU;
	CondFmtStructure mainCF;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_COMPLEX(frtRefHeaderU)
		//BO_ATTRIB_MARKUP_COMPLEX(mainCF)
	//BO_ATTRIB_MARKUP_END

};

typedef boost::shared_ptr<CondFmt12> CondFmt12Ptr;

} // namespace XLS

