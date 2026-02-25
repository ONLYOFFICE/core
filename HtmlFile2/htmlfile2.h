#ifndef _HTMLFILE2_HTMLFILE2_H
#define _HTMLFILE2_HTMLFILE2_H

#include <string>
#include <vector>
#include "../DesktopEditor/common/Types.h"

#include "HTMLParameters.h"
#include "MarkdownParameters.h"

#ifndef HTMLFILE2_USE_DYNAMIC_LIBRARY
#define HTMLFILE2_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define HTMLFILE2_DECL_EXPORT Q_DECL_EXPORT
#endif

#ifdef USE_OLD_HTML_CONVERTER
class CHtmlFile2_Private;
#else
namespace HTML { class CHTMLReader; }
#endif

class HTMLFILE2_DECL_EXPORT CHtmlFile2
{
private:
    #ifdef USE_OLD_HTML_CONVERTER
    CHtmlFile2_Private* m_internal;
    #else
    HTML::CHTMLReader* m_pReader;
    #endif
public:
    CHtmlFile2();
    ~CHtmlFile2();

    #ifdef USE_OLD_HTML_CONVERTER
    bool IsHtmlFile(const std::wstring& wsFile);
    bool IsMhtFile (const std::wstring& wsFile);
    #endif

    void SetTempDirectory(const std::wstring& wsFolder);
    void SetCoreDirectory(const std::wstring& wsFolder);

    HRESULT ConvertHTML2OOXML   (const std::wstring& wsPath, const std::wstring& wsDirectory, HTML::THTMLParameters*     pParametrs = nullptr);
    HRESULT ConvertHTML2Markdown(const std::wstring& wsPath, const std::wstring& wsFinalFile, HTML::TMarkdownParameters* pParametrs = nullptr);

    HRESULT ConvertMHT2OOXML    (const std::wstring& wsPath,  const std::wstring& wsDirectory,  HTML::THTMLParameters*     pParametrs = nullptr);
    HRESULT ConvertMHT2Markdown (const std::wstring& wsPath,  const std::wstring& wsFinalFile,  HTML::TMarkdownParameters* pParametrs = nullptr);

    HRESULT ConvertHTML2OOXML   (const std::vector<std::wstring>& arPaths, const std::wstring& wsDirectory, HTML::THTMLParameters*     pParametrs = nullptr);
    HRESULT ConvertHTML2Markdown(const std::vector<std::wstring>& arPaths, const std::wstring& wsFinalFile, HTML::TMarkdownParameters* pParametrs = nullptr);

    HRESULT ConvertMHT2OOXML    (const std::vector<std::wstring>& arPaths, const std::wstring& wsDirectory, HTML::THTMLParameters*     pParametrs = nullptr);
    HRESULT ConvertMHT2Markdown (const std::vector<std::wstring>& arPaths, const std::wstring& wsFinalFile, HTML::TMarkdownParameters* pParametrs = nullptr);
};

#endif // _HTMLFILE2_HTMLFILE2_H
