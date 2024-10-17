#include <iostream>
#include <stdio.h>
#include <codecvt>

#include "../../pro/Graphics.h"
#include "../../Image.h"

int main(int argc, char *argv[])
{
    Aggplus::CImage cimg;
    for (size_t i = 1; i <= argc/2; i++)
    {
        cimg.GetInsideFrom(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(argv[i]), std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(argv[i + argc/2]));
    }


    return 0;
}
