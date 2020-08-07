#ifndef CEPUBFILE_H
#define CEPUBFILE_H

#include "src/EpubFile_global.h"
#include "src/CBookInfo.h"
#include "src/CBookItem.h"
#include "src/CBookToc.h"
#include "src/CBookContentItem.h"
#include <iostream>

class EPUBFILE_EXPORT CEpubFile
{
    std::wstring m_sFileName;
    std::wstring m_sTempDir;

    CBookInfo m_oBookInfo;
    std::map <std::wstring, CBookItem> m_mapRefs;
    CBookToc m_oToc;
    std::vector<CBookContentItem> m_arContents;

public:
    CEpubFile();
    ~CEpubFile();

    void Clear();
    HRESULT IsEbubFile(const std::wstring& sfileName);
    void SetTempDirectory(const std::wstring& stempDir);
    HRESULT Convert(const std::wstring& sInputFile, const std::wstring& sOutputFile);
    void ShowMap();
};

#endif // CEPUBFILE_H
