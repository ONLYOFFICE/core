
#include "EncryptionHeaderFlags.h"
#include <Binary/CFRecord.h>

namespace CRYPTO
{;


XLS::BiffStructurePtr EncryptionHeaderFlags::clone()
{
	return XLS::BiffStructurePtr(new EncryptionHeaderFlags(*this));
}


void EncryptionHeaderFlags::store(XLS::CFRecord& record)
{
#pragma message("####################### EncryptionHeaderFlags record has no BiffStructure::store() implemented")
	Log::error(" Error!!! EncryptionHeaderFlags record has no BiffStructure::store() implemented.");
	//record << something;
}


void EncryptionHeaderFlags::load(XLS::CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	fCryptoAPI = GETBIT(flags, 2);
	fDocProps = GETBIT(flags, 3);
	fExternal = GETBIT(flags, 4);
	fAES = GETBIT(flags, 5);
}


} // namespace CRYPTO

