#pragma once

#include "../../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"

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
			CMetaFileBuffer oMetaFile;

			CString sExt = L".jpg";
			
            if (oHeader.ReadFromStream(pStream) == false )
			{
				break;
			}

			if ((oHeader.RecVersion != PSFLAG_CONTAINER) && ((oHeader.RecVersion & 0x0F) != 0x0F))
			{
				long lOffset = 0;

				switch (oHeader.RecType)
				{
				case RECORD_TYPE_ESCHER_BLIP_EMF:
				{
					if (0x03D4 == oHeader.RecInstance)
					{
						lOffset = 16;
					}
					else if (0x03D5 == oHeader.RecInstance)
					{
						lOffset = 32;
					}
					StreamUtils::StreamSkip(lOffset, pStream);
					lOffset += 34;

					oMetaFile.m_bIsValid	= TRUE;
					oMetaFile.m_sExtension	= L".emf";
					
					CMetaHeader oMetaHeader;
					oMetaHeader.FromStream(pStream);
					
					Gdiplus::ENHMETAHEADER3 oEmfHeader;
					oMetaHeader.ToEMFHeader(&oEmfHeader);
					
					//LONG lLenHeader = 88;
					//BYTE* pMetaHeader = new BYTE[lLenHeader];
					//memcpy(pMetaHeader, (void*)(&oEmfHeader), lLenHeader);
					//oMetaFile.SetHeader(pMetaHeader, lLenHeader);

					oMetaFile.SetHeader(NULL, 0);

					BYTE* pData = new BYTE[oHeader.RecLen - lOffset];
					pStream->read(pData, oHeader.RecLen - lOffset); 

					oMetaFile.SetData(pData, oMetaHeader.cbSave, 
						oMetaHeader.cbSize, 0 == oMetaHeader.compression);

					if (0 == oMetaHeader.compression)
					{
						RELEASEARRAYOBJECTS(pData);
					}
					
					break;
				}
				case RECORD_TYPE_ESCHER_BLIP_WMF:
				{
					if (0x0216 == oHeader.RecInstance)
					{
						lOffset = 16;
					}
					else if (0x0217 == oHeader.RecInstance)
					{
						lOffset = 32;
					}
					StreamUtils::StreamSkip(lOffset, pStream);

					lOffset += 34;

					oMetaFile.m_bIsValid	= TRUE;
					oMetaFile.m_sExtension	= L".wmf";
					
					CMetaHeader oMetaHeader;
					oMetaHeader.FromStream(pStream);
					Gdiplus::WmfPlaceableFileHeader oWmfHeader;
					oMetaHeader.ToWMFHeader(&oWmfHeader);
					
					LONG lLenHeader = 22;
					BYTE* pMetaHeader = new BYTE[lLenHeader];
					memcpy(pMetaHeader, (void*)(&oWmfHeader), lLenHeader);

					oMetaFile.SetHeader(pMetaHeader, lLenHeader);

					BYTE* pData = new BYTE[oHeader.RecLen - lOffset];
					pStream->read(pData, oHeader.RecLen - lOffset); 

					oMetaFile.SetData(pData, oMetaHeader.cbSave, 
						oMetaHeader.cbSize, 0 == oMetaHeader.compression);

					if (0 == oMetaHeader.compression)
					{
						RELEASEARRAYOBJECTS(pData);
					}

					break;
				}
				case RECORD_TYPE_ESCHER_BLIP_PICT:
				{
					if (0x0542 == oHeader.RecInstance)
					{
						lOffset = 16;
					}
					else if (0x0543 == oHeader.RecInstance)
					{
						lOffset = 32;
					}
					StreamUtils::StreamSkip(lOffset, pStream);
					lOffset += 34;

					oMetaFile.m_bIsValid	= TRUE;
					oMetaFile.m_sExtension	= L".wmf";
					
					CMetaHeader oMetaHeader;
					oMetaHeader.FromStream(pStream);
					Gdiplus::WmfPlaceableFileHeader oWmfHeader;
					oMetaHeader.ToWMFHeader(&oWmfHeader);
					
					LONG lLenHeader = 22;
					BYTE* pMetaHeader = new BYTE[lLenHeader];
					memcpy(pMetaHeader, (void*)(&oWmfHeader), lLenHeader);

					oMetaFile.SetHeader(pMetaHeader, lLenHeader);

					BYTE* pData = new BYTE[oHeader.RecLen - lOffset];
					pStream->read(pData, oHeader.RecLen - lOffset); 

					oMetaFile.SetData(pData, oMetaHeader.cbSave, 
						oMetaHeader.cbSize, 0 == oMetaHeader.compression);

					if (0 == oMetaHeader.compression)
					{
						RELEASEARRAYOBJECTS(pData);
					}

					break;
				}
				case RECORD_TYPE_ESCHER_BLIP_JPEG:
				{
					if (0x046A == oHeader.RecInstance || 0x06E2 == oHeader.RecInstance)
					{
						lOffset = 17;
					}
					else if (0x046B == oHeader.RecInstance || 0x06E3 == oHeader.RecInstance)
					{
						lOffset = 33;
					}
					StreamUtils::StreamSkip(lOffset, pStream);
					
					sExt = _T(".jpg");
					break;
				}
				case RECORD_TYPE_ESCHER_BLIP_PNG:
				{
					if (0x06E0 == oHeader.RecInstance)
					{
						lOffset = 17;
					}
					else if (0x06E1 == oHeader.RecInstance)
					{
						lOffset = 33;
					}
					StreamUtils::StreamSkip(lOffset, pStream);
	
					sExt = _T(".png");
					break;
				}
				case RECORD_TYPE_ESCHER_BLIP_DIB:
				{
					if (0x07A8 == oHeader.RecInstance)
					{
						lOffset = 17;
					}
					else if (0x07A9 == oHeader.RecInstance)
					{
						lOffset = 33;
					}
					StreamUtils::StreamSkip(lOffset, pStream);
				
					sExt = _T(".bmp");
					break;
				}
				case RECORD_TYPE_ESCHER_BLIP_TIFF:
				{
					if (0x06E4 == oHeader.RecInstance)
					{
						lOffset = 17;
					}
					else if (0x06E5 == oHeader.RecInstance)
					{
						lOffset = 33;
					}
					StreamUtils::StreamSkip(lOffset, pStream);
					
					sExt = _T(".tif");
					break;
				}
				default:
				{
					break;
				}
				};

				if (oMetaFile.m_bIsValid)
				{
					CString strFile = CString(L"Image ") + CDirectory::ToString(++m_lImagesCount) + oMetaFile.m_sExtension;

                    CFile fileMeta;
                    HRESULT hr = fileMeta.CreateFile(m_strMemoryForder + FILE_SEPARATOR_STR + strFile);
				
                    if (hr == S_OK)
					{
                        oMetaFile.ToFile(&fileMeta);
                        fileMeta.CloseFile();
                    }
					m_oDocumentInfo.m_mapStoreImageFile[m_lImagesCount] = string2std_string(strFile);
					continue;
				}
				
				BYTE* pImage = new BYTE[oHeader.RecLen - lOffset]; 

				pStream->read(pImage, oHeader.RecLen - lOffset); 

				CString strFile = CString(L"Image ") + CDirectory::ToString(++m_lImagesCount) + sExt;
				
                CFile fileImage;
                HRESULT hr = fileImage.CreateFile(m_strMemoryForder+ FILE_SEPARATOR_STR +  strFile);
                if (hr == S_OK)
				{
					if (RECORD_TYPE_ESCHER_BLIP_DIB == oHeader.RecType)
					{
						WORD vtType = 0x4D42;
                        fileImage.WriteFile((void*)&vtType, 2);
						DWORD dwLen = oHeader.RecLen - lOffset;
                        fileImage.WriteFile((void*)&dwLen, 4);
						DWORD dwRes = 0;
                        fileImage.WriteFile((void*)&dwRes, 4);
						DWORD dwOffset = 2;
                        fileImage.WriteFile((void*)&dwOffset, 4);
					}
                    fileImage.WriteFile((void*)pImage, oHeader.RecLen - lOffset);
                    fileImage.CloseFile();
				}
				m_oDocumentInfo.m_mapStoreImageFile[m_lImagesCount] = string2std_string(strFile);

				delete[] pImage;
			}
			else
			{
				// этого быть не должно...
				m_lImagesCount++;
				continue;
			}
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
