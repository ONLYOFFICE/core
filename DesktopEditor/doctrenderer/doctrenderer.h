#ifndef DOCTRENDERER_H
#define DOCTRENDERER_H

#include <string>
#include <vector>
#include "../common/Types.h"
#include "../common/Array.h"
#include "../common/base_export.h"

namespace NSDoctRenderer
{
    namespace DoctRendererFormat
    {
        enum FormatFile
        {
            DOCT	= 0,
            XLST	= 1,
            PPTT	= 2,
            PDF		= 3,
            HTML        = 4,

            INVALID = 255
        };
    }

    class Q_DECL_EXPORT CExecuteParams
    {
    public:
        DoctRendererFormat::FormatFile m_eSrcFormat;
        DoctRendererFormat::FormatFile m_eDstFormat;

        std::wstring m_strFontsDirectory;
        std::wstring m_strImagesDirectory;
        std::wstring m_strThemesDirectory;

        std::wstring m_strSrcFilePath;
        std::wstring m_strDstFilePath;

        CArray<std::wstring> m_arChanges;
        int m_nCountChangesItems;

        std::wstring m_strMailMergeDatabasePath;
        std::wstring m_strMailMergeField;
        int m_nMailMergeIndexStart;
        int m_nMailMergeIndexEnd;

    public:
        CExecuteParams();
        ~CExecuteParams();

    public:
        bool FromXml(const std::wstring& sXml);
    };
}

namespace NSDoctRenderer
{
    class CDoctRenderer_Private;
    class Q_DECL_EXPORT CDoctrenderer
    {
    public:
        CDoctrenderer();
        ~CDoctrenderer();

    public:
        bool Execute(const std::wstring& strXml, std::wstring& strError);
        std::vector<std::wstring> GetImagesInChanges();

    private:
        CDoctRenderer_Private* m_pInternal;
    };
}

#endif // DOCTRENDERER_H
