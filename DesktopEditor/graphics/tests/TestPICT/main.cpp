#include <codecvt>

#include "../../pro/Graphics.h"

int main(int argc, char *argv[])
{
    Aggplus::CImage Cimg(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(argv[1]));
    Cimg.SaveFile(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(argv[2]), 1);

    return 0;
}
