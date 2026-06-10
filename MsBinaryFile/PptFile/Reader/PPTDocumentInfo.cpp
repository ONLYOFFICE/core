/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

        bool bResult = oHeader.ReadFromStream(pStream);
        if (!bResult) break;

        oUserAtom.ReadFromStream(oHeader, pStream);

        CPPTUserInfo* pInfo = new CPPTUserInfo();

        pInfo->m_pDocumentInfo = this;

        pInfo->m_bEncrypt = m_oCurrentUser.m_bIsEncrypt;
        pInfo->m_strPassword = m_strPassword;
        pInfo->m_bMacroEnabled = m_bMacroEnabled;

        bResult = pInfo->ReadFromStream(&oUserAtom, pStream);

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
        // now need to set parameters for other users
        pInfo->m_lIndexThisUser = m_arUsers.size() - 1;

        pInfo = NULL;
    }
    return (false == m_arUsers.empty());
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
        m_arUsers[0]->ReadExtenalObjects(); // todo ???? read for all (see 66864)
        m_arUsers[0]->FromDocument();

        m_bMacroEnabled = m_arUsers[0]->m_bMacroEnabled;
    }
    catch(int) //error code
    {
        return false;
    }

    return true;
}
