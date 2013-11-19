#pragma once
#include "PPTDocumentInfo.h"

#define CURRENT_USER_STREAM      L"Current User" 
#define DOCUMENT_STREAM          L"PowerPoint Document" 
#define PICTURE_STREAM           L"Pictures" 
 
class CPPTFileReader
{
public: 
	CPPTFileReader(IStorage *pStg, CStringW strTemp): 
		   m_pPowerPointStg(pStg),  
		   m_bIsPPTFile(FALSE), 
		   m_pDocStream(NULL), 
		   m_pPictureStream(NULL),
		   m_lImagesCount(0),
		   m_strMemoryForder(strTemp),
		   m_oDocumentInfo(),
		   m_arLoadImageFlags()
	{ 
		IStream *pStm = NULL; 
		m_pPowerPointStg->AddRef(); 
		HRESULT hr = pStg->OpenStream( CURRENT_USER_STREAM, NULL, STGM_READ | STGM_DIRECT | STGM_SHARE_EXCLUSIVE, NULL, &pStm ); 
		if (SUCCEEDED(hr) && ReadCurrentUser(pStm)) 
		  m_bIsPPTFile = TRUE; 
		if (NULL != pStm)
			pStm->Release(); 

		if (0 == m_strMemoryForder.GetLength())
		{
			CStringW sTempPath;
			GetTempPathW(4096, sTempPath.GetBuffer(4096));
			sTempPath.ReleaseBuffer();
			
			sTempPath += L"PPTMemory\\";

			#ifdef _DEBUG
			sTempPath = L"C:\\PPTMemory\\";
			#endif

			m_strMemoryForder = sTempPath;
			CreateDirectoryW(m_strMemoryForder, NULL);
		}
		else
		{
			if ('\\' != m_strMemoryForder[m_strMemoryForder.GetLength() - 1])
			{
				m_strMemoryForder += L"\\";
			}
		}
	}
	~CPPTFileReader()
	{
		m_pPowerPointStg->Release();
		// вообще - надо удалить директорию
		//RemoveDirectory(m_strMemoryForder);
		WIN32_FIND_DATA winFileData;
		HANDLE hFile;

		CString strFoundPreset = m_strMemoryForder + _T("\\*JPG");
		
		hFile = FindFirstFile(strFoundPreset, &winFileData); 
		if (hFile != INVALID_HANDLE_VALUE)
		{ 
			DeleteFile(winFileData.cFileName);
			while (FALSE != FindNextFile(hFile, &winFileData))
			{
				DeleteFile(winFileData.cFileName);
			}
			FindClose(hFile);
		}
	}

	BOOL IsPowerPoint() 
	{ 
		return m_bIsPPTFile;
	} 

	void ReadPersistDirectory()
	{
		// нужно вызывать РОВНО один раз...
		m_oDocumentInfo.ReadFromStream(&m_oCurrentUser, GetDocStream(), (CStringW)m_strMemoryForder);
	}

	void ReadSlideList()
	{
		// этот метод читает картинки.
		// сначала нужно заполнить m_arLoadImageFlags
		//CSimpleArray<CRecordBlipStoreEntry*> oArrayBSE;
		//m_oDocumentInfo.m_arUsers[0]->m_oDocument.GetRecordsByType(&oArrayBSE, true, false);

		// читаем картинки...
		BOOL bRes = SavePictures();

		DWORD nPID = m_oDocumentInfo.m_arUsers[0]->m_oUser.m_nDocumentRef;
		DWORD offset = m_oDocumentInfo.m_arUsers[0]->m_mapOffsetInPIDs.Lookup(nPID);

		StreamUtils::StreamSeek((long)offset, GetDocStream());
		
		if (PPT_DUMP_LOG)
		{
			ParseForSlideLists();
		}
	}

protected: 
	BOOL ReadCurrentUser(IStream *pStm)
	{
		ULONG nRd = 0; 
		SRecordHeader rh; 
		BOOL isPP = FALSE; 
		
		if(SUCCEEDED(pStm->Read(&rh, sizeof(rh), &nRd))) 
		{ 
			m_oCurrentUser.ReadFromStream(rh, pStm);
			isPP = (m_oCurrentUser.m_nSize == 0x00000014 && (m_oCurrentUser.m_nToken == NO_ENCRYPT) && 
							(m_oCurrentUser.m_nLenUserName <= 255)); 
		} 
 
		return isPP; 
	}

