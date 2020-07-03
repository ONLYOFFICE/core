#include "../CEpubFile.h"

#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    CEpubFile Epub;

    if (Epub.IsEbubFile(L"../../../Files/BULGAKOV.epub"))
    {
        Epub.SetTempDirectory(L"Temp");
        Epub.Convert(L"../../../Files/BULGAKOV.epub", L"");
    }
    return 0;
}
