
#include "FilePass.h"
//#include <Exception/FileIsEncrypted.h>
#include <Crypt/Decryptor.h>

namespace XLS
{


FilePass::FilePass()
{
}


FilePass::~FilePass()
{
}


BaseObjectPtr FilePass::clone()
{
	return BaseObjectPtr(new FilePass(*this));
}


void FilePass::writeFields(CFRecord& record)
{
#pragma message("####################### FilePass record is not implemented")
	Log::error("FilePass record is not implemented.");
	//record << some_value;
}


void FilePass::readFields(CFRecord& record)
{
	record >> wEncryptionType;
	if(!wEncryptionType)
	{
		record >> key;
		Log::info("FilePass: Encryption type: XOR");
		throw;// EXCEPT::STOP::FileIsEncrypted();
	}
	else
	{
		majorVer = *record.getCurData<unsigned short>();
		if(0x0001 == majorVer) // RC4 encryption header structure
		{
			rc4HeaderPtr = CRYPTO::RC4EncryptionHeaderPtr(new CRYPTO::RC4EncryptionHeader());

			rc4HeaderPtr->load (record);

			record.getGlobalWorkbookInfo()->decryptor = 
				CRYPT::DecryptorPtr(new CRYPT::Decryptor(rc4HeaderPtr, record.getGlobalWorkbookInfo()->password));
			
			Log::info("Encryption type: RC4 Standard");
		}
		else // RC4 CryptoAPI encryption header structuren
		{
			record >> rc4CryptoAPIHeader;
			Log::info("FilePass: Encryption type: RC4 Non-Standard");
			throw;// EXCEPT::STOP::FileIsEncrypted();
		}
	}

}

} // namespace XLS