	IStream* GetDocStream()
	{ 
		if (m_pDocStream == NULL) 
		{ 
			if (!m_bIsPPTFile) 
				return NULL; 
			HRESULT hr = m_pPowerPointStg->OpenStream(DOCUMENT_STREAM, NULL, STGM_READ | STGM_DIRECT | STGM_SHARE_EXCLUSIVE, NULL, &m_pDocStream ); 
			if (FAILED(hr)) 
			{ 
				return NULL; 
			} 
		} 
		return m_pDocStream; 
 	}
	IStream* GetPictureStream()
	{ 
		if (m_pPictureStream == NULL) 
		{ 
			if (!m_bIsPPTFile) 
				return NULL; 
			HRESULT hr = m_pPowerPointStg->OpenStream(PICTURE_STREAM, NULL, STGM_READ | STGM_DIRECT | STGM_SHARE_EXCLUSIVE, NULL, &m_pPictureStream ); 
			if (FAILED(hr)) 
			{ 
				return NULL; 
			} 
		} 
		return m_pPictureStream; 
 	}


	DWORD ParseForSlideLists()
	{
		IStream *pStm = GetDocStream(); 
 
		SRecordHeader rh; 
		DWORD nRd = 0; 

		XmlUtils::CXmlWriter oWriter;
		oWriter.WriteNodeBegin(_T("PPTFile"));

		while (TRUE)
		{
			/////////////////////////////////////////////
			
			// здесь создаем xml

			pStm->Read(&rh, sizeof(rh), &nRd);
			if (0 >= nRd)
			{
				break;
			}

			rh.ToXmlWriter(&oWriter, pStm);
		}

		oWriter.WriteNodeEnd(_T("PPTFile"));

		CDirectory::SaveToFile(_T("C:\\PPT.xml"), oWriter.GetXmlString());
		//oWriter.SaveToFile(_T("C:\\PPT.xml"));
		return 0;
	}
	
