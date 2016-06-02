#include "docbuilder_p.h"

namespace NSDoctRenderer
{
    CDocBuilder::CDocBuilder()
    {
        m_pInternal = new CDocBuilder_Private();
    }
    CDocBuilder::~CDocBuilder()
    {
        RELEASEOBJECT(m_pInternal);
    }

    bool CDocBuilder::OpenFile(const wchar_t* path, const wchar_t* params)
    {
        m_pInternal->m_nFileType = -1;
        if (!NSDirectory::Exists(m_pInternal->m_sTmpFolder))
            NSDirectory::CreateDirectory(m_pInternal->m_sTmpFolder);

        return m_pInternal->OpenFile(path, params);
    }
    bool CDocBuilder::SaveFile(const int& type, const wchar_t* path)
    {
        return m_pInternal->SaveFile(type, path);
    }
    bool CDocBuilder::ExecuteCommand(const wchar_t* command)
    {
        return m_pInternal->ExecuteCommand(command);
    }
}
