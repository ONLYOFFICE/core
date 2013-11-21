#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/CellRangeRef.h>
#include <XLS_logic/Biff_structures/HyperlinkObject.h>

namespace XLS
{;


// Logical representation of HLink record in BIFF8
class HLink: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(HLink)
	BASE_OBJECT_DEFINE_CLASS_NAME(HLink)
public:
	HLink();
	~HLink();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	Ref8U ref8;
	ForwardOnlyParam<_bstr_t> hlinkClsid;
	OSHARED::HyperlinkObject hyperlink;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(ref8)
		BO_ATTRIB_MARKUP_ATTRIB(hlinkClsid)
		BO_ATTRIB_MARKUP_COMPLEX(hyperlink)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