	BOOL SavePictures()
	{
		IStream* pStream = GetPictureStream();
		if (NULL == pStream)
		{
			return FALSE;
		}
		
		SRecordHeader oHeader;
		ULONG nRd = 0;
		m_lImagesCount = 0;

		// теперь нужно удалить все картинки...
		WIN32_FIND_DATA winFileData;
		HANDLE hFile;

		CString strFoundPreset = m_strMemoryForder + _T("\\*JPG");
		
		hFile = FindFirstFile(strFoundPreset, &winFileData); 
		if (hFile != INVALID_HANDLE_VALUE)
		{ 
			DeleteFile(winFileData.cFileName);
			while (FALSE != FindNextFile(hFile, &winFileData))
			{
				DeleteFile(winFileData.cFileName);
			}
			FindClose(hFile);
		}
		
		CFile* pFile = new CFile();

		while (TRUE)
		{
			/*if (m_lImagesCount < m_oDocumentInfo.m_arEmptyPictures.GetCount())
			{
				if (m_oDocumentInfo.m_arEmptyPictures[m_lImagesCount])
				{
					++m_lImagesCount;
					continue;
				}
			}*/
			
			CMetaFileBuffer oMetaFile;
			
			pStream->Read(&oHeader, sizeof(oHeader), &nRd);
			if (0 >= nRd)
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

					oMetaFile.m_bIsValid = TRUE;
					
					CMetaHeader oMetaHeader;
					oMetaHeader.FromStream(pStream);
					
					ENHMETAHEADER3 oEmfHeader;
					oMetaHeader.ToEMFHeader(&oEmfHeader);
					
					//LONG lLenHeader = 88;
					//BYTE* pMetaHeader = new BYTE[lLenHeader];
					//memcpy(pMetaHeader, (void*)(&oEmfHeader), lLenHeader);
					//oMetaFile.SetHeader(pMetaHeader, lLenHeader);

					oMetaFile.SetHeader(NULL, 0);

					BYTE* pData = new BYTE[oHeader.RecLen - lOffset];
					pStream->Read(pData, oHeader.RecLen - lOffset, NULL); 

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

					oMetaFile.m_bIsValid = TRUE;
					
					CMetaHeader oMetaHeader;
					oMetaHeader.FromStream(pStream);
					WmfPlaceableFileHeader oWmfHeader;
					oMetaHeader.ToWMFHeader(&oWmfHeader);
					
					LONG lLenHeader = 22;
					BYTE* pMetaHeader = new BYTE[lLenHeader];
					memcpy(pMetaHeader, (void*)(&oWmfHeader), lLenHeader);

					oMetaFile.SetHeader(pMetaHeader, lLenHeader);

					BYTE* pData = new BYTE[oHeader.RecLen - lOffset];
					pStream->Read(pData, oHeader.RecLen - lOffset, NULL); 

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

					oMetaFile.m_bIsValid = TRUE;
					
					CMetaHeader oMetaHeader;
					oMetaHeader.FromStream(pStream);
					WmfPlaceableFileHeader oWmfHeader;
					oMetaHeader.ToWMFHeader(&oWmfHeader);
					
					LONG lLenHeader = 22;
					BYTE* pMetaHeader = new BYTE[lLenHeader];
					memcpy(pMetaHeader, (void*)(&oWmfHeader), lLenHeader);

					oMetaFile.SetHeader(pMetaHeader, lLenHeader);

					BYTE* pData = new BYTE[oHeader.RecLen - lOffset];
					pStream->Read(pData, oHeader.RecLen - lOffset, NULL); 

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
					break;
				}
				default:
				{
					break;
				}
				};

				if (oMetaFile.m_bIsValid)
				{
					CString strFile = CDirectory::ToString(++m_lImagesCount) + _T(".jpg");
				
					if (CDirectory::CreateFile((CString)m_strMemoryForder, strFile, pFile))
					{
						oMetaFile.ToFile(pFile);
					}
					continue;
				}
				
				BYTE* pImage = new BYTE[oHeader.RecLen - lOffset]; 
				//StreamUtils::StreamSkip(lOffset, pStream);

				pStream->Read(pImage, oHeader.RecLen - lOffset, NULL); 

				CString strFile = CDirectory::ToString(++m_lImagesCount) + _T(".jpg");
				
				if (CDirectory::CreateFile((CString)m_strMemoryForder, strFile, pFile))
				{
					if (RECORD_TYPE_ESCHER_BLIP_DIB == oHeader.RecType)
					{
						WORD vtType = 0x4D42;
						pFile->WriteFile((void*)&vtType, 2);
						DWORD dwLen = oHeader.RecLen - lOffset;
						pFile->WriteFile((void*)&dwLen, 4);
						DWORD dwRes = 0;
						pFile->WriteFile((void*)&dwRes, 4);
						DWORD dwOffset = 2;
						pFile->WriteFile((void*)&dwOffset, 4);
					}
					pFile->WriteFile((void*)pImage, oHeader.RecLen - lOffset);
				}

				delete[] pImage;
			}
			else
			{
				// этого быть не должно...
				continue;
			}
		}

		RELEASEOBJECT(pFile);
		return TRUE;
	}
 
private: 
	CRecordCurrentUserAtom		m_oCurrentUser; 
	IStream*					m_pDocStream; 
	IStream*					m_pPictureStream; 
	IStorage*					m_pPowerPointStg; 
	BOOL						m_bIsPPTFile; 

public:	
	// для картинок
	CStringW					m_strMemoryForder;

	CAtlArray<BOOL>				m_arLoadImageFlags;
	DWORD						m_lImagesCount;

public:
	CPPTDocumentInfo m_oDocumentInfo;
};
