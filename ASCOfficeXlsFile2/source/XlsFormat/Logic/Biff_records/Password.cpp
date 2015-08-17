
#include "Password.h"

namespace XLS
{

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
	unsigned short wPassword_num = static_cast<unsigned short>(STR::hex_str2int(wPassword));
	record << wPassword_num;
}


void Password::readFields(CFRecord& record)
{
	unsigned short wPassword_num;
	record >> wPassword_num;
	wPassword = std::wstring (STR::int2hex_wstr(wPassword_num, sizeof(wPassword_num)).c_str());
}

} // namespace XLS

