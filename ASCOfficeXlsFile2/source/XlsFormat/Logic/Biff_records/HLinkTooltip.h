#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of HLinkTooltip record in BIFF8
class HLinkTooltip: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(HLinkTooltip)
	BASE_OBJECT_DEFINE_CLASS_NAME(HLinkTooltip)
public:
	HLinkTooltip();
	~HLinkTooltip();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeHLinkTooltip;

//-----------------------------
	BIFF_BSTR wzTooltip;
	BackwardOnlyParam<std::wstring > ref_;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB_NAME(ref_, L"ref")
		//BO_ATTRIB_MARKUP_ATTRIB(wzTooltip)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

