#pragma once

#include "BiffStructure.h"
#include <XLS_logic/Biff_structures/BitMarkedStructs.h>

namespace CRYPTO
{;


class RC4EncryptionHeader : public XLS::BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(RC4EncryptionHeader)
public:
	XLS::BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

public:
	Version EncryptionVersionInfo;
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
	struct ENCRYPTED_VERIFIER_HASH_TAG {
		DWORD b1;
		DWORD b2;
		DWORD b3;
		DWORD b4;
	} EncryptedVerifierHash;
};

} // namespace CRYPTO

