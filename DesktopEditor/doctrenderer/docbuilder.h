#ifndef DOCBUILDER_H
#define DOCBUILDER_H

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
        bool OpenFile(const wchar_t* path, const wchar_t* params);
        bool CreateFile(const int& type);
        void SetTmpFolder(const wchar_t* folder);
        bool SaveFile(const int& type, const wchar_t* path);
        void CloseFile();
        bool ExecuteCommand(const wchar_t* command);

        bool Run(const wchar_t* path);

        bool RunTextW(const wchar_t* commands);
        bool RunTextA(const char* commands);

        void SetProperty(const char* param, const wchar_t* value);
        void SetPropertyW(const wchar_t* param, const wchar_t* value);

    public:
        static void Initialize();
        static void Dispose();

    private:
        CDocBuilder_Private* m_pInternal;
    };
}

#endif // DOCBUILDER_H
