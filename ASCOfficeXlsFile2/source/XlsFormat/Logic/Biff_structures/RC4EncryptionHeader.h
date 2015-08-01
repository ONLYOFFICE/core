#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace CRYPTO
{;


class RC4EncryptionHeader : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(RC4EncryptionHeader)
public:
	XLS::BiffStructurePtr clone();

	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	static const XLS::ElementType	type = XLS::typeRC4EncryptionHeader;

	Version EncryptionVersionInfo;
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
	struct ENCRYPTED_VERIFIER_HASH_TAG {
		unsigned int b1;
		unsigned int b2;
		unsigned int b3;
		unsigned int b4;
	} EncryptedVerifierHash;
};

} // namespace CRYPTO

