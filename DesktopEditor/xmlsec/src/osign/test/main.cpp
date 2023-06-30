#include "../lib/include/osign.h"

int main()
{
	if (false)
	{
		// test crypt/decrypt
		OSign::CStorageBuffer oPassword = OSign::Crypt::GeneratePassword(100);

		std::string sDataCrypt = "Hello, world!";
		OSign::CStorageBuffer oBuffer;
		oBuffer.Add((const unsigned char*)sDataCrypt.c_str(), sDataCrypt.length());

		OSign::CStorageBuffer oCryptBuffer = OSign::Crypt::Encrypt(oBuffer, oPassword);
		OSign::CStorageBuffer oDecryptBuffer = OSign::Crypt::Decrypt(oCryptBuffer, oPassword);

		std::string sDecryptData((char*)oDecryptBuffer.GetData(), oDecryptBuffer.GetLength());

		return 0;
	}

	if (false)
	{
		// test serialize buffer
		std::string sDataBuffer = "Hello, world!";
		OSign::CStorageBuffer oBuffer;
		oBuffer.Add((const unsigned char*)sDataBuffer.c_str(), sDataBuffer.length());

		std::string sBase64 = oBuffer.ToBase64();
		OSign::CStorageBuffer oDecodeBuffer;
		oDecodeBuffer.FromBase64(sBase64);

		std::string sDecryptData((char*)oDecodeBuffer.GetData(), oDecodeBuffer.GetLength());

		return 0;
	}

	if (true)
	{
		OSign::CCertificate* pCert = new OSign::CCertificate();
		pCert->Generate();

		std::string sDataBuffer = "Hello, world!";
		OSign::CStorageBuffer oBuffer;
		oBuffer.Add((const unsigned char*)sDataBuffer.c_str(), sDataBuffer.length());

		OSign::CStorageBuffer oBufferSign = pCert->Sign(oBuffer);
		bool bIsValid = pCert->Verify(oBuffer, oBufferSign);
		if (!bIsValid)
			return 1;

		OSign::CStorage oStorage;
		oStorage.Add(pCert);

		OSign::CStorageBuffer oStorageBuffer;
		oStorage.Save(&oStorageBuffer);

		OSign::CStorage oStorageLoad;
		oStorageLoad.Load(&oStorageBuffer);

		OSign::CCertificate* pCert2 = oStorage.Get(0);
		bool bIsValid2 = pCert2->Verify(oBuffer, oBufferSign);
		if (!bIsValid2)
			return 1;

		return 0;
	}

	return 0;
}
