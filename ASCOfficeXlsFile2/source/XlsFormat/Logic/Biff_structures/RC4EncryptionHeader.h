#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace CRYPTO
{


class RC4EncryptionHeader : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(RC4EncryptionHeader)
public:
	XLS::BiffStructurePtr clone();

	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	static const XLS::ElementType	type = XLS::typeRC4EncryptionHeader;

	Version EncryptionVersionInfo;
	
	struct SALT_TAG 
	{
		_UINT32 b1;
		_UINT32 b2;
		_UINT32 b3;
		_UINT32 b4;
	} Salt;

	struct ENCRYPTED_VERIFIER_TAG 
	{
		_UINT32 b1;
		_UINT32 b2;
		_UINT32 b3;
		_UINT32 b4;
	} EncryptedVerifier;

	struct ENCRYPTED_VERIFIER_HASH_TAG 
	{
		_UINT32 b1;
		_UINT32 b2;
		_UINT32 b3;
		_UINT32 b4;
	} EncryptedVerifierHash;
};

	typedef boost::shared_ptr<RC4EncryptionHeader> RC4EncryptionHeaderPtr;

} // namespace CRYPTO

