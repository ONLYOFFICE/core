#include "../lib/include/osign.h"

int main()
{
	OSign::CStorageBuffer oPassword = OSign::Crypt::GeneratePassword(100);

	std::string sDataCrypt = "Hello, world!";
	OSign::CStorageBuffer oBuffer;
	oBuffer.Add((const unsigned char*)sDataCrypt.c_str(), sDataCrypt.length());

	OSign::CStorageBuffer oCryptBuffer = OSign::Crypt::Encrypt(oBuffer, oPassword);
	OSign::CStorageBuffer oDecryptBuffer = OSign::Crypt::Decrypt(oCryptBuffer, oPassword);

	std::string sDecryptData((char*)oDecryptBuffer.GetData(), oDecryptBuffer.GetLength());

	return 0;
}
