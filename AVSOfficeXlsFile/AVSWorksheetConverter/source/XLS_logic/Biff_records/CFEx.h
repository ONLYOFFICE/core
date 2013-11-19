#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/FrtRefHeaderU.h>
#include <XLS_logic/Biff_structures/CFExNonCF12.h>

namespace XLS
{;


// Logical representation of CFEx record in BIFF8
class CFEx: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CFEx)
	BASE_OBJECT_DEFINE_CLASS_NAME(CFEx)
public:
	CFEx();
	~CFEx();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	const WORD getID() const;
private:
	FrtRefHeaderU frtRefHeaderU;
	Boolean<DWORD> fIsCF12;
	BIFF_WORD nID;
	CFExNonCF12 rgbContent;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(frtRefHeaderU)
		BO_ATTRIB_MARKUP_ATTRIB(fIsCF12)
		BO_ATTRIB_MARKUP_ATTRIB(nID)
		if(!fIsCF12)
		{
			BO_ATTRIB_MARKUP_COMPLEX(rgbContent)
		}
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

