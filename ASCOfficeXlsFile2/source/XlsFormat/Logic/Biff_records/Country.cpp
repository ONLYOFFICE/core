#include "precompiled_xls.h"
#include "Country.h"

namespace XLS
{;

Country::Country()
{
}


Country::~Country()
{
}


BaseObjectPtr Country::clone()
{
	return BaseObjectPtr(new Country(*this));
}


void Country::writeFields(CFRecord& record)
{
	record << iCountryDef << iCountryWinIni;
}


void Country::readFields(CFRecord& record)
{
	record >> iCountryDef >> iCountryWinIni;
}

} // namespace XLS

