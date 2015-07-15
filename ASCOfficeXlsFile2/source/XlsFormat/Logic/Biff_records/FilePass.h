#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/XORObfuscation.h>
#include <Logic/Biff_structures/RC4EncryptionHeader.h>
#include <Logic/Biff_structures/RC4CryptoAPIEncryptionHeader.h>

namespace XLS
{;

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
	Boolean<unsigned __int16> wEncryptionType;
	XORObfuscation key;
	BIFF_WORD majorVer;
	CRYPTO::RC4EncryptionHeader rc4Header;
	CRYPTO::RC4CryptoAPIEncryptionHeader rc4CryptoAPIHeader;

public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(wEncryptionType)
		//if(!wEncryptionType)
		//{
		//	//BO_ATTRIB_MARKUP_COMPLEX(key)
		//}
		//if(0x0001 == majorVer) // RC4 encryption header structure
		//{
		//	//BO_ATTRIB_MARKUP_COMPLEX(rc4Header)
		//}
		//else // RC4 CryptoAPI encryption header structure
		//{
		//	//BO_ATTRIB_MARKUP_COMPLEX(rc4CryptoAPIHeader)
		//}

	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

