#pragma once

#include "BiffStructure.h"
#include "EncryptionHeaderFlags.h"
#include "EncryptionHeader.h"
#include "EncryptionVerifier.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace CRYPTO
{;

class CFRecord;

class RC4CryptoAPIEncryptionHeader : public XLS::BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(RC4CryptoAPIEncryptionHeader)
public:
	XLS::BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	Version EncryptionVersionInfo;
	EncryptionHeaderFlags Flags;
	unsigned int EncryptionHeaderSize;
	EncryptionHeader encryptionHeader;
	EncryptionVerifier encryptionVerifier;
};

} // namespace CRYPTO

