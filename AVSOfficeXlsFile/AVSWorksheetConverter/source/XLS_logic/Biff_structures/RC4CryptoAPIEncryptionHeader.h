#pragma once

#include "BiffStructure.h"
#include "EncryptionHeaderFlags.h"
#include "EncryptionHeader.h"
#include "EncryptionVerifier.h"
#include <XLS_logic/Biff_structures/BitMarkedStructs.h>

namespace CRYPTO
{;

class CFRecord;

class RC4CryptoAPIEncryptionHeader : public XLS::BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(RC4CryptoAPIEncryptionHeader)
public:
	XLS::BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	Version EncryptionVersionInfo;
	EncryptionHeaderFlags Flags;
	DWORD EncryptionHeaderSize;
	EncryptionHeader encryptionHeader;
	EncryptionVerifier encryptionVerifier;
};

} // namespace CRYPTO

