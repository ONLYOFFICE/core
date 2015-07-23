#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/CellRangeRef.h>
#include <Logic/Biff_structures/HyperlinkObject.h>

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

	static const ElementType	type = typeHLink;

//-----------------------------
	Ref8U ref8;
	ForwardOnlyParam<std::wstring > hlinkClsid;
	OSHARED::HyperlinkObject		hyperlink;


};

} // namespace XLS

