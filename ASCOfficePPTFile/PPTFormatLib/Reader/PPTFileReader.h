#pragma once

#include "../../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"
#include "../Records/Drawing/ArtBlip.h"

#include "PPTDocumentInfo.h"

#define CURRENT_USER_STREAM		"Current User" 
#define DOCUMENT_STREAM			"PowerPoint Document" 
#define PICTURE_STREAM			"Pictures" 
#define HEADER_STREAM			"Header" 

#define PP97_DUALSTORAGE		"PP97_DUALSTORAGE"

class CPPTFileReader
{
public: 
	CPPTFileReader(POLE::Storage *pStg, CStringW strTemp): 
		   m_pPowerPointStg(pStg),  
           m_bIsPPTFile(false),
		   m_pDocStream(NULL), 
		   m_pPictureStream(NULL),
		   m_lImagesCount(0),
		   m_strMemoryForder(strTemp),
		   m_oDocumentInfo()
	{ 
		m_bDualStorage = false;

		POLE::Stream *pStm = new POLE::Stream( m_pPowerPointStg, CURRENT_USER_STREAM);
		if (pStm == NULL) return;
				
		if ( ReadCurrentUser(pStm)) 
		  m_bIsPPTFile = TRUE; 
		else
		{
			RELEASEOBJECT(pStm);

			std::string stream_name = std::string(PP97_DUALSTORAGE) + std::string("/") + std::string(CURRENT_USER_STREAM);
			pStm = new POLE::Stream( m_pPowerPointStg, stream_name);
			if (pStm == NULL) return;
			
			m_bDualStorage = true;
			if ( ReadCurrentUser(pStm)) 
			  m_bIsPPTFile = TRUE; 
		}
		
		RELEASEOBJECT(pStm);

		if (0 == m_strMemoryForder.GetLength())
		{
			m_strMemoryForder = FileSystem::Directory::GetTempPath();
		}

		m_strMemoryForder = FileSystem::Directory::CreateDirectoryWithUniqueName(m_strMemoryForder);

	}
	~CPPTFileReader()
	{
		RELEASEOBJECT(m_pDocStream); 
		RELEASEOBJECT(m_pPictureStream); 
	
		RELEASEOBJECT(m_pPowerPointStg);
		
		FileSystem::Directory::DeleteDirectory(m_strMemoryForder);
	}

    bool IsPowerPoint()
	{ 
		return m_bIsPPTFile;
	} 

	void ReadPersistDirectory()
	{
        bool bRes = SavePictures();	
		// нужно вызывать РОВНО один раз...
		m_oDocumentInfo.ReadFromStream(&m_oCurrentUser, GetDocStream(), m_strMemoryForder);
	}

	void ReadSlideList()
	{
		if (m_oDocumentInfo.m_arUsers.size() > 0)
		{

			DWORD nPID = m_oDocumentInfo.m_arUsers[0]->m_oUser.m_nDocumentRef;
			std::map<DWORD, DWORD>::iterator pPair = m_oDocumentInfo.m_arUsers[0]->m_mapOffsetInPIDs.find(nPID);

			if (pPair == m_oDocumentInfo.m_arUsers[0]->m_mapOffsetInPIDs.end()) return;

			DWORD offset = pPair->second;
			StreamUtils::StreamSeek((long)offset, GetDocStream());
		}
	}

protected: 

    bool ReadCurrentUser(POLE::Stream *pStm)
	{
		SRecordHeader oHeader; 
        bool isPP = false;
		
		if( oHeader.ReadFromStream(pStm))
		{ 
			m_oCurrentUser.ReadFromStream(oHeader, pStm);
			
			isPP = (m_oCurrentUser.m_nSize == 0x00000014 && (m_oCurrentUser.m_nToken == NO_ENCRYPT) && 
								(m_oCurrentUser.m_nLenUserName <= 255)); 
		} 
 
		return isPP; 
	}

	POLE::Stream* GetDocStream()
	{ 
		if (m_pDocStream == NULL) 
		{ 
			if (!m_bIsPPTFile) 
				return NULL; 
			
			std::string stream_name;
		
			if (m_bDualStorage)	stream_name = std::string(PP97_DUALSTORAGE) + std::string("/");

			m_pDocStream = new POLE::Stream(m_pPowerPointStg, stream_name + DOCUMENT_STREAM);
		} 
		return m_pDocStream; 
 	}
	POLE::Stream* GetPictureStream()
	{ 
		if (m_pPictureStream == NULL) 
		{ 
			if (!m_bIsPPTFile) 
				return NULL; 

			std::string stream_name;

			if (m_bDualStorage)	stream_name = std::string(PP97_DUALSTORAGE) + std::string("/");

			m_pPictureStream = new POLE::Stream(m_pPowerPointStg, stream_name + PICTURE_STREAM);
		} 
		return m_pPictureStream; 
 	}

    bool SavePictures()
	{
		POLE::Stream* pStream = GetPictureStream();

        if (NULL == pStream)
		{
            return false;
		}
		
		SRecordHeader oHeader;
		ULONG nRd = 0;
		m_lImagesCount = 0;

//  удаление картинок при завершении программы

		while (TRUE)
		{
            if (oHeader.ReadFromStream(pStream) == false )
			{
				break;
			}

			CRecordOfficeArtBlip art_blip;
			art_blip.m_strMemoryForder	= m_strMemoryForder;
			art_blip.m_oDocumentInfo	= &m_oDocumentInfo;

			art_blip.ReadFromStream(oHeader, pStream);

		}
		return TRUE;
	}
 
private: 
	bool						m_bDualStorage;
	CRecordCurrentUserAtom		m_oCurrentUser; 
	POLE::Stream *				m_pDocStream; 
	POLE::Stream *				m_pPictureStream; 
	POLE::Storage*				m_pPowerPointStg; 
    bool						m_bIsPPTFile;

public:	
	// для картинок
	CStringW					m_strMemoryForder;

    std::vector<bool>			m_arLoadImageFlags;
	DWORD						m_lImagesCount;

	CPPTDocumentInfo			m_oDocumentInfo;
};
