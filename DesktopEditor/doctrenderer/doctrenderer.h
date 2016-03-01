#ifndef DOCTRENDERER_H
#define DOCTRENDERER_H

#include <string>
#include <vector>
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
}

namespace NSDoctRenderer
{
    class CDoctRenderer_Private;
    class Q_DECL_EXPORT CDoctrenderer
    {
    public:
        CDoctrenderer(const std::wstring& sAllFontsPath = L"");
        ~CDoctrenderer();

    public:
        bool Execute(const std::wstring& strXml, std::wstring& strError);
        std::vector<std::wstring> GetImagesInChanges();

    private:
        CDoctRenderer_Private* m_pInternal;
    };
}

#endif // DOCTRENDERER_H
