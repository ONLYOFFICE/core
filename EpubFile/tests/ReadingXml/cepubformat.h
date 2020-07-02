#ifndef CEPUBFORMAT_H
#define CEPUBFORMAT_H

#include <iostream>
#include "../../../OfficeUtils/src/OfficeUtils.h"
#include "../../../DesktopEditor/xml/include/xmlutils.h"
#include <map>


class CEpubFormat
{
    std::wstring _fileName;
    std::wstring _tempDir;

    std::map <std::wstring, std::wstring> dataAboutFile;

public:
    CEpubFormat();
    virtual ~CEpubFormat();
    bool IsEbubFile(const std::wstring& fileName);
    void SetTempDirectory(const std::wstring& tempDir);
    void Convert();
};

#endif // CEPUBFORMAT_H
