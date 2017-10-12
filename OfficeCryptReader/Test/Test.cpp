// Test.cpp : Defines the entry point for the console application.
//
#include "tchar.h"
#include "../source/ECMACryptFile.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//std::wstring srcFileName	= L"D:\\test\\_crypted\\test-password-2016.docx";
	//std::wstring dstFileName	= srcFileName + L".oox";
	//std::wstring dstFileName2	= dstFileName + L"-mycrypt.docx";
	
	std::wstring srcFileName	= L"D:\\test\\_crypted\\test.docx";
	std::wstring dstFileName2	= srcFileName + L"-mycrypt.docx";

	std::wstring password = L"password132eqdqdwewedwdwwskskms09elzwewedskjsdnkjsdnjksjsnkcsdncskjdnss";

	ECMACryptFile crypt_file;
	bool result, bDataIntegrity;
		
	//result = crypt_file.DecryptOfficeFile(srcFileName, dstFileName, password, bDataIntegrity);

	//result = crypt_file.EncryptOfficeFile(dstFileName, dstFileName2, password);
	result = crypt_file.EncryptOfficeFile(srcFileName, dstFileName2, password);
	return 0;
}

