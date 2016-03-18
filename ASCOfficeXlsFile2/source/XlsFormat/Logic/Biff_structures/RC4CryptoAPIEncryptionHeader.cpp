
#include "RC4CryptoAPIEncryptionHeader.h"
#include <Binary/CFRecord.h>

namespace CRYPTO
{


XLS::BiffStructurePtr RC4CryptoAPIEncryptionHeader::clone()
{
	return XLS::BiffStructurePtr(new RC4CryptoAPIEncryptionHeader(*this));
}


void RC4CryptoAPIEncryptionHeader::store(XLS::CFRecord& record)
{
#pragma message("####################### RC4CryptoAPIEncryptionHeader record has no BiffStructure::store() implemented")
	Log::error(" Error!!! RC4CryptoAPIEncryptionHeader record has no BiffStructure::store() implemented.");
	//record << something;
}


void RC4CryptoAPIEncryptionHeader::load(XLS::CFRecord& record)
{
	record >> EncryptionVersionInfo >> Flags >> EncryptionHeaderSize >> encryptionHeader >> encryptionVerifier;
}


} // namespace CRYPTO

