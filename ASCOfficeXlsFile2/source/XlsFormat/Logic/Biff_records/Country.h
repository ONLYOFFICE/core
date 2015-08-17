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

//-----------------------------
	BIFF_WORD iCountryDef;
	BIFF_WORD iCountryWinIni;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(iCountryDef)
		//BO_ATTRIB_MARKUP_ATTRIB(iCountryWinIni)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

