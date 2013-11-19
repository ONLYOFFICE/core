#include "stdafx.h"
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
	WORD protPwdRev_num = static_cast<WORD>(STR::hex_str2int(static_cast<wchar_t*>(static_cast<_bstr_t>(protPwdRev))));
	record << protPwdRev_num;
}


void Prot4RevPass::readFields(CFRecord& record)
{
	WORD protPwdRev_num;
	record >> protPwdRev_num;
	protPwdRev = _bstr_t(STR::int2hex_str(protPwdRev_num, sizeof(protPwdRev_num)).c_str());
}

} // namespace XLS

