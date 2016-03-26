#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/ChartNumNillable.h>

namespace XLS
{


// Logical representation of SerAuxTrend record in BIFF8
class SerAuxTrend: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SerAuxTrend)
	BASE_OBJECT_DEFINE_CLASS_NAME(SerAuxTrend)
public:
	SerAuxTrend();
	~SerAuxTrend();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSerAuxTrend;

//-----------------------------
	unsigned char			regt;
	unsigned char			ordUser;
	ChartNumNillable		numIntercept;
	unsigned char			fEquation;
	unsigned char			fRSquared;
	BIFF_DOUBLE				numForecast;
	BIFF_DOUBLE				numBackcast;

};

} // namespace XLS

