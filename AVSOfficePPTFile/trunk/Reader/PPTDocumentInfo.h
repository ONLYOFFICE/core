#pragma once
#include "PPTDocumentInfoOneUser.h"

class CPPTDocumentInfo
{
public:
	CCurrentUser m_oCurrentUser;
	CAtlArray<CPPTUserInfo*> m_arUsers;

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
		size_t nCount = m_arUsers.GetCount();
		while(0 != nCount) 
		{ 
			if (NULL != m_arUsers[0])
				delete m_arUsers[0]; 
			
			m_arUsers.RemoveAt(0);
			--nCount;
		} 
	}

	void ReadFromStream(CRecordCurrentUserAtom* pCurrentUser, IStream* pStream, CStringW strFolderMem)
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

			pStream->Read(&oHeader, sizeof(SRecordHeader), NULL);
			oUserAtom.ReadFromStream(oHeader, pStream);

			CPPTUserInfo* pInfo = new CPPTUserInfo();
			pInfo->m_strFileDirectory = m_strFileDirectory;
			BOOL bRes = pInfo->ReadFromStream(&oUserAtom, pStream, strFolderMem);

			offsetToEdit = pInfo->m_oUser.m_nOffsetLastEdit;
			
			if (!bRes)
			{
				delete pInfo;
				continue;
			}
			
			m_arUsers.Add(pInfo);
			// теперь нужно выставить у него параметры для других юзеров
			pInfo->m_pDocumentInfo = this;
			pInfo->m_lIndexThisUser = m_arUsers.GetCount() - 1;

			pInfo = NULL;
		}

		// посмотреть, где выставлены blip. если не выставлено - то можно поменять
		
		LONG lCount = m_arUsers.GetCount();
		if (1 < lCount)
		{
			m_arUsers[0]->m_arEmptyPictures.Copy(m_arUsers[lCount - 1]->m_arEmptyPictures);
		}

		// теперь нужно у всех сделать FromDocument...
		for (size_t i = 0; i < m_arUsers.GetCount(); ++i)
		{
			m_arUsers[i]->FromDocument();
		}

		#ifdef _DEBUG
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

		for (size_t nIndex = 0; nIndex < m_arUsers.GetCount(); ++nIndex)
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
		for (size_t i = 0; i < m_arUsers[nUser]->m_arEmptyPictures.GetCount(); ++i)
		{
			if (!m_arUsers[nUser]->m_arEmptyPictures[i])
				++lRes;
		}
		return lRes;
	}
};