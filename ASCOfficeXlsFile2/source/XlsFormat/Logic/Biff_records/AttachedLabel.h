#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of AttachedLabel record in BIFF8
class AttachedLabel: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(AttachedLabel)
	BASE_OBJECT_DEFINE_CLASS_NAME(AttachedLabel)
public:
	AttachedLabel();
	~AttachedLabel();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
	
	static const ElementType	type = typeAttachedLabel;

	int serialize(std::wostream & _stream);

	BIFF_BOOL fShowValue;
	BIFF_BOOL fShowPercent;
	BIFF_BOOL fShowLabelAndPerc;
	BIFF_BOOL fShowLabel;
	BIFF_BOOL fShowBubbleSizes;
	BIFF_BOOL fShowSeriesName;

//-----------------------------

	bool is_area;

};

} // namespace XLS

