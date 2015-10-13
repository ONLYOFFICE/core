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
			// теперь нужно выставить у него параметры дл€ других юзеров
			pInfo->m_pDocumentInfo = this;
			pInfo->m_lIndexThisUser = m_arUsers.size() - 1;

			pInfo = NULL;
		}
		// теперь нужно у всех сделать FromDocument...или только дл€ ѕќ—Ћ≈ƒЌ≈√ќ пользовател€ ??? (остальные то  не нужны)
		//for (int i = 0; i < m_arUsers.size(); ++i)
		if (m_arUsers.size() > 0)
		{
			m_arUsers[0]->FromDocument();
		}
	}
};
