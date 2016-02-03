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

	_UINT32 SaltSize;
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
	_UINT32 VerifierHashSize;
	struct ENCRYPTED_VERIFIER_HASH_TAG
	{
		_UINT32 b1;
		_UINT32 b2;
		_UINT32 b3;
		_UINT32 b4;
	} EncryptedVerifierHash;

};

} // namespace CRYPTO

