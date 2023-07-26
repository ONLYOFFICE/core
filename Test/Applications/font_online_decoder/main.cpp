#include <iostream>
#include "../../../DesktopEditor/common/File.h"

#ifdef WIN32
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif
{
	if (0 == argc)
		return 0;

#ifdef WIN32
	std::wstring sFile(argv[argc - 1]);
#else
	std::string sFileA(argv[argc - 1]);
	std::wstring sFile = UTF8_TO_U(sFileA);
#endif

	BYTE* pData = NULL;
	DWORD nSize = 0;

	NSFile::CFileBinary::ReadAllBytes(sFile, &pData, nSize);

	BYTE guidOdttf[] = {0xA0, 0x66, 0xD6, 0x20, 0x14, 0x96, 0x47, 0xfa, 0x95, 0x69, 0xB8, 0x50, 0xB0, 0x41, 0x49, 0x48};

	int nCount = nSize > 32 ? 32 : nSize;
	for (int i = 0; i < nCount; ++i)
		pData[i] ^= guidOdttf[i % 16];

	NSFile::CFileBinary oDst;
	oDst.CreateFileW(NSFile::GetProcessDirectory() + L"/font.ttf");
	oDst.WriteFile(pData, nSize);
	oDst.CloseFile();

	RELEASEARRAYOBJECTS(pData);
	return 0;
}
