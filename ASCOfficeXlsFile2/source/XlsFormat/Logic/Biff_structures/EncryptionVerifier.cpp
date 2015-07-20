
#include "EncryptionVerifier.h"
#include <Binary/CFRecord.h>

namespace CRYPTO
{;


XLS::BiffStructurePtr EncryptionVerifier::clone()
{
	return XLS::BiffStructurePtr(new EncryptionVerifier(*this));
}


void EncryptionVerifier::store(XLS::CFRecord& record)
{
#pragma message("####################### EncryptionVerifier record has no BiffStructure::store() implemented")
	Log::error(" Error!!! EncryptionVerifier record has no BiffStructure::store() implemented.");
	//record << something;
}


void EncryptionVerifier::load(XLS::CFRecord& record)
{
	record >> SaltSize;
	record.loadAnyData(Salt);
	record.loadAnyData(EncryptedVerifier);
	record >> VerifierHashSize;
	record.loadAnyData(EncryptedVerifierHash);
}


} // namespace CRYPTO

