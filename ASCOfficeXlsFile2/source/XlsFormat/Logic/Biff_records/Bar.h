#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Bar record in BIFF8
class Bar: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Bar)
	BASE_OBJECT_DEFINE_CLASS_NAME(Bar)
public:
	Bar();
	~Bar();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeBar;

//-----------------------------
	BIFF_SHORT pcOverlap;
	BIFF_WORD pcGap;

	BIFF_BOOL fTranspose;
	BIFF_BOOL fStacked;
	BIFF_BOOL f100;
	BIFF_BOOL fHasShadow;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(pcOverlap)
		//BO_ATTRIB_MARKUP_ATTRIB(pcGap)

		//BO_ATTRIB_MARKUP_ATTRIB(fTranspose)
		//BO_ATTRIB_MARKUP_ATTRIB(fStacked)
		//BO_ATTRIB_MARKUP_ATTRIB(f100)
		//BO_ATTRIB_MARKUP_ATTRIB(fHasShadow)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

