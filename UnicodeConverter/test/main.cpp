#include "../UnicodeConverter.h"
#include "../../DesktopEditor/common/File.h"

int main(int argc, char *argv[])
{
    std::wstring sPathTests = NSFile::GetProcessDirectory() + L"/../testfiles/";

#if 0
    std::wstring sOriginal = sPathTests + L"0.txt";
    for (int i = 1; i < 52; i++)
    {
        std::wstring s1 = sPathTests + std::to_wstring(i) + L".txt";
        std::wstring s2 = sPathTests + std::to_wstring(i) + L"_utf8.txt";
        NSFile::CFileBinary::Copy(sOriginal, s1);
        NSFile::CFileBinary::Copy(sOriginal, s2);
    }

    return 0;
#endif

    NSUnicodeConverter::CUnicodeConverter oConverter;

    int nCountTests = 46;

    for (int i = 0; i < nCountTests; ++i)
    {
        BYTE* pDataSrc = NULL;
        DWORD nLenSrc = 0;

        if (!NSFile::CFileBinary::ReadAllBytes(sPathTests + std::to_wstring(i) + L".txt", &pDataSrc, nLenSrc))
            continue;

        std::wstring sResult = oConverter.toUnicode((const char*)pDataSrc, (unsigned int)nLenSrc, NSUnicodeConverter::Encodings[i].Name);

        std::wstring sResultCache = L"";
        NSFile::CFileBinary::ReadAllTextUtf8(sPathTests + std::to_wstring(i) + L"_utf8.txt", sResultCache);

        bool bIsEqual = (sResult == sResultCache);

        RELEASEARRAYOBJECTS(pDataSrc);

        NSFile::CFileBinary::SaveToFile(sPathTests + std::to_wstring(i) + L"_test_utf8.txt", sResult, true);
    }

#if 0
    std::string str1 = oConverter.fromUnicode(L"рус", "windows-1251");

    std::wstring str2 = oConverter.toUnicode(str1, "windows-1251");
    std::string str3 = oConverter.fromUnicode(str2, "windows-1251");
#endif

    return 0;
}
