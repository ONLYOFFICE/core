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

		// для поиска правильных индексов картинок - накатим на активного пользователя все остальные возможные каартинки
		for (size_t i = 1; i < m_arUsers.size(); ++i)
		{
			for (long j = 0 ; j < m_arUsers[i]->m_arEmptyPictures.size(); j++)
			{
				if (m_arUsers[i]->m_arEmptyPictures[j] == false && j < m_arUsers[0]->m_arEmptyPictures.size() && m_arUsers[0]->m_arEmptyPictures[j] == true)
					m_arUsers[0]->m_arEmptyPictures[j] = false;
			}
		}


		// теперь нужно у всех сделать FromDocument...
		for (size_t i = 0; i < m_arUsers.size(); ++i)
		{
			m_arUsers[i]->FromDocument();
		}
	}

	int GetCountPicturesUsed(size_t nUser)
	{
		int lRes = 0;
		for (size_t i = 0; i < m_arUsers[nUser]->m_arEmptyPictures.size(); ++i)
		{
			if (!m_arUsers[nUser]->m_arEmptyPictures[i])
				++lRes;
		}
		return lRes;
	}
};
