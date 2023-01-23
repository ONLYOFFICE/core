#ifndef CEPUBFILE_H
#define CEPUBFILE_H

#include "src/EpubFile_global.h"
#include "src/CBookInfo.h"
#include "src/CBookItem.h"
#include "src/CBookToc.h"
#include "src/CBookContentItem.h"

#include <string>
#include <vector>
#include <map>

class EPUBFILE_EXPORT CEpubFile
{
    std::wstring m_sFileName;
    std::wstring m_sTempDir;

    CBookInfo m_oBookInfo;
    CBookToc  m_oToc;
    std::map<std::wstring, CBookItem> m_mapRefs;
    std::vector<CBookContentItem>  m_arContents;

public:
    CEpubFile();
    ~CEpubFile();

    void Clear();
    HRESULT IsEpubFile(const std::wstring& sfileName);
    void SetTempDirectory(const std::wstring& stempDir);
    HRESULT Convert(const std::wstring& sInputFile, const std::wstring& sOutputFile, bool bIsOutCompress = true);
    void ShowMap();
    HRESULT FromHtml(const std::wstring& sHtmlFile, const std::wstring& sDstFile, const std::wstring& sInpTitle);
};

#endif // CEPUBFILE_H
