#ifndef CEPUBFILE_H
#define CEPUBFILE_H

#include "src/EpubFile_global.h"
#include <iostream>

class EPUBFILE_EXPORT CEpubFile
{
    std::wstring _fileName;
    std::wstring _tempDir;

public:
    CEpubFile();
    ~CEpubFile();

    bool IsEbubFile(const std::wstring& fileName);
    void SetTempDirectory(const std::wstring& tempDir);
    bool Convert(const std::wstring& inputFile, const std::wstring& outputFile);
};

#endif // CEPUBFILE_H
