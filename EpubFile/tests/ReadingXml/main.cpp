#include "cepubformat.h"

int main()
{
    std::wstring fileName = L"../../../../EpubFiles/BULGAKOV.epub";
    std::wstring tempDir = L"../../../../tempDirectory";

    CEpubFormat Epub;

    if (Epub.IsEbubFile(fileName))
    {
        Epub.SetTempDirectory(tempDir);
        Epub.Convert();
    }

    return 0;
}
