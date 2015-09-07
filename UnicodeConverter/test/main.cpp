#include "../UnicodeConverter.h"
#include "../../DesktopEditor/common/File.h"

int main(int argc, char *argv[])
{
    NSUnicodeConverter::CUnicodeConverter oConverter;

    std::string str1 = oConverter.fromUnicode(L"рус", "windows-1251");

    std::wstring str2 = oConverter.toUnicode(str1, "windows-1251");
    std::string str3 = oConverter.fromUnicode(str2, "windows-1251");

    return 0;
}
