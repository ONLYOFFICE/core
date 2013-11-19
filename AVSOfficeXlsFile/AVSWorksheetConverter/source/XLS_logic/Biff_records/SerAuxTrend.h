#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/ChartNumNillable.h>

namespace XLS
{;


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
private:
	BIFF_BYTE regt;
	BIFF_BYTE ordUser;
	ChartNumNillable numIntercept;
	Boolean<BYTE> fEquation;
	Boolean<BYTE> fRSquared;
	BIFF_DOUBLE numForecast;
	BIFF_DOUBLE numBackcast;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(regt)
		BO_ATTRIB_MARKUP_ATTRIB(ordUser)
		BO_ATTRIB_MARKUP_ATTRIB(numIntercept)
		BO_ATTRIB_MARKUP_ATTRIB(fEquation)
		BO_ATTRIB_MARKUP_ATTRIB(fRSquared)
		BO_ATTRIB_MARKUP_ATTRIB(numForecast)
		BO_ATTRIB_MARKUP_ATTRIB(numBackcast)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

