#include "stdafx.h"
#include "Password.h"

namespace XLS
{;

Password::Password()
{
}


Password::~Password()
{
}


BaseObjectPtr Password::clone()
{
	return BaseObjectPtr(new Password(*this));
}


void Password::writeFields(CFRecord& record)
{
	WORD wPassword_num = static_cast<WORD>(STR::hex_str2int(static_cast<wchar_t*>(static_cast<_bstr_t>(wPassword))));
	record << wPassword_num;
}


void Password::readFields(CFRecord& record)
{
	WORD wPassword_num;
	record >> wPassword_num;
	wPassword = _bstr_t(STR::int2hex_str(wPassword_num, sizeof(wPassword_num)).c_str());
}

} // namespace XLS

