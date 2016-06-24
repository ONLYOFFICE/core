/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once

#include "PPTDocumentInfoOneUser.h"

class CPPTDocumentInfo
{
public:
	CCurrentUser					m_oCurrentUser;
	std::vector<CPPTUserInfo*>		m_arUsers;
	CString							m_strFileDirectory;
	std::map<int, std::wstring>		m_mapStoreImageFile;

public:

	CPPTDocumentInfo() : m_oCurrentUser(), m_arUsers() 
	{
		m_strFileDirectory = _T("");
	}

	~CPPTDocumentInfo()
	{
		Clear();
	}

	void Clear()
	{
		size_t nCount = m_arUsers.size();
		while(0 != nCount) 
		{ 
			if (NULL != m_arUsers[nCount-1])
				delete m_arUsers[nCount-1]; 
			
			m_arUsers.pop_back();
			--nCount;
		} 
	}

	void ReadFromStream(CRecordCurrentUserAtom* pCurrentUser, POLE::Stream* pStream, CStringW strFolderMem)
	{
		m_oCurrentUser.FromAtom(pCurrentUser);

		DWORD offsetToEdit = m_oCurrentUser.m_nOffsetToCurrentEdit;
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
			pInfo->m_strFileDirectory	= m_strFileDirectory;
           
			bool bRes = pInfo->ReadFromStream(&oUserAtom, pStream, strFolderMem);

			offsetToEdit = pInfo->m_oUser.m_nOffsetLastEdit;
			
			if (!bRes)
			{
				delete pInfo;
				continue;
			}
			
			m_arUsers.push_back(pInfo);
			// теперь нужно выставить у него параметры для других юзеров
			pInfo->m_pDocumentInfo = this;
			pInfo->m_lIndexThisUser = m_arUsers.size() - 1;

			pInfo = NULL;
		}
		// теперь нужно у всех сделать FromDocument...или только для ПОСЛЕДНЕГО пользователя ??? (остальные то  не нужны)
		//for (int i = 0; i < m_arUsers.size(); ++i)
		if (m_arUsers.size() > 0)
		{
			m_arUsers[0]->FromDocument();
		}
	}
};
