#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Country record in BIFF8
class Country: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Country)
	BASE_OBJECT_DEFINE_CLASS_NAME(Country)
public:
	Country();
	~Country();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeCountry;

	_UINT16 iCountryDef;
	_UINT16 iCountryWinIni;

};

} // namespace XLS

