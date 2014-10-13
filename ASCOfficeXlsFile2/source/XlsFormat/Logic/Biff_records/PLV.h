#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of PLV record in BIFF8
class PLV: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(PLV)
	BASE_OBJECT_DEFINE_CLASS_NAME(PLV)
public:
	PLV();
	~PLV();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD wScalePLV;
	BIFF_BOOL fPageLayoutView;
	BIFF_BOOL fRulerVisible;
	BIFF_BOOL fWhitespaceHidden;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(wScalePLV)
		BO_ATTRIB_MARKUP_ATTRIB(fPageLayoutView)
		BO_ATTRIB_MARKUP_ATTRIB(fRulerVisible)
		BO_ATTRIB_MARKUP_ATTRIB(fWhitespaceHidden)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

