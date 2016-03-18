#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/XORObfuscation.h>
#include <Logic/Biff_structures/RC4EncryptionHeader.h>
#include <Logic/Biff_structures/RC4CryptoAPIEncryptionHeader.h>

namespace XLS
{

class CFStreamCacheReader;

// Logical representation of FilePass record in BIFF8
class FilePass: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(FilePass)
	BASE_OBJECT_DEFINE_CLASS_NAME(FilePass)
public:
	FilePass();
	~FilePass();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeFilePass;

//-----------------------------
	Boolean<unsigned short> wEncryptionType;
	XORObfuscation key;
	_UINT16 majorVer;
	CRYPTO::RC4EncryptionHeader rc4Header;
	CRYPTO::RC4CryptoAPIEncryptionHeader rc4CryptoAPIHeader;

};

} // namespace XLS

