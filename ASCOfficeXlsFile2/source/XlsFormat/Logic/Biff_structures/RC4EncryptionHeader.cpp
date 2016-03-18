
#include "RC4EncryptionHeader.h"
#include <Binary/CFRecord.h>

namespace CRYPTO
{


XLS::BiffStructurePtr RC4EncryptionHeader::clone()
{
	return XLS::BiffStructurePtr(new RC4EncryptionHeader(*this));
}


void RC4EncryptionHeader::store(XLS::CFRecord& record)
{
#pragma message("####################### RC4EncryptionHeader record has no BiffStructure::store() implemented")
	Log::error(" Error!!! RC4EncryptionHeader record has no BiffStructure::store() implemented.");
	//record << something;
}


void RC4EncryptionHeader::load(XLS::CFRecord& record)
{
	record >> EncryptionVersionInfo;
	record.loadAnyData(Salt);
	record.loadAnyData(EncryptedVerifier);
	record.loadAnyData(EncryptedVerifierHash);
}


} // namespace CRYPTO

