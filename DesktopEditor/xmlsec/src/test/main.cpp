#include "../../../common/File.h"
#include "../include/CertificateCommon.h"
#include "../include/OOXMLSigner.h"
#include "../include/OOXMLVerifier.h"

#ifdef _WIN32
//#define USE_MS_CRYPTO
#endif

#define USE_SIGN
//#define USE_VERIFY

int main()
{
	std::wstring sTestDir = NSFile::GetProcessDirectory() + L"/../../";

	//ICertificate* pCertificate = NSCertificate::FromFiles(sTestDir + L"keys/key.key", "", sTestDir + L"keys/cert.crt", "");

	std::map<std::wstring, std::wstring> properties;
	properties.insert(std::make_pair(L"email", L"sign@onlyoffice.com"));
	properties.insert(std::make_pair(L"phone", L"+00000000000"));
	std::wstring sNameTest = L"NameTest";
	std::wstring sValueTest = L"ValueTest";
	properties.insert(std::make_pair(sNameTest, sValueTest));

	//ICertificate* pCertificate = NSCertificate::GenerateByAlg("rsa2048", properties);
	ICertificate* pCertificate = NSCertificate::GenerateByAlg("ed25519", properties);

	unsigned char* pSignData = NULL;
	unsigned int nSignDataLen = 0;
	std::string sSignData = "Hello world!";
	//bool bRes = pCertificate->SignPKCS7((unsigned char*)sSignData.c_str(), (unsigned int)sSignData.length(), pSignData, nSignDataLen);
	bool bRes = pCertificate->Sign((unsigned char*)sSignData.c_str(), (unsigned int)sSignData.length(), pSignData, nSignDataLen);

	RELEASEARRAYOBJECTS(pSignData);

	BYTE* pDataDst = NULL;
	unsigned long nLenDst = 0;

#ifdef USE_SIGN
#if 0
	COOXMLSigner oSigner(sTestDir + L"file", pCertificate);
	oSigner.Sign(pDataDst, nLenDst);
#else
	BYTE* pDataSrc = NULL;
	unsigned long nLenSrc = 0;
	NSFile::CFileBinary::ReadAllBytes(sTestDir + L"/file.docx", &pDataSrc, nLenSrc);

	COOXMLSigner oSigner(pDataSrc, nLenSrc, pCertificate);
	oSigner.Sign(pDataDst, nLenDst);
	RELEASEARRAYOBJECTS(pDataSrc);

	NSFile::CFileBinary oFileDst;
	oFileDst.CreateFileW(sTestDir + L"/file2.docx");
	oFileDst.WriteFile(pDataDst, nLenDst);
	oFileDst.CloseFile();
#endif
#endif

	RELEASEARRAYOBJECTS(pDataDst);

#ifdef USE_VERIFY
#if 1
	COOXMLVerifier oVerifier(sTestDir + L"file");
	int nCount = oVerifier.GetSignatureCount();
	for (int i = 0; i < nCount; i++)
	{
		COOXMLSignature* pSign = oVerifier.GetSignature(i);
		pSign->Check();
	}
#endif
#endif

	delete pCertificate;
	return 0;
}
