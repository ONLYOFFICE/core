#include "precompiled_xls.h"
#include "Prot4RevPass.h"

namespace XLS
{;

Prot4RevPass::Prot4RevPass()
{
}


Prot4RevPass::~Prot4RevPass()
{
}


BaseObjectPtr Prot4RevPass::clone()
{
	return BaseObjectPtr(new Prot4RevPass(*this));
}


void Prot4RevPass::writeFields(CFRecord& record)
{
	unsigned __int16 protPwdRev_num = static_cast<unsigned __int16>(STR::hex_str2int(protPwdRev));
	record << protPwdRev_num;
}


void Prot4RevPass::readFields(CFRecord& record)
{
	unsigned __int16 protPwdRev_num;
	record >> protPwdRev_num;
	protPwdRev = std::wstring (STR::int2hex_wstr(protPwdRev_num, sizeof(protPwdRev_num)).c_str());
}

} // namespace XLS

