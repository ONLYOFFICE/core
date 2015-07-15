#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of ChartFormat record in BIFF8
class ChartFormat: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ChartFormat)
	BASE_OBJECT_DEFINE_CLASS_NAME(ChartFormat)
public:
	ChartFormat();
	~ChartFormat();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeChartFormat;

//-----------------------------
	BIFF_BOOL fVaried;
	BIFF_WORD icrt;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(fVaried)
		//BO_ATTRIB_MARKUP_ATTRIB(icrt)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

