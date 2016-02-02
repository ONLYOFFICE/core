#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Dat record in BIFF8
class Dat: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Dat)
	BASE_OBJECT_DEFINE_CLASS_NAME(Dat)
public:
	Dat();
	~Dat();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeDat;

//-----------------------------
	bool fHasBordHorz;
	bool fHasBordVert;
	bool fHasBordOutline;
	bool fShowSeriesKey;
};

} // namespace XLS

