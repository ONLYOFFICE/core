#include "PPTDocumentInfo.h"


CPPTDocumentInfo::CPPTDocumentInfo() : m_oCurrentUser(), m_bMacros(true)
{
}

CPPTDocumentInfo::~CPPTDocumentInfo()
{
    Clear();
}

void CPPTDocumentInfo::Clear()
{
    size_t nCount = m_arUsers.size();
    while(0 != nCount)
    {
        if (NULL != m_arUsers[nCount-1])
            RELEASEOBJECT(m_arUsers[nCount-1])

                    m_arUsers.pop_back();
        --nCount;
    }
}

bool CPPTDocumentInfo::ReadFromStream(CRecordCurrentUserAtom *pCurrentUser, POLE::Stream *pStream)
{
    m_oCurrentUser.FromAtom(pCurrentUser);

    _UINT32 offsetToEdit = m_oCurrentUser.m_nOffsetToCurrentEdit;
    SRecordHeader oHeader;
    CRecordUserEditAtom oUserAtom;

    Clear();

    size_t nCountUsers = 0;

    while (0 < offsetToEdit)
    {
        StreamUtils::StreamSeek(offsetToEdit, pStream);

        oHeader.ReadFromStream(pStream);
        oUserAtom.ReadFromStream(oHeader, pStream);

        CPPTUserInfo* pInfo			= new CPPTUserInfo();

        pInfo->m_strTmpDirectory	= m_strTmpDirectory;
        pInfo->m_bEncrypt			= m_oCurrentUser.m_bIsEncrypt;
        pInfo->m_strPassword		= m_strPassword;
        pInfo->m_bMacros			= m_bMacros;

        bool bResult = pInfo->ReadFromStream(&oUserAtom, pStream);

        m_bMacros					= pInfo->m_bMacros;
        offsetToEdit				= pInfo->m_oUser.m_nOffsetLastEdit;
        m_oCurrentUser.m_bIsEncrypt = pInfo->m_bEncrypt;

        if (bResult == false)
        {
            delete pInfo;
            pInfo = NULL;   // may not work

            if (m_oCurrentUser.m_bIsEncrypt)
                return false;
            else
                continue;
        }

        m_arUsers.push_back(pInfo);
        // теперь нужно выставить у него параметры для других юзеров
        pInfo->m_pDocumentInfo = this;
        pInfo->m_lIndexThisUser = m_arUsers.size() - 1;

        pInfo = NULL;
    }

    return true;
}

bool CPPTDocumentInfo::LoadDocument(std::wstring strFolderMem)
{
    if (m_arUsers.empty()) return false;

    try
    {
        m_arUsers[0]->ReadExtenalObjects(strFolderMem);
        m_arUsers[0]->FromDocument();
    }
    catch(int) //error code
    {
        return false;
    }

    return true;
}
