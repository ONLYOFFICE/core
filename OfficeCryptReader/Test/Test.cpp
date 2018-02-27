// Test.cpp : Defines the entry point for the console application.
//
#include "tchar.h"
#include "../source/ECMACryptFile.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::wstring srcFileName	= L"D:\\test\\_crypted\\test-111.docx";
	std::wstring dstFileName	= srcFileName + L"-mycrypt.docx";
	std::wstring dstFileName2	= dstFileName + L".oox";
	
	//std::wstring dstFileName	= srcFileName + L".oox";
	//std::wstring dstFileName2	= dstFileName + L"-mycrypt.docx";

	//std::wstring srcFileName	= L"D:\\test\\_crypted\\test.docx";
	//std::wstring dstFileName3	= srcFileName + L"-mycrypt1.docx";

	std::wstring password = L"574446f1-6aa0-860a-0296-787a87a214bb";

	ECMACryptFile crypt_file;
	bool result, bDataIntegrity;
		
	//result = crypt_file.DecryptOfficeFile(srcFileName, dstFileName, password, bDataIntegrity);
	//result = crypt_file.EncryptOfficeFile(dstFileName, dstFileName2, password);
	
	result = crypt_file.EncryptOfficeFile(srcFileName, dstFileName, password);
	result = crypt_file.DecryptOfficeFile(dstFileName, dstFileName2, password, bDataIntegrity);
	
	return 0;
}

