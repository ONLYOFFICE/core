
#include "XORObfuscation.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr XORObfuscation::clone()
{
	return BiffStructurePtr(new XORObfuscation(*this));
}


void XORObfuscation::store(CFRecord& record)
{
	record << key << verificationBytes;
}


void XORObfuscation::load(CFRecord& record)
{
	record >> key >> verificationBytes;
}


} // namespace XLS

