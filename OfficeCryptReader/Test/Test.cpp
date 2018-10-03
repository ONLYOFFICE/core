// Test.cpp : Defines the entry point for the console application.
//
#include "tchar.h"
#include "../source/ECMACryptFile.h"
#include "../../Common/OfficeFileFormatChecker.h"

#if defined(_WIN64)
	#pragma comment(lib, "../../build/bin/icu/win_64/icuuc.lib")
#elif defined (_WIN32)

	#if defined(_DEBUG)
		//#pragma comment(lib, "../../build/lib/win_32/DEBUG/graphics.lib")
		#pragma comment(lib, "../../build/lib/win_32/DEBUG/kernel.lib")
		#pragma comment(lib, "../../build/lib/win_32/DEBUG/UnicodeConverter.lib")
	#else
		//#pragma comment(lib, "../../build/lib/win_32/graphics.lib")
		#pragma comment(lib, "../../build/lib/win_32/kernel.lib")
	#endif
	#pragma comment(lib, "../../build/bin/icu/win_32/icuuc.lib")
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	std::wstring password = L"password";
	ECMACryptFile crypt_file;
	bool result = false, bDataIntegrity = false;

	std::wstring srcFileName	= L"D:\\test\\_crypted\\test-111.docx";
	std::wstring dstFileName	= srcFileName + L"-mycrypt.docx";
	std::wstring dstFileName2	= dstFileName + L".oox";
	
	//std::wstring dstFileName	= srcFileName + L".oox";
	//std::wstring dstFileName2	= dstFileName + L"-mycrypt.docx";

	//std::wstring srcFileName	= L"D:\\test\\_crypted\\test.docx";
	//std::wstring dstFileName3	= srcFileName + L"-mycrypt1.docx";

	//std::wstring srcFileName1	= L"D:\\test\\_crypted\\test-111.docx-ms.docx";
	//std::wstring dstFileName1	= srcFileName1 + L".oox";
	//result = crypt_file.DecryptOfficeFile(srcFileName1, dstFileName1, password, bDataIntegrity);
			
	result = crypt_file.EncryptOfficeFile(srcFileName, dstFileName, password, L"123456789");
	result = crypt_file.DecryptOfficeFile(dstFileName, dstFileName2, password, bDataIntegrity);
		
	return 0;
}
