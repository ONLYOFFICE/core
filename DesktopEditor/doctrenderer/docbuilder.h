#ifndef DOCBUILDER_H
#define DOCBUILDER_H

#include <string>
#include <vector>
#include "../common/base_export.h"

namespace NSDoctRenderer
{
    class CDocBuilder_Private;
    class Q_DECL_EXPORT CDocBuilder
    {
    public:
        CDocBuilder();
        ~CDocBuilder();

    public:
        bool OpenFile(const std::wstring& path, const std::wstring& params);
        bool CreateFile(const int& type);
        void SetTmpFolder(const std::wstring& folder);
        bool SaveFile(const int& type, const std::wstring& path);
        void CloseFile();
        bool ExecuteCommand(const std::wstring& command);

    private:
        CDocBuilder_Private* m_pInternal;
    };
}

#endif // DOCBUILDER_H
