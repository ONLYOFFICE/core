
#include "CalcMode.h"
//#include <Exception/AttributeDataWrong.h>

namespace XLS
{;

CalcMode::CalcMode()
{
}


CalcMode::~CalcMode()
{
}


BaseObjectPtr CalcMode::clone()
{
	return BaseObjectPtr(new CalcMode(*this));
}


void CalcMode::writeFields(CFRecord& record)
{
	short fAutoRecalc_num;
	if(fAutoRecalc == std::wstring (L"manual"))
	{
		fAutoRecalc_num = 0;
	}
	else if(fAutoRecalc == std::wstring (L"auto"))
	{
		fAutoRecalc_num = 1;
	}
	else if( fAutoRecalc == std::wstring (L"autoNoTable"))
	{
		fAutoRecalc_num = 2;
	}
	else 
	{
		throw;// EXCEPT::LE::AttributeDataWrong(L"fAutoRecalc", record.getTypeString().c_str(), fAutoRecalc);
	}
	record << fAutoRecalc_num;
}


void CalcMode::readFields(CFRecord& record)
{
	short fAutoRecalc_num;
	record >> fAutoRecalc_num;
	switch (fAutoRecalc_num)
	{
		case 0:
			fAutoRecalc = std::wstring (L"manual");
			break;
		case 1:
			fAutoRecalc = std::wstring (L"auto");
			break;
		case 2:
			fAutoRecalc = std::wstring (L"autoNoTable");
			break;
		default:
			throw;// EXCEPT::RT::WrongBiffRecord("Unsupported value of fAutoRecalc.", record.getTypeString());
	}
}

} // namespace XLS

