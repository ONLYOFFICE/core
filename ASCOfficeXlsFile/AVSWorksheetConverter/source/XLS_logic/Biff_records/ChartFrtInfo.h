#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of ChartFrtInfo record in BIFF8
class ChartFrtInfo: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ChartFrtInfo)
	BASE_OBJECT_DEFINE_CLASS_NAME(ChartFrtInfo)
public:
	ChartFrtInfo();
	~ChartFrtInfo();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_BYTE verOriginator;
	BIFF_BYTE verWriter;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(verOriginator)
		BO_ATTRIB_MARKUP_ATTRIB(verWriter)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

