#pragma once

#include "PPTDocumentInfoOneUser.h"

class CPPTDocumentInfo
{
public:
	CCurrentUser m_oCurrentUser;
	std::vector<CPPTUserInfo*> m_arUsers;

	CString m_strFileDirectory;

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

			CPPTUserInfo* pInfo = new CPPTUserInfo();
			pInfo->m_strFileDirectory = m_strFileDirectory;
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

		// посмотреть, где выставлены blip. если не выставлено - то можно поменять
		
		LONG lCount = m_arUsers.size();
		if (1 < lCount)
		{
			m_arUsers[0]->m_arEmptyPictures = m_arUsers[lCount - 1]->m_arEmptyPictures;
		}

		// теперь нужно у всех сделать FromDocument...
		for (size_t i = 0; i < m_arUsers.size(); ++i)
		{
			m_arUsers[i]->FromDocument();
		}

    #if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			SaveXmlInfo();
    #endif
	}

	void SaveXmlInfo()
	{
		if (!PPT_DUMP_LOG)
			return;
		
		XmlUtils::CXmlWriter oWriter;

		oWriter.WriteNodeBegin(_T("PPTInfo"));

		oWriter.WriteString(m_oCurrentUser.ToString());

		for (size_t nIndex = 0; nIndex < m_arUsers.size(); ++nIndex)
		{
			oWriter.WriteString(m_arUsers[nIndex]->m_strXmlInfo);
		}

		oWriter.WriteNodeEnd(_T("PPTInfo"));

		//CDirectory::SaveToFile(_T("C:\\PPTInfo.xml"), oWriter.GetXmlString());

		oWriter.SaveToFile(_T("C:\\PPTInfo.xml"), TRUE);
	}

	LONG GetCountPicturesUsed(size_t nUser)
	{
		LONG lRes = 0;
		for (size_t i = 0; i < m_arUsers[nUser]->m_arEmptyPictures.size(); ++i)
		{
			if (!m_arUsers[nUser]->m_arEmptyPictures[i])
				++lRes;
		}
		return lRes;
	}
};
