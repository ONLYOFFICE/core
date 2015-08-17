#pragma once

#include "BiffStructure.h"

namespace CRYPTO
{

class CFRecord;

class EncryptionVerifier : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(EncryptionVerifier)
public:
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeEncryptionVerifier;

	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	unsigned int SaltSize;
	struct SALT_TAG {
		unsigned int b1;
		unsigned int b2;
		unsigned int b3;
		unsigned int b4;
	} Salt;
	struct ENCRYPTED_VERIFIER_TAG {
		unsigned int b1;
		unsigned int b2;
		unsigned int b3;
		unsigned int b4;
	} EncryptedVerifier;
	unsigned int VerifierHashSize;
	struct ENCRYPTED_VERIFIER_HASH_TAG {
		unsigned int b1;
		unsigned int b2;
		unsigned int b3;
		unsigned int b4;
	} EncryptedVerifierHash;

};

} // namespace CRYPTO

