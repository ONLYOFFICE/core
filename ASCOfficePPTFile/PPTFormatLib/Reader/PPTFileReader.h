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

#include "../../../DesktopEditor/common/Directory.h"
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
    CPPTFileReader(POLE::Storage *pStg, std::wstring strTemp):
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
				
		if ( ReadCurrentUser(pStm) ) 
		{
			m_bIsPPTFile = TRUE; 
		}
		else
		{
			RELEASEOBJECT(pStm);
			
			std::string stream_name = std::string(PP97_DUALSTORAGE) + std::string("/") + std::string(CURRENT_USER_STREAM);
			pStm = new POLE::Stream( m_pPowerPointStg, stream_name);
			
			if (pStm == NULL)
				return;
			
			m_bDualStorage = true;
			if ( ReadCurrentUser(pStm))
			{
				m_bIsPPTFile = TRUE; 
			}
		}
		
		RELEASEOBJECT(pStm);

        if (0 == m_strMemoryForder.length())
		{
            m_strMemoryForder = NSDirectory::GetTempPath();
		}

        m_strMemoryForder = NSDirectory::CreateDirectoryWithUniqueName(m_strMemoryForder);

	}
	~CPPTFileReader()
	{
		RELEASEOBJECT(m_pDocStream); 
		RELEASEOBJECT(m_pPictureStream); 
	
		RELEASEOBJECT(m_pPowerPointStg);
		
        NSDirectory::DeleteDirectory(m_strMemoryForder);
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
		if (!pStm) return false;

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
    std::wstring				m_strMemoryForder;

    std::vector<bool>			m_arLoadImageFlags;
	DWORD						m_lImagesCount;

	CPPTDocumentInfo			m_oDocumentInfo;
};
