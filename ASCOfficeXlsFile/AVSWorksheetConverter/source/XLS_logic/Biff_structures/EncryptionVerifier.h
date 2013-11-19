#pragma once

#include "BiffStructure.h"

namespace CRYPTO
{;

class CFRecord;

class EncryptionVerifier : public XLS::BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(EncryptionVerifier)
public:
	XLS::BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	DWORD SaltSize;
	struct SALT_TAG {
		DWORD b1;
		DWORD b2;
		DWORD b3;
		DWORD b4;
	} Salt;
	struct ENCRYPTED_VERIFIER_TAG {
		DWORD b1;
		DWORD b2;
		DWORD b3;
		DWORD b4;
	} EncryptedVerifier;
	DWORD VerifierHashSize;
	struct ENCRYPTED_VERIFIER_HASH_TAG {
		DWORD b1;
		DWORD b2;
		DWORD b3;
		DWORD b4;
	} EncryptedVerifierHash;

};

} // namespace CRYPTO

