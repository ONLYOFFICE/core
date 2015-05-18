#ifndef DOCTRENDERER_H
#define DOCTRENDERER_H

#include <string>
#include "../common/Types.h"
#include "../common/Array.h"

#include <QtCore/qglobal.h>

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
            HTML    = 4,

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
    class Q_DECL_EXPORT CDoctrenderer
    {
    private:
        CExecuteParams m_oParams;

        std::wstring m_strConfigDir;
        std::wstring m_strConfigPath;
        CArray<std::wstring> m_arrFiles;

        std::wstring m_strDoctSDK;
        std::wstring m_strPpttSDK;
        std::wstring m_strXlstSDK;

        std::wstring m_strEditorType;
        std::wstring m_strFilePath;

        bool m_bIsInitTypedArrays;

    public:
        CDoctrenderer();
        ~CDoctrenderer();

    public:
        bool Execute(const std::wstring& strXml, std::wstring& strError);

    private:
        bool ExecuteScript(const std::string& strScript, std::wstring& strError);
        std::string ReadScriptFile(const std::wstring& strFile);
    };
}

#endif // DOCTRENDERER_H
