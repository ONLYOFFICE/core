
#include "CalcRefMode.h"
//#include <Exception/AttributeDataWrong.h>

namespace XLS
{;

CalcRefMode::CalcRefMode()
{
}


CalcRefMode::~CalcRefMode()
{
}


BaseObjectPtr CalcRefMode::clone()
{
	return BaseObjectPtr(new CalcRefMode(*this));
}


void CalcRefMode::writeFields(CFRecord& record)
{
	Boolean<unsigned short> fRef;
	if(fRefA1 == std::wstring (L"A1"))
	{
		fRef = true;
	}
	else if(fRefA1 == std::wstring (L"R1C1"))
	{
		fRef = false;
	}
	else
	{
		throw;// EXCEPT::LE::AttributeDataWrong(L"fRefA1", record.getTypeString().c_str(), fRefA1);
	}
	record << fRef;
}


void CalcRefMode::readFields(CFRecord& record)
{
	Boolean<unsigned short> fRef;
	record >> fRef;
	if(fRef)
	{
		fRefA1 = std::wstring (L"A1");
	}
	else
	{
		fRefA1 = std::wstring (L"R1C1");
	}
}

} // namespace XLS

