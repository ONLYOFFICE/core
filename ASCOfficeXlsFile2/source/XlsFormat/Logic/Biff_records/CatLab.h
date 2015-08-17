#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of CatLab record in BIFF8
class CatLab: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CatLab)
	BASE_OBJECT_DEFINE_CLASS_NAME(CatLab)
public:
	CatLab();
	~CatLab();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeCatLab;

//-----------------------------
	BIFF_WORD wOffset;
	BIFF_WORD at;
	BIFF_BOOL cAutoCatLabelReal;

	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(wOffset)
		//BO_ATTRIB_MARKUP_ATTRIB(at)
		//BO_ATTRIB_MARKUP_ATTRIB(cAutoCatLabelReal)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

