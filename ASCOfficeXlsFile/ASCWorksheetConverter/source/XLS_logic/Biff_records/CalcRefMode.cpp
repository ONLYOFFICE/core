#include "stdafx.h"
#include "CalcRefMode.h"
#include <Exception/AttributeDataWrong.h>

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
	Boolean<WORD> fRef;
	if(_bstr_t(L"A1") == fRefA1)
	{
		fRef = true;
	}
	else if(_bstr_t(L"R1C1") == fRefA1)
	{
		fRef = false;
	}
	else
	{
		throw EXCEPT::LE::AttributeDataWrong(L"fRefA1", record.getTypeString().c_str(), fRefA1);
	}
	record << fRef;
}


void CalcRefMode::readFields(CFRecord& record)
{
	Boolean<WORD> fRef;
	record >> fRef;
	if(fRef)
	{
		fRefA1 = _bstr_t(L"A1");
	}
	else
	{
		fRefA1 = _bstr_t(L"R1C1");
	}
}

} // namespace XLS

