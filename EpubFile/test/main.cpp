#include "../CEpubFile.h"

int main(int argc, char *argv[])
{
    CEpubFile Epub;

    if (Epub.IsEbubFile(L"../../../Files/Harry Potter 1.epub"))
    {
        Epub.SetTempDirectory(L"Temp");
        Epub.Convert(L"../../../Files/Harry Potter 1.epub", L"");
    }
    return 0;
}
