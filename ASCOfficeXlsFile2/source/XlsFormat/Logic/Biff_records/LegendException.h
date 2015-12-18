#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of LegendException record in BIFF8
class LegendException: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(LegendException)
	BASE_OBJECT_DEFINE_CLASS_NAME(LegendException)
public:
	LegendException();
	~LegendException();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	BIFF_WORD	iss;
	BIFF_BOOL	fDelete;
	BIFF_BOOL	fLabel;

	int serialize(std::wostream & _stream);


};

} // namespace XLS

