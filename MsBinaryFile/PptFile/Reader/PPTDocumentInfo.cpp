/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "PPTDocumentInfo.h"

using namespace PPT;

CPPTDocumentInfo::CPPTDocumentInfo() : m_oCurrentUser(), m_bMacroEnabled(true), m_pStream(NULL)
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
    m_pStream = pStream;
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

        CPPTUserInfo* pInfo = new CPPTUserInfo();

        pInfo->m_pDocumentInfo      = this;

        pInfo->m_bEncrypt = m_oCurrentUser.m_bIsEncrypt;
        pInfo->m_strPassword = m_strPassword;
        pInfo->m_bMacroEnabled = m_bMacroEnabled;

        bool bResult = pInfo->ReadFromStream(&oUserAtom, pStream);

        offsetToEdit = pInfo->m_oUser.m_nOffsetLastEdit;
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
        pInfo->m_lIndexThisUser = m_arUsers.size() - 1;

        pInfo = NULL;
    }
    return true;
}
std::wstring CPPTDocumentInfo::GetBinFromStg(const std::wstring& name, _UINT32 nRef)
{
    for (size_t i = 0; i < m_arUsers.size(); ++i)
    {
        std::map<_UINT32, _UINT32>::iterator nIndexPsrRef = m_arUsers[i]->m_mapOffsetInPIDs.find(nRef);
        if (m_arUsers[i]->m_mapOffsetInPIDs.end() != nIndexPsrRef)
        {
            std::wstring result;
            _UINT32 offset_stream = nIndexPsrRef->second;
            StreamUtils::StreamSeek(offset_stream, m_pStream);

            SRecordHeader oHeader;
            oHeader.ReadFromStream(m_pStream);

            CRecordExObjStg* pExObjStg = new CRecordExObjStg(name, m_pCommonInfo->tempPath);

            if (pExObjStg)
            {
                pExObjStg->ReadFromStream(oHeader, m_pStream);
                result = pExObjStg->m_sFileName;

                RELEASEOBJECT(pExObjStg);
            }
            return result;
        }
    }
    return L"";
}

bool CPPTDocumentInfo::LoadDocument()
{
    if (m_arUsers.empty()) return false;

    try
    {
        m_arUsers[0]->ReadExtenalObjects(); // todooo ???? прочитать по всем (см 66864)
        m_arUsers[0]->FromDocument();

        m_bMacroEnabled = m_arUsers[0]->m_bMacroEnabled;
    }
    catch(int) //error code
    {
        return false;
    }

    return true;
}
