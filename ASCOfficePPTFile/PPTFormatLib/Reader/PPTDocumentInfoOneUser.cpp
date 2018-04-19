﻿/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
#include <boost/make_shared.hpp>

#include "PPTDocumentInfo.h"

#include "../Records/ExMIDIAudioContainer.h"
#include "../Records/ExCDAudioContainer.h"
#include "../Records/ExWAVAudioLinkContainer.h"
#include "../Records/ExWAVAudioEmbeddedContainer.h"

#include "../Records/ExObjListContainer.h"
#include "../Records/SoundCollectionContainer.h"
#include "../Records/SoundContainer.h"

CPPTUserInfo::CPPTUserInfo() :	CDocument(),
								m_oUser(),
								m_mapOffsetInPIDs(), 
								m_oDocument(),
								m_mapMasters(),
								m_mapNotes(),
								m_mapSlides(),
								m_bEncrypt(false),
								m_pStorageDecrypt(NULL),
								m_pDecryptor(NULL),
								m_arOffsetPictures()
{
	m_VbaProjectStg			= NULL;
	m_pDocumentInfo			= NULL;
	m_lIndexThisUser		= -1;

	m_pNotesMasterWrapper	= NULL;
	m_pHandoutMasterWrapper	= NULL;
	
	m_nWriteSlideTimeOffset	=	0.0;
	m_nWriteSlideTime		=	0.0;

    m_bIsSetupEmpty			= false;

	m_bRtl					= false;
	m_bShowComments			= false;

	m_bHasDate				= false;
	m_bHasSlideNumber		= false;
	m_bHasFooter			= false;
	m_nFormatDate			= 1;

	m_current_elements		= NULL;
	m_current_level			= 0;
}

CPPTUserInfo::~CPPTUserInfo()
{
	Clear();
}

void CPPTUserInfo::Clear()
{
	CDocument::Clear();

	RELEASEOBJECT(m_pDecryptor);
	RELEASEOBJECT(m_pStorageDecrypt);
	RELEASEOBJECT(m_VbaProjectStg);
	
	for (std::map<_UINT32, CRecordSlide*>::iterator pPair = m_mapSlides.begin(); pPair != m_mapSlides.end(); ++pPair)
	{
		RELEASEINTERFACE(pPair->second);
	}
	m_mapSlides.clear();
	m_arrSlidesOrder.clear();

	for (std::map<_UINT32, CRecordSlide*>::iterator pPair = m_mapMasters.begin(); pPair != m_mapMasters.end(); ++pPair)
	{
		RELEASEINTERFACE(pPair->second);
	}
	m_mapMasters.clear();
	m_arrMastersOrder.clear();

	for (std::map<_UINT32, CRecordSlide*>::iterator pPair = m_mapNotesMasters.begin(); pPair != m_mapNotesMasters.end(); ++pPair)
	{
		RELEASEINTERFACE(pPair->second);
	}
	m_mapNotesMasters.clear();

	for (std::map<_UINT32, CRecordSlide*>::iterator pPair = m_mapHandoutMasters.begin(); pPair != m_mapHandoutMasters.end(); ++pPair)
	{
		RELEASEINTERFACE(pPair->second);
	}
	m_mapHandoutMasters.clear();

	RELEASEOBJECT(m_pNotesMasterWrapper);
	RELEASEOBJECT(m_pHandoutMasterWrapper);

	for (std::map<_UINT32, CRecordSlide*>::iterator pPair = m_mapNotes.begin(); pPair != m_mapNotes.end(); ++pPair)
	{
		RELEASEINTERFACE(pPair->second);
	}
	m_mapNotes.clear();
	m_arrNotesOrder.clear();

	m_oExMedia.Clear();
	m_arrFonts.clear();

	for (size_t nIndex = 0; nIndex < m_mapAnimations.size(); ++nIndex)
	{
		RELEASEOBJECT ( m_mapAnimations[nIndex]);
	}

	m_mapAnimations.clear();
	m_mapTransitions.clear ();
	m_arOffsetPictures.clear();
}

bool CPPTUserInfo::ReadFromStream(CRecordUserEditAtom* pUser, POLE::Stream* pStream)
{
	m_oUser.FromAtom(pUser);

//--------------------------------------------------------------------------------------------------
	SRecordHeader oHeader;
	CRecordUserEditAtom oUserAtom;

	StreamUtils::StreamSeek(m_oUser.m_nOffsetPersistDirectory, pStream);

	oHeader.ReadFromStream(pStream);

	if (RECORD_TYPE_PERSISTPTRINCREMENTALBLOCK != oHeader.RecType)
	{
        return false;
	}

	CRecordPersistDirectoryAtom oPersist;
	oPersist.ReadFromStream(oHeader, pStream);
	oPersist.ToMap(&m_mapOffsetInPIDs);
//--------------------------------------------------------------------------------------------------
	std::map<_UINT32, _UINT32>::iterator pPair = m_mapOffsetInPIDs.find(m_oUser.m_nEncryptRef);
	
	if (pPair != m_mapOffsetInPIDs.end())
	{
		StreamUtils::StreamSeek(pPair->second, pStream);
		oHeader.ReadFromStream(pStream);

		if (RECORD_TYPE_DOCUMENT_ENCRYPTION_ATOM  == oHeader.RecType)
		{
			m_bEncrypt = true;
			m_oEncryptionHeader.ReadFromStream(oHeader, pStream);
			
			m_pDecryptor = new CRYPT::ECMADecryptor();
			m_pDecryptor->SetCryptData(m_oEncryptionHeader.crypt_data_aes);

			if (m_strPassword.empty())
			{
				if (m_pDecryptor->SetPassword(L"VelvetSweatshop") == false)
					return false;
			}
			else if (m_pDecryptor->SetPassword(m_strPassword) == false)
			{
				return false;
			}
			std::wstring sTemp = m_strTmpDirectory + FILE_SEPARATOR_STR + L"~tempFile.ppt";
	
			m_pStorageDecrypt = new POLE::Storage(sTemp.c_str());		
			m_pStorageDecrypt->open(true, true);
		}
	}

	ReadDocumentPersists(pStream);
	return true;
}

void CPPTUserInfo::DecryptStream(POLE::Stream *pStream, int block)
{
	int size = pStream->size() - pStream->tell();
	
	POLE::Stream *pStreamTmp = new POLE::Stream(m_pStorageDecrypt, L"Tmp" + std::to_wstring(m_arStreamDecrypt.size() + 1), true, size);
	unsigned char* data_stream = new unsigned char[size]; 

	pStream->read(data_stream, size);
	m_pDecryptor->Decrypt((char*)data_stream, size, block);
	pStreamTmp->write(data_stream, size);
	pStreamTmp->flush();
	pStreamTmp->seek(0);
	
	m_arStreamDecrypt.push_back(CFStreamPtr(new CFStream(pStreamTmp)));
}

bool CPPTUserInfo::ReadDocumentPersists(POLE::Stream* pStream)
{
	SRecordHeader	oHeader;
	std::map<_UINT32, _UINT32>::iterator pPair = m_mapOffsetInPIDs.find(m_oUser.m_nDocumentRef);

	if (pPair == m_mapOffsetInPIDs.end())
        return false;

	_UINT32 offset_stream = pPair->second;

	StreamUtils::StreamSeek(offset_stream, pStream);

	POLE::Stream *	pStreamTmp = pStream;
	if (m_pDecryptor)
	{
		DecryptStream(pStream, m_oUser.m_nDocumentRef);
		pStreamTmp = m_arStreamDecrypt.back()->stream_;
	}
	oHeader.ReadFromStream(pStreamTmp);
	if (RECORD_TYPE_DOCUMENT != oHeader.RecType)
	{
		return false;
	}
	m_oDocument.ReadFromStream(oHeader, pStreamTmp);

	std::map<_UINT32, _UINT32>::iterator nIndexPsrRef;

	for (size_t index = 0; index < m_oDocument.m_arMasterPersists.size(); ++index)
	{
		nIndexPsrRef = m_mapOffsetInPIDs.find(m_oDocument.m_arMasterPersists[index].m_nPsrRef);
		
		if (nIndexPsrRef != m_mapOffsetInPIDs.end())
		{
			offset_stream = nIndexPsrRef->second;

			StreamUtils::StreamSeek(offset_stream, pStream);
			POLE::Stream *pStreamTmp = pStream;
			if (m_pDecryptor)
			{
				DecryptStream(pStream, m_oDocument.m_arMasterPersists[index].m_nPsrRef);
				pStreamTmp = m_arStreamDecrypt.back()->stream_;
			}
			oHeader.ReadFromStream(pStreamTmp);
			
			CRecordSlide* pSlide = new CRecordSlide();
			pSlide->ReadFromStream(oHeader, pStreamTmp);
			pSlide->m_oPersist = m_oDocument.m_arMasterPersists[index];

			pSlide->m_Index		= m_mapMasters.size();			
			m_mapMasters.insert(m_mapMasters.end(), std::pair<_UINT32, CRecordSlide*>(m_oDocument.m_arMasterPersists[index].m_nSlideID, pSlide));
			pSlide = NULL;
		}
		else
		{
			m_mapMasters.insert(m_mapMasters.end(), std::pair<_UINT32, CRecordSlide*>(m_oDocument.m_arMasterPersists[index].m_nSlideID, NULL));
		}
		m_arrMastersOrder.push_back(m_oDocument.m_arMasterPersists[index].m_nSlideID);
	}
	for (size_t index = 0; index < m_oDocument.m_arNotePersists.size(); ++index)
	{
		nIndexPsrRef = m_mapOffsetInPIDs.find(m_oDocument.m_arNotePersists[index].m_nPsrRef);
		
		if (m_mapOffsetInPIDs.end() != nIndexPsrRef)
		{
			offset_stream = nIndexPsrRef->second;
			StreamUtils::StreamSeek(offset_stream, pStream);
			
			POLE::Stream *pStreamTmp = pStream;
			if (m_pDecryptor)
			{
				DecryptStream(pStream, m_oDocument.m_arNotePersists[index].m_nPsrRef);
				pStreamTmp = m_arStreamDecrypt.back()->stream_;
			}
			oHeader.ReadFromStream(pStreamTmp);
			
			CRecordSlide* pSlide = new CRecordSlide();
			pSlide->ReadFromStream(oHeader, pStreamTmp);
			pSlide->m_oPersist = m_oDocument.m_arNotePersists[index];

			pSlide->m_Index		= m_mapNotes.size();			
			m_mapNotes.insert(std::pair<_UINT32, CRecordSlide*>(m_oDocument.m_arNotePersists[index].m_nSlideID, pSlide));
			pSlide = NULL;
		}
		else
		{
			m_mapNotes.insert(std::pair<_UINT32, CRecordSlide*>(m_oDocument.m_arNotePersists[index].m_nSlideID, NULL));
		}
		m_arrNotesOrder.push_back(m_oDocument.m_arNotePersists[index].m_nSlideID);
	}

	for (size_t index = 0; index < m_oDocument.m_arSlidePersists.size(); ++index)
	{
		nIndexPsrRef = m_mapOffsetInPIDs.find(m_oDocument.m_arSlidePersists[index].m_nPsrRef);
		
		if (m_mapOffsetInPIDs.end() != nIndexPsrRef)
		{
			offset_stream = (long)nIndexPsrRef->second;
			StreamUtils::StreamSeek(offset_stream, pStream);
			
			POLE::Stream *pStreamTmp = pStream;
			if (m_pDecryptor)
			{
				DecryptStream(pStream, m_oDocument.m_arSlidePersists[index].m_nPsrRef);
				pStreamTmp = m_arStreamDecrypt.back()->stream_;
			}
			oHeader.ReadFromStream(pStreamTmp);

			CRecordSlide* pSlide = new CRecordSlide();
			pSlide->ReadFromStream(oHeader, pStreamTmp);
			pSlide->m_oPersist	= m_oDocument.m_arSlidePersists[index];
			
			pSlide->m_Index		= m_mapSlides.size(); // in m_arrSlidesOrder			
			
			m_mapSlides.insert( std::pair<_UINT32, CRecordSlide*>(m_oDocument.m_arSlidePersists[index].m_nSlideID, pSlide ));

			if ( pSlide->m_bExistsTransition )
			{
				m_mapTransitions.insert (std::pair<_UINT32, CSlideShowSlideInfoAtom>( (_UINT32)index, pSlide->m_oSlideShowSlideInfoAtom ));
			}

			if ( pSlide->m_pSlideProgTagsContainer )
			{
				Animations::CSlideTimeLine* pEffects = pSlide->m_pSlideProgTagsContainer->GetTimeLine ();
				if (pEffects)
				{
					m_mapAnimations.insert(std::pair<_UINT32, Animations::CSlideTimeLine*>((_UINT32)index, pEffects));
				}
			}
		}
		else
		{
			m_mapSlides.insert( std::pair<_UINT32, CRecordSlide*>(m_oDocument.m_arSlidePersists[index].m_nSlideID, NULL));
		}
		m_arrSlidesOrder.push_back(m_oDocument.m_arSlidePersists[index].m_nSlideID);
	}
	std::vector<CRecordDocumentAtom*> oArrayDoc;
	m_oDocument.GetRecordsByType(&oArrayDoc, true, true);

	if (!oArrayDoc.empty())
	{
		nIndexPsrRef = m_mapOffsetInPIDs.find(oArrayDoc[0]->m_nNotesMasterPersistIDRef);
		
		if (m_mapOffsetInPIDs.end() != nIndexPsrRef)
		{
			offset_stream = nIndexPsrRef->second;
			StreamUtils::StreamSeek(offset_stream, pStream);
			
			POLE::Stream *pStreamTmp = pStream;
			if (m_pDecryptor)
			{
				DecryptStream(pStream, oArrayDoc[0]->m_nNotesMasterPersistIDRef);
				pStreamTmp = m_arStreamDecrypt.back()->stream_;
			}
			oHeader.ReadFromStream(pStreamTmp);

			CRecordSlide* pSlide = new CRecordSlide();
			pSlide->ReadFromStream(oHeader, pStreamTmp);
			pSlide->m_oPersist.m_nPsrRef = oArrayDoc[0]->m_nNotesMasterPersistIDRef;
			pSlide->m_Index		= 0;			
			
			m_mapNotesMasters.insert( std::pair<_UINT32, CRecordSlide*>(0, pSlide ));		
		}
		nIndexPsrRef = m_mapOffsetInPIDs.find(oArrayDoc[0]->m_nHandoutMasterPersistIDRef);
		
		if (m_mapOffsetInPIDs.end() != nIndexPsrRef)
		{
			offset_stream = nIndexPsrRef->second;
			StreamUtils::StreamSeek(offset_stream, pStream);
			
			POLE::Stream *pStreamTmp = pStream;
			if (m_pDecryptor)
			{
				DecryptStream(pStream, oArrayDoc[0]->m_nHandoutMasterPersistIDRef);
				pStreamTmp = m_arStreamDecrypt.back()->stream_;
			}
			oHeader.ReadFromStream(pStreamTmp);

			CRecordSlide* pSlide = new CRecordSlide();
			pSlide->ReadFromStream(oHeader, pStreamTmp);
			pSlide->m_oPersist.m_nPsrRef = oArrayDoc[0]->m_nHandoutMasterPersistIDRef;
			pSlide->m_Index		= 0;			
			
			m_mapHandoutMasters.insert( std::pair<_UINT32, CRecordSlide*>(0, pSlide ));		
		}
	}
	if (m_bMacros)
	{
		m_bMacros = false;
		std::vector<CRecordVBAInfoAtom*> oArrayVba;
		m_oDocument.GetRecordsByType(&oArrayVba, true, true);

		if (!oArrayVba.empty())
		{
			if (oArrayVba[0]->m_nHasMacros)
			{
				nIndexPsrRef = m_mapOffsetInPIDs.find(oArrayVba[0]->m_nObjStgDataRef);
				
				if (m_mapOffsetInPIDs.end() != nIndexPsrRef)
				{
					offset_stream = nIndexPsrRef->second;
					StreamUtils::StreamSeek(offset_stream, pStream);

					POLE::Stream *pStreamTmp = pStream;
					if (m_pDecryptor)
					{
						DecryptStream(pStream, oArrayVba[0]->m_nObjStgDataRef);
						pStreamTmp = m_arStreamDecrypt.back()->stream_;
					}
					oHeader.ReadFromStream(pStreamTmp);

					m_VbaProjectStg = new CRecordVbaProjectStg(m_strTmpDirectory);
					m_VbaProjectStg->ReadFromStream(oHeader, pStreamTmp);

					if (m_VbaProjectStg->m_sFileName.empty())
					{
						RELEASEOBJECT(m_VbaProjectStg);
					}
					else
					{
						m_sVbaProjectFile = m_VbaProjectStg->m_sFileName;
						m_bMacros = true;
					}
				}
			}
		}
	}
	return true;
}
//--------------------------------------------------------------------------------------------
void CPPTUserInfo::ReadExtenalObjects(std::wstring strFolderMem)
{
	// так... теперь берем всю инфу о ExObject -----------------------------
	m_oExMedia.m_strPresentationDirectory	= strFolderMem;

	NSPresentationEditor::CExFilesInfo oInfo;

	oInfo.m_strFilePath = m_oExMedia.m_strPresentationDirectory;
	oInfo.m_dwID		= 0xFFFFFFFF;
	m_oExMedia.m_arImages.push_back(oInfo);

	// читаем все внешние объекты
	std::vector<CRecordExObjListContainer*> oArrayExObjects;
	m_oDocument.GetRecordsByType(&oArrayExObjects, false, true);

	if (0 != oArrayExObjects.size())
	{
		LoadExternal(oArrayExObjects[0]);
	}
	else
	{
		// прочитать все равно надо, так как там может быть
		// транзишн
		LoadExternal(NULL);
	}

	// -------------------------------------------------------------------

	// а теперь заберем всю информацию о шрифтах
	std::vector<CRecordFontEntityAtom*> oArrayFonts;
	m_oDocument.GetRecordsByType(&oArrayFonts, true);

	for (size_t nIndex = 0; nIndex < oArrayFonts.size(); ++nIndex)
	{
		CFont oFont;
		oFont.Name		= oArrayFonts[nIndex]->m_strFaceName;
		oFont.Charset	= oArrayFonts[nIndex]->m_lfCharSet;

		oFont.PitchFamily = oArrayFonts[nIndex]->m_lfPitchAndFamily;

		m_arrFonts.push_back(oFont);
	}
	m_oDocument.GetRecordsByType(&m_arrBlipStore, true, true);
	if (0 < m_arrBlipStore.size())
	{
		m_bIsSetupEmpty = TRUE;
		m_arrBlipStore[0]->SetUpPicturesInfos(&m_arOffsetPictures);
	}
}

void CPPTUserInfo::FromDocument()
{
	m_arSlides.clear();

	std::vector<CRecordDocumentAtom*> oArrayDoc;
	m_oDocument.GetRecordsByType(&oArrayDoc, true, true);

	if (0 == oArrayDoc.size())
		return;

	std::vector<CRecordDocumentTextInfo*> oArrayInfo;
	m_oDocument.GetRecordsByType(&oArrayInfo, false, true);

	if (0 != oArrayInfo.size())
	{
		std::vector<CRecordTextMasterStyleAtom*> oStyles;
		oArrayInfo[0]->GetRecordsByType(&oStyles, false, false);

		if (0 != oStyles.size())
			m_oDefaultTextStyle.SetStyles((NSPresentationEditor::CTextStyles*)oStyles[0]);		

		std::vector<CRecordTextSIExceptionAtom*> oSI;
		oArrayInfo[0]->GetRecordsByType(&oSI, false, false);

		if (0 != oSI.size())
		{
			if (oSI[0]->m_oSIRun.bLang)
				m_wLanguage = oSI[0]->m_oSIRun.Lang;

			m_oDefaultTextStyle.SetLanguage(m_wLanguage);
		}
	}
	std::vector<CRecordHeadersFootersContainer*> oArrayHeadersFootersInfo;
	m_oDocument.GetRecordsByType(&oArrayHeadersFootersInfo, false, false); 

	if (0 != oArrayHeadersFootersInfo.size())
	{
		if (oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom)
		{
			m_bHasDate			=	oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasDate ||
									oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasTodayDate || 
									oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasUserDate;
			m_bHasFooter		=	oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasFooter;
			m_bHasSlideNumber	=	oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasSlideNumber;

			if (oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasUserDate)	m_nFormatDate = 2;
		}
		for (int i = 0 ; i < 3; i++) m_PlaceholdersReplaceString[i] = oArrayHeadersFootersInfo[0]->m_HeadersFootersString[i];
	}

	double master_to_emu = 1./576. ;//inch

	master_to_emu *= 72;//pt
	master_to_emu *= 12700;//emu

	m_lSlideWidth	= (oArrayDoc[0]->m_oSlideSize.X * 1587.5 + 0.5);
	m_lSlideHeight	= (oArrayDoc[0]->m_oSlideSize.Y * 1587.5 + 0.5);

	m_lNotesWidth	= (oArrayDoc[0]->m_oNotesSize.X * 1587.5 + 0.5);
	m_lNotesHeight	= (oArrayDoc[0]->m_oNotesSize.Y * 1587.5 + 0.5);

	m_bRtl			= (oArrayDoc[0]->m_bRightToLeft!=0);
	m_bShowComments	= (oArrayDoc[0]->m_bShowComments!=0);

	LoadMasters();

	double DurationSlide = PPT_DEFAULT_SLIDE_DURATION;

	m_arSlides.reserve(m_arrSlidesOrder.size());
	for (size_t i = 0; i < m_arrSlidesOrder.size(); i++)
	{
		std::map<_UINT32, CRecordSlide*>::iterator pPair = m_mapSlides.find(m_arrSlidesOrder[i]);
		
		if (pPair ==  m_mapSlides.end()) 
			continue;

		LoadSlideFromPrevUsers	( pPair->first );

		DurationSlide	=	PPT_DEFAULT_SLIDE_DURATION;

		m_arSlides.push_back(new CSlide());

		// если на слайде есть анимации
		std::map <_UINT32, Animations::CSlideTimeLine*>::iterator pTimeLine =	m_mapAnimations.find( pPair->first);

		if ( m_mapAnimations.end() != pTimeLine )
		{
			if ( (pTimeLine->second)  && (0.0 != pTimeLine->second->GetTime () ))
			{
				DurationSlide	=	pTimeLine->second->GetTime ();
			}
		}

		CSlide* pSlide = m_arSlides.back();

		pSlide->m_dStartTime		= 0.0;
		pSlide->m_dEndTime			= DurationSlide;
		pSlide->m_dDuration			= DurationSlide;

		LoadSlide ( pPair->first, pSlide);
	}

	m_arNotes.reserve(m_arrNotesOrder.size());
	for (size_t i = 0; i< m_arrNotesOrder.size(); i++)
	{
		std::map<_UINT32, CRecordSlide*>::iterator pPair = m_mapNotes.find(m_arrNotesOrder[i]);
		
		if (pPair ==  m_mapNotes.end()) 
			continue;

		LoadNotesFromPrevUsers	( pPair->first );

		DurationSlide	=	PPT_DEFAULT_SLIDE_DURATION;

		m_arNotes.push_back(new CSlide());

		CSlide* pSlide = m_arNotes.back();

		LoadNotes ( pPair->first, pSlide);
	}
	
	CalculateEditor();
}

void CPPTUserInfo::LoadNotes(_UINT32 dwNoteID, CSlide* pNotes)
{
	std::map<_UINT32, CRecordSlide*>::iterator pPairNotes = m_mapNotes.find(dwNoteID);

	if (pPairNotes == m_mapNotes.end()) return;
	
	CRecordSlide* pRecordSlide = pPairNotes->second;

	if (NULL == pRecordSlide) return;

	pNotes->m_bUseLayoutColorScheme = true;

	CSlideInfo slide_info;
	m_arNotesWrapper.push_back(slide_info);
	
	CSlideInfo* pNotesWrapper = &m_arNotesWrapper.back();

	int indexUser = pRecordSlide->m_IndexUser;

	if (m_pDocumentInfo->m_arUsers[indexUser]->m_arOffsetPictures.empty())
		pNotesWrapper->m_parEmptyPictures	= &m_pDocumentInfo->m_arUsers[0]->m_arOffsetPictures;
	else
		pNotesWrapper->m_parEmptyPictures	= &m_pDocumentInfo->m_arUsers[indexUser]->m_arOffsetPictures;

	pNotesWrapper->m_mapFilePictures	= &m_pDocumentInfo->m_mapStoreImageFile;
	pNotesWrapper->m_arTextPlaceHolders = pRecordSlide->m_oPersist.m_arTextAttrs;

	std::vector<CRecordNotesAtom*> oArrayNotesAtoms;
	pRecordSlide->GetRecordsByType(&oArrayNotesAtoms, false, true);
	if (0 == oArrayNotesAtoms.size())
	{
		// ошибка!!!
		return;
	}
    bool bMasterColorScheme = oArrayNotesAtoms[0]->m_bMasterScheme;
    bool bMasterBackGround	= oArrayNotesAtoms[0]->m_bMasterBackground;
    bool bMasterObjects		= oArrayNotesAtoms[0]->m_bMasterObjects;

	std::map<_UINT32, CRecordSlide*>::iterator pPairSlide = m_mapSlides.find(oArrayNotesAtoms[0]->m_nSlideIDRef);

	if (pPairSlide == m_mapSlides.end())
	{
		//????? у заметок нет слайда !!!
	}
//-----------------------------------------------------
	size_t index = pPairSlide->second->m_Index;
	if (index >= m_arSlides.size())
		return;
	CSlide* pSlide		= m_arSlides[pPairSlide->second->m_Index];

	pNotes->m_lSlideID	= pPairSlide->second->m_Index;
	pSlide->m_lNotesID	= m_arNotes.size() - 1;
//-----------------------------------------------------

	CTheme		* pTheme		= m_pNotesMaster.get();
	CSlideInfo	* pThemeWrapper	= m_pNotesMasterWrapper;

	CLayout* pLayout	= NULL;
//-----------------------------------------------------
	std::vector<NSPresentationEditor::CColor>* pArrayColorScheme = pTheme ? &pTheme->m_arColorScheme : NULL;

// читаем цветовую схему -----------------------------------------------------------
	pNotes->m_bUseLayoutColorScheme = true;
	if (!bMasterColorScheme)
	{
		std::vector<CRecordColorSchemeAtom*> oArrayColors;
		pRecordSlide->GetRecordsByType(&oArrayColors, false);

		for (size_t i = 0; i < oArrayColors.size(); ++i)
		{
			if (0x01 == oArrayColors[i]->m_oHeader.RecInstance)
			{
				pNotes->m_bUseLayoutColorScheme = false;
				oArrayColors[i]->ToArray(&pNotes->m_arColorScheme);
				CorrectColorScheme(pNotes->m_arColorScheme);

				// проверим на совпадение
				size_t nCountC = pNotes->m_arColorScheme.size();
				size_t nIndexC = 0;
				if (pArrayColorScheme && nCountC == pArrayColorScheme->size())
				{
					for (; nIndexC < nCountC; ++nIndexC)
					{
						if (pNotes->m_arColorScheme[i].IsEqual(pArrayColorScheme->at(i)))
							break;
					}
				}

				if (nIndexC == nCountC)
				{
					pNotes->m_bUseLayoutColorScheme = true;
					pNotes->m_arColorScheme.clear();
				}

				break;
			}
		}
	}
//------------------------------------------------------------------------------------
	bool	bHasDate		= false;
	bool	bHasSlideNumber = false;
	bool	bHasFooter		= false;
	int		nFormatDate		= 1;	
	
	std::vector<CRecordHeadersFootersContainer*> oArrayHeadersFootersInfo;
	pRecordSlide->GetRecordsByType(&oArrayHeadersFootersInfo, true, false);

	if (!oArrayHeadersFootersInfo.empty())
	{
		if (oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom)
		{
			bHasDate		=	oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasDate ||
								oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasTodayDate || 
								oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasUserDate;
			bHasFooter		=	oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasFooter;
			bHasSlideNumber	=	oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasSlideNumber;

			if (oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasUserDate)	nFormatDate = 2;
		}
		for (int i = 0 ; i < 3; i++) 
			pNotes->m_PlaceholdersReplaceString[i] = oArrayHeadersFootersInfo[0]->m_HeadersFootersString[i];
	}
	pNotes->m_bIsBackground		= false;
	
//-------------------------------------------------------------------------------------------------------
	std::vector<CRecordDrawingContainer*> oArrayDrawing;

	pRecordSlide->GetRecordsByType(&oArrayDrawing, true);

	m_current_level = 0;
	m_current_elements = &pNotes->m_arElements;
	
	if (!oArrayDrawing.empty())
	{
		for (size_t nIndex = 0; nIndex < oArrayDrawing[0]->m_arRecords.size(); ++nIndex)
		{
			CRecordGroupShapeContainer* pGroup = dynamic_cast<CRecordGroupShapeContainer*>(oArrayDrawing[0]->m_arRecords[nIndex]);

			if (pGroup)
			{
				LoadGroupShapeContainer(pGroup, NULL, pTheme, pLayout, pThemeWrapper, pNotesWrapper, pNotes);
			}
			else
			{
				CRecordShapeContainer* pShapeGroup = dynamic_cast<CRecordShapeContainer*>(oArrayDrawing[0]->m_arRecords[nIndex]);
				if (pShapeGroup)
				{
					CElementPtr pElement = pShapeGroup->GetElement(false, &m_oExMedia,
												pTheme, pLayout, pThemeWrapper, pNotesWrapper, pNotes);
					
					CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElement.get());
					if (NULL != pShape && pShape->m_bIsBackground && !pShape->m_bHaveAnchor)
					{
						pShape->SetupProperties(pNotes, pTheme, pLayout);

						pNotes->m_bIsBackground = true;
						pNotes->m_oBackground	= pShape->m_oBrush;
					}
				}
			}			
		}
	}
}


void CPPTUserInfo::LoadSlide(_UINT32 dwSlideID, CSlide* pSlide)
{
	std::map<_UINT32, CRecordSlide*>::iterator pPairSlide = m_mapSlides.find(dwSlideID);

	if (pPairSlide == m_mapSlides.end()) return;
	
	CRecordSlide* pRecordSlide = pPairSlide->second;

	if (NULL == pRecordSlide) return;

	// транзишн

	CTransition* pTransition = &pSlide->m_oSlideShow.m_oTransition;

	if (pRecordSlide->m_bExistsTransition)
	{
		CSlideShowSlideInfoAtom* pAtom	= &pRecordSlide->m_oSlideShowSlideInfoAtom;

		pTransition->m_bAudioPresent	= pAtom->m_bSound;

		NSPresentationEditor::CExFilesInfo* pInfo	= m_oExMedia.LockAudioFromCollection(pAtom->m_nSoundRef);
		if (NULL != pInfo)
		{
			pTransition->m_oAudio.m_strAudioFileName = pInfo->m_strFilePath;
		}

		pTransition->m_bLoopSound		= pAtom->m_bLoopSound;
		pTransition->m_bStopSound		= pAtom->m_bStopSound;
		pTransition->m_nEffectDirection = pAtom->m_nEffectDirection;
		pTransition->m_nEffectType		= pAtom->m_nEffectType;  // тут нужно сконвертить тип
		pTransition->m_nSpeed			= pAtom->m_nSpeed;

		pSlide->m_oSlideShow.m_dSlideDuration = pAtom->m_nSlideTime;
	}

	CSlideShowSlideInfoAtom* pAtom	= &pRecordSlide->m_oSlideShowSlideInfoAtom;
	if (pAtom->m_bSound)
	{
		NSPresentationEditor::CExFilesInfo* pInfo	= m_oExMedia.LockAudioFromCollection(pAtom->m_nSoundRef);
		if (NULL != pInfo)
			AddAudioTransition (dwSlideID, pTransition, pInfo->m_strFilePath);
	}

	// анимации

	pSlide->m_bUseLayoutColorScheme = true;

	CSlideInfo slide_info;
	m_arSlideWrapper.push_back(slide_info);
	
	CSlideInfo* pSlideWrapper = &m_arSlideWrapper.back();

	int indexUser = pRecordSlide->m_IndexUser;

	if (m_pDocumentInfo->m_arUsers[indexUser]->m_arOffsetPictures.empty())
		pSlideWrapper->m_parEmptyPictures	= &m_pDocumentInfo->m_arUsers[0]->m_arOffsetPictures;
	else
		pSlideWrapper->m_parEmptyPictures	= &m_pDocumentInfo->m_arUsers[indexUser]->m_arOffsetPictures;
	
	pSlideWrapper->m_mapFilePictures	= &m_pDocumentInfo->m_mapStoreImageFile;
	pSlideWrapper->m_arTextPlaceHolders = pRecordSlide->m_oPersist.m_arTextAttrs;

	// записываем шрифты
	std::vector<CRecordSlideAtom*> oArraySlideAtoms;
	pRecordSlide->GetRecordsByType(&oArraySlideAtoms, false, true);
	if (0 == oArraySlideAtoms.size())
	{
		// ошибка!!!
		return;
	}
    bool bMasterColorScheme = oArraySlideAtoms[0]->m_bMasterScheme;
    bool bMasterBackGround	= oArraySlideAtoms[0]->m_bMasterBackground;
    bool bMasterObjects		= oArraySlideAtoms[0]->m_bMasterObjects;

	std::map<_UINT32, LONG>::iterator pPairTheme = m_mapMasterToTheme.find(oArraySlideAtoms[0]->m_nMasterIDRef);

	if (pPairTheme == m_mapMasterToTheme.end())
	{
		//????? слайду не присвоена тема !!!
		pPairTheme = m_mapMasterToTheme.begin();
	}
//-----------------	
	pSlide->m_lThemeID			= pPairTheme->second;
	
	CTheme		* pTheme		= m_arThemes		[pSlide->m_lThemeID].get();
	CSlideInfo	* pThemeWrapper	= &m_arMasterWrapper[pSlide->m_lThemeID];

	CLayout* pLayout	= NULL;

	std::map<_UINT32, LONG>::iterator		pPairLayoutTitle	= pTheme->m_mapTitleLayout.find(oArraySlideAtoms[0]->m_nMasterIDRef);
	if (pPairLayoutTitle != pTheme->m_mapTitleLayout.end())
	{
		//основан на заголовочном шаблоне
		pSlide->m_bShowMasterShapes = bMasterObjects;
		pSlide->m_lLayoutID			= pPairLayoutTitle->second;
		pLayout						= pTheme->m_arLayouts[pSlide->m_lLayoutID].get();
	}
	else
	{
		pSlide->m_bShowMasterShapes			= true; //???
		//основан на типовом шаблоне
		std::map<_UINT64, LONG>::iterator	pPairLayoutGeom	= pTheme->m_mapGeomToLayout.find(oArraySlideAtoms[0]->m_oLayout.m_hash);
		
		if (pPairLayoutGeom == pTheme->m_mapGeomToLayout.end())
		{
			pSlide->m_lLayoutID = AddNewLayout(pTheme, pRecordSlide, true, bMasterObjects);

			pLayout				= pTheme->m_arLayouts[pSlide->m_lLayoutID].get();
			pLayout->m_bShowMasterShapes	= true;
		}
		else
		{
			pSlide->m_lLayoutID = pPairLayoutGeom->second;
			pLayout				= pTheme->m_arLayouts[pSlide->m_lLayoutID].get();
		}
	}

	std::vector<NSPresentationEditor::CColor>* pArrayColorScheme = &pTheme->m_arColorScheme;
	if (!pLayout->m_bUseThemeColorScheme)
		pArrayColorScheme = &pLayout->m_arColorScheme;

	// читаем цветовую схему -----------------------------------------------------------
	pSlide->m_bUseLayoutColorScheme = true;
	if (!bMasterColorScheme)
	{
		std::vector<CRecordColorSchemeAtom*> oArrayColors;
		pRecordSlide->GetRecordsByType(&oArrayColors, false);

		for (size_t i = 0; i < oArrayColors.size(); ++i)
		{
			if (0x01 == oArrayColors[i]->m_oHeader.RecInstance)
			{
				pSlide->m_bUseLayoutColorScheme = false;
				oArrayColors[i]->ToArray(&pSlide->m_arColorScheme);
				CorrectColorScheme(pSlide->m_arColorScheme);

				// проверим на совпадение
				size_t nCountC = pSlide->m_arColorScheme.size();
				size_t nIndexC = 0;
				if (pArrayColorScheme && nCountC == pArrayColorScheme->size())
				{
					for (; nIndexC < nCountC; ++nIndexC)
					{
						if (pSlide->m_arColorScheme[i].IsEqual(pArrayColorScheme->at(i)))
							break;
					}
				}

				if (nIndexC == nCountC)
				{
					pSlide->m_bUseLayoutColorScheme = true;
					pSlide->m_arColorScheme.clear();
				}

				break;
			}
		}
	}
//------------------------------------------------------------------------------------
	bool	bHasDate		= false;
	bool	bHasSlideNumber = false;
	bool	bHasFooter		= false;
	int		nFormatDate		= 1;	
	
	std::vector<CRecordHeadersFootersContainer*> oArrayHeadersFootersInfo;
	pRecordSlide->GetRecordsByType(&oArrayHeadersFootersInfo, true, false);

	if (!oArrayHeadersFootersInfo.empty())
	{
		if (oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom)
		{
			bHasDate		=	oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasDate ||
								oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasTodayDate || 
								oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasUserDate;
			bHasFooter		=	oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasFooter;
			bHasSlideNumber	=	oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasSlideNumber;

			if (oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasUserDate)	nFormatDate = 2;
		}
		for (int i = 0 ; i < 3; i++) pSlide->m_PlaceholdersReplaceString[i] = oArrayHeadersFootersInfo[0]->m_HeadersFootersString[i];
	}
	else
	{
		bHasDate		= pLayout->m_bHasDate;
		bHasFooter		= pLayout->m_bHasFooter;
		bHasSlideNumber	= pLayout->m_bHasSlideNumber;
		nFormatDate		= pLayout->m_nFormatDate;

		for (int i = 0 ; i < 3; i++) pSlide->m_PlaceholdersReplaceString[i] = pLayout->m_PlaceholdersReplaceString[i];
	}
//-------------------------------------------------------------------------------------------------------
	std::vector<CRecordCString*> oArrayStrings;
	pRecordSlide->GetRecordsByType(&oArrayStrings, false, false);
	
	for (size_t i = 0; i < oArrayStrings.size(); i++)
	{
		if (oArrayStrings[i]->m_oHeader.RecType == 0x0fba)
		{
			pSlide->m_sName = oArrayStrings[i]->m_strText;
		}
	}
	pSlide->m_bIsBackground = false;
//-------------------------------------------------------------------------------------------------------
	std::vector<CRecordDrawingContainer*> oArrayDrawing;
	pRecordSlide->GetRecordsByType(&oArrayDrawing, true);

	m_current_level = 0;
	m_current_elements = &pSlide->m_arElements;
	
	if (!oArrayDrawing.empty())
	{
		for (size_t nIndex = 0; nIndex < oArrayDrawing[0]->m_arRecords.size(); ++nIndex)
		{
			CRecordGroupShapeContainer* pGroup = dynamic_cast<CRecordGroupShapeContainer*>(oArrayDrawing[0]->m_arRecords[nIndex]);

			if (pGroup)
			{
				LoadGroupShapeContainer(pGroup, NULL, pTheme, pLayout, pThemeWrapper, pSlideWrapper, pSlide);
			}
			else
			{
				CRecordShapeContainer* pShapeGroup = dynamic_cast<CRecordShapeContainer*>(oArrayDrawing[0]->m_arRecords[nIndex]);
				if (pShapeGroup)
				{
					CElementPtr pElement = pShapeGroup->GetElement(false, &m_oExMedia,
															pTheme, pLayout, pThemeWrapper, pSlideWrapper, pSlide);
					
					if (pElement->m_bIsBackground && !pElement->m_bHaveAnchor && !bMasterBackGround)
					{
						CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElement.get());
						if (NULL != pShape && pSlide)
						{
							pShape->SetupProperties(pSlide, pTheme, pLayout);

							pSlide->m_bIsBackground = true;
							pSlide->m_oBackground	= pShape->m_oBrush;
						}
					}
				}
			}			
		}
	}
	std::multimap<int, int>::iterator it;
		
	if (bHasSlideNumber)	AddLayoutSlidePlaceholder(pSlide, MasterSlideNumber, pLayout, true);	
	
	if (bHasDate)
	{
		CElementPtr pElement = AddLayoutSlidePlaceholder(pSlide, MasterDate, pLayout, true);
	
		if (pElement) pElement->m_nFormatDate = nFormatDate;
	}

	if (bHasFooter)	AddLayoutSlidePlaceholder(pSlide, MasterFooter, pLayout, true);
}

void CPPTUserInfo::LoadGroupShapeContainer(CRecordGroupShapeContainer* pGroupContainer, std::vector<CElementPtr>* pParentElements, CTheme* pTheme, CLayout* pLayout,												
											CSlideInfo* pThemeWrapper, CSlideInfo* pSlideWrapper, CSlide* pSlide)
{
	if (!pGroupContainer) return;
	if (pGroupContainer->m_arRecords.empty()) return;

	CRecordShapeContainer* pShapeGroup = dynamic_cast<CRecordShapeContainer*>(pGroupContainer->m_arRecords[0]);
	
	CElementPtr pElement;
	if (pShapeGroup)
	{
		pShapeGroup->bGroupShape = true;

		pElement = pShapeGroup->GetElement(m_current_level > 1, &m_oExMedia, pTheme, pLayout, pThemeWrapper, pSlideWrapper, pSlide);
	}

	if (!pElement) return;
	pElement->m_pParentElements = pParentElements;
	m_current_elements->push_back(pElement);
	
	m_current_level++;
	
	pParentElements = m_current_elements;
	m_current_elements = &pElement->m_pChildElements;

	for (size_t i = 1; i < pGroupContainer->m_arRecords.size(); i++)
	{
		CRecordGroupShapeContainer* pGroup = dynamic_cast<CRecordGroupShapeContainer*>(pGroupContainer->m_arRecords[i]);

		if (pGroup)
		{
			LoadGroupShapeContainer(pGroup, pParentElements,  pTheme, pLayout, pThemeWrapper, pSlideWrapper, pSlide);
		}
		else
		{
			pShapeGroup = dynamic_cast<CRecordShapeContainer*>(pGroupContainer->m_arRecords[i]);
			if (pShapeGroup)
			{
				CElementPtr pElement = pShapeGroup->GetElement(m_current_level > 1, &m_oExMedia, pTheme, pLayout, pThemeWrapper, pSlideWrapper, pSlide);
				
				CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElement.get());
				
				if (NULL != pElement)
				{
					pElement->m_pParentElements = pParentElements;

					//	AddAnimation ( dwSlideID, pSlide->m_lOriginalWidth, pSlide->m_lOriginalHeight, pElement );

					if (NULL != pShape)
						pShape->SetupProperties(pSlide, pTheme, pLayout);

					if ( pElement->m_lPlaceholderType > 0)
					{
						if (pSlide)
						{
							pSlide->m_mapPlaceholders.insert(std::make_pair(pElement->m_lPlaceholderType, pElement)); 
						}
						else if (pTheme)
						{
							pTheme->m_mapPlaceholders.insert(std::make_pair(pElement->m_lPlaceholderType, pElement)); 
						}
						else
						{
							if (pElement->m_lPlaceholderID >= 0)
							{
								if (pElement->m_lPlaceholderType == MasterSlideNumber)	pLayout->m_bHasSlideNumber	= true;
								if (pElement->m_lPlaceholderType == MasterDate)			pLayout->m_bHasDate			= true;
								if (pElement->m_lPlaceholderType == MasterFooter)		pLayout->m_bHasFooter		= true;
							}
							pLayout->m_mapPlaceholders.insert(std::make_pair(pElement->m_lPlaceholderType, pElement)); 
						}
					}
					if (pElement->m_bHaveAnchor)
					{
						m_current_elements->push_back(pElement);
					}
				}
			}
		}
	}

	m_current_level--;
	
	if (false == m_current_elements->empty())
	{
		m_current_elements =  m_current_elements->front()->m_pParentElements;
	}
}
CElementPtr CPPTUserInfo::AddLayoutSlidePlaceholder (CSlide *pSlide, int placeholderType, CLayout *pLayout, bool idx_only)
{
	CElementPtr pElement;
	
	for (std::multimap<int, CElementPtr>::iterator it = pLayout->m_mapPlaceholders.begin(); it != pLayout->m_mapPlaceholders.end(); ++it)
	{
		CElementPtr pElementPlaceholder = it->second;
		if (it->first == placeholderType )
		{
			if (idx_only == false)
			{
				if (pElementPlaceholder->m_lPlaceholderID >= 0 ) continue;

				pElement = pElementPlaceholder->CreateDublicate();
				
				pSlide->m_arElements.push_back(pElement);
				pSlide->m_mapPlaceholders.insert(std::make_pair(placeholderType, pElement)); 
			}
			else
			{
				if (pElementPlaceholder->m_lPlaceholderID < 0 ) continue;

				for (std::multimap<int, CElementPtr>::iterator it1 = pSlide->m_mapPlaceholders.begin(); it1 != pSlide->m_mapPlaceholders.end(); it1++)
				{
					if (it1->first == placeholderType && (it1->second)->m_lPlaceholderID >= 0)
					{
						pElement = it1->second;
						break;
					}
				}
				if (!pElement)
				{
					pElement = pElementPlaceholder->CreateDublicate();

					pSlide->m_arElements.push_back(pElement);
					pSlide->m_mapPlaceholders.insert(std::make_pair(placeholderType, pElement)); 
				}
			}
		}
	}

	return pElement; 
}
	
CElementPtr  CPPTUserInfo::AddThemeLayoutPlaceholder (CLayout *pLayout, int placeholderType, CTheme* pTheme, bool idx_only)
{
	CElementPtr pElement;

	for (std::multimap<int, CElementPtr>::iterator it = pTheme->m_mapPlaceholders.begin(); it != pTheme->m_mapPlaceholders.end(); ++it)
	{
		CElementPtr pElementPlaceholder = it->second;

		if (it->first == placeholderType )
		{			
			if (idx_only && pElementPlaceholder->m_lPlaceholderID < 0) continue;
			
			pElement = pElementPlaceholder->CreateDublicate();

			pElement->m_bPlaceholderSet = true;
			
			pLayout->m_arElements.push_back(pElement);
			pLayout->m_mapPlaceholders.insert(std::make_pair(placeholderType, pElement)); 
		}
	}

	return pElement; //last added
}

CElementPtr CPPTUserInfo::AddNewLayoutPlaceholder (CLayout *pLayout, int placeholderType, int placeholderSizePreset)
{
	if (placeholderType < 1) return CElementPtr();

	CShapeElement* pShape = new CShapeElement(NSBaseShape::ppt, PPTShapes::sptCRect);

	pShape->m_lPlaceholderType			= placeholderType;
	pShape->m_lPlaceholderSizePreset	= placeholderSizePreset;
	
	pShape->m_bPlaceholderSet			= false;
	pShape->m_bLine						= false;
	pShape->m_bAnchorEnabled			= false;
	pShape->m_bChildAnchorEnabled		= false;

	CorrectPlaceholderType(pShape->m_lPlaceholderType);

	CElementPtr pElement = CElementPtr(pShape);

	pLayout->m_arElements.push_back(pElement);
	pLayout->m_mapPlaceholders.insert(std::make_pair(pShape->m_lPlaceholderType, pElement)); 

	return pElement;
}

int CPPTUserInfo::AddNewLayout(CTheme* pTheme, CRecordSlide* pRecordSlide, bool addShapes, bool bMasterObjects)
{
	if (pTheme			== NULL) return -1;
	if (pRecordSlide	== NULL) return -1;

	std::vector<CRecordSlideAtom*> oArraySlideAtoms;

	pRecordSlide->GetRecordsByType(&oArraySlideAtoms, true);
	if (0 == oArraySlideAtoms.size())	return -1;
		
	SSlideLayoutAtom & layoutRecord	= oArraySlideAtoms[0]->m_oLayout;
	
	int ind = pTheme->m_arLayouts.size();
	
	pTheme->m_arLayouts.push_back(boost::make_shared<NSPresentationEditor::CLayout>());
	CLayout *pLayout = pTheme->m_arLayouts.back().get();
	
	pLayout->m_bUseThemeColorScheme = true;
	pLayout->m_bShowMasterShapes	= true;

	pLayout->m_strLayoutType = ConvertLayoutType(layoutRecord.m_nGeom, layoutRecord.m_pPlaceHolderID);

	if (!addShapes) return ind;
//далее только для типовых шаблонов
	pTheme->m_mapGeomToLayout.insert(std::pair<_UINT64, LONG>(layoutRecord.m_hash, ind));
	
	int defObjSize = -1;
	switch (layoutRecord.m_nGeom)
	{
		case 0x01:	// SL_TitleBody
			defObjSize = 0;	break;
		case 0x08:	//SL_TwoColumns
		case 0x09:	//SL_TwoRows
			defObjSize = 1;	break;
		case 0x0A:	//SL_ColumnTwoRows
			defObjSize = 3;	break;
		case 0x0B:	//SL_TwoRowsColumn
			defObjSize = 3;	break;
		case 0x0D:	//SL_TwoColumnsRow
			defObjSize = 3; break;
		case 0x0E://SL_FourObjects
			defObjSize = 2; break;
		case 0x0F:	//SL_BigObject
		case 0x11:	//SL_VerticalTitleBody
		case 0x12:	//SL_VerticalTwoRows
			defObjSize = 0; break;
	}

	for (int i = 0 ; i < 8; i ++)
	{
		if (layoutRecord.m_pPlaceHolderID[i] == 0)	break;

		switch(layoutRecord.m_pPlaceHolderID[i])
		{
		case NSOfficePPT::MasterTitle	:
		case NSOfficePPT::MasterBody			:
		case NSOfficePPT::MasterCenteredTitle	:
		case NSOfficePPT::MasterSubtitle		:
		case NSOfficePPT::MasterNotesSlideImage	:
		case NSOfficePPT::MasterNotesBody:
		case NSOfficePPT::MasterSlideNumber:
		case NSOfficePPT::MasterDate:
		case NSOfficePPT::MasterHeader:
		case NSOfficePPT::MasterFooter:
			{
				int usualType = layoutRecord.m_pPlaceHolderID[i];
				CorrectPlaceholderType(usualType);

				if (!AddThemeLayoutPlaceholder(pLayout, usualType, pTheme))
				{
					AddNewLayoutPlaceholder(pLayout, usualType, defObjSize);
				}					
			}break;
		default:
			AddNewLayoutPlaceholder(pLayout, layoutRecord.m_pPlaceHolderID[i], defObjSize);
			break;
		}
	}
	//if (layoutRecord.m_nGeom==0x0F) return ind; // big object only !!!

	//копируем все элементы без idx которые не были прописаны явно
	for (std::multimap<int, CElementPtr>::iterator it = pTheme->m_mapPlaceholders.begin(); it != pTheme->m_mapPlaceholders.end(); ++it)
	{		
		CElementPtr pElemTheme = it->second;
		if (pElemTheme->m_lPlaceholderID >= 0) continue;

		bool found = false;
		for (std::multimap<int, CElementPtr>::iterator it1 = pLayout->m_mapPlaceholders.begin(); it1 != pLayout->m_mapPlaceholders.end(); it1++)
		{
			if (it1->first == it->first)
			{
				CElementPtr pElemLayout = it1->second;
				if (pElemLayout->m_lPlaceholderID == pElemTheme->m_lPlaceholderID)
				{
					found = true;
					break;
				}					
			}
		}
		if (found == false)
		{
			CElementPtr pElement = pElemTheme->CreateDublicate();
		
			pLayout->m_arElements.push_back(pElement);
			pLayout->m_mapPlaceholders.insert(std::make_pair(it->first, pElement)); 
		}
	}

	pLayout->m_bHasDate			= pTheme->m_bHasDate;
	pLayout->m_bHasFooter		= pTheme->m_bHasFooter;
	pLayout->m_bHasSlideNumber	= pTheme->m_bHasSlideNumber;
	pLayout->m_nFormatDate		= pTheme->m_nFormatDate;

	for (int i = 0; i < 3; i++) pLayout->m_PlaceholdersReplaceString[i] = pTheme->m_PlaceholdersReplaceString[i];

	if (pLayout->m_bHasSlideNumber)		AddThemeLayoutPlaceholder(pLayout, MasterSlideNumber,	pTheme, true);
	if (pLayout->m_bHasDate)			AddThemeLayoutPlaceholder(pLayout, MasterDate,			pTheme, true);
	if (pLayout->m_bHasFooter)			AddThemeLayoutPlaceholder(pLayout, MasterFooter,		pTheme, true);

	return ind;
}

CElementPtr CPPTUserInfo::AddNewThemePlaceholder (CTheme* pTheme, int placeholderType, int placeholderSizePreset)
{
	if (placeholderType < 1) return CElementPtr();

	CShapeElement* pShape = new CShapeElement(NSBaseShape::ppt, PPTShapes::sptCRect);

	pShape->m_lPlaceholderType			= placeholderType;
	pShape->m_lPlaceholderSizePreset	= placeholderSizePreset;
	
	pShape->m_bPlaceholderSet			= false;
	pShape->m_bLine						= false;
	pShape->m_bAnchorEnabled			= false;
	pShape->m_bChildAnchorEnabled		= false;

	CorrectPlaceholderType(pShape->m_lPlaceholderType);

	CElementPtr pElement = CElementPtr(pShape);

	pTheme->m_arElements.push_back(pElement);
	pTheme->m_mapPlaceholders.insert(std::make_pair(pShape->m_lPlaceholderType, pElement)); 

	return pElement;
}
void CPPTUserInfo::LoadMainMaster(_UINT32 dwMasterID)
{
	std::map<_UINT32, LONG>::iterator pPair = m_mapMasterToTheme.find(dwMasterID);
	if (pPair != m_mapMasterToTheme.end())
	{
		// мастер уже загружен
		return;
	}

	LoadMasterFromPrevUsers(dwMasterID);

	std::map<_UINT32, CRecordSlide*>::iterator pPairMaster = m_mapMasters.find(dwMasterID);

	if (m_mapMasters.end() == pPairMaster)//??? не может быть 
		return;

	CRecordSlide* pMaster = pPairMaster->second;

    if (pMaster == NULL)//??? не может быть 
        return; 

	std::vector<CRecordSlideAtom*> oArraySlideAtoms;
	pMaster->GetRecordsByType(&oArraySlideAtoms, true);
	if (0 == oArraySlideAtoms.size())
		return;

	_UINT32 dwID				= (_UINT32)oArraySlideAtoms[0]->m_nMasterIDRef;
	if (0 != dwID)
	{
		// этот мастер - не main!!!
		// сначала загрузим все main, а потом - title
		// title нужно грузить как обычный слайд.
		return;
	}
	std::vector<CRecordTripCompositeMasterId12Atom*> oArrayCompId;
	pMaster->GetRecordsByType(&oArrayCompId, false, true);
	if (0 != oArrayCompId.size())
	{
		// этот мастер - не main!!!
		// сначала загрузим все main, а потом - title
		// title нужно грузить как обычный слайд.
		return;
	}    
	
	bool bMasterColorScheme = oArraySlideAtoms[0]->m_bMasterScheme;
    bool bMasterBackGround	= oArraySlideAtoms[0]->m_bMasterBackground;
    bool bMasterObjects		= oArraySlideAtoms[0]->m_bMasterObjects;

	std::vector<CRecordTripOriginalMasterId12Atom*> oArrayOrigId;
	pMaster->GetRecordsByType(&oArrayOrigId, false, true);
	
	if (0 != oArrayOrigId.size())
		m_mapMasterOriginalIds.insert(std::pair<_UINT32, _UINT32>(oArrayOrigId[0]->m_dwID, dwMasterID));

	LONG lIndexTheme = (LONG)m_arThemes.size();
	
	m_mapMasterToTheme.insert(std::pair<_UINT32, LONG>(dwMasterID, lIndexTheme));

	m_arThemes.push_back(boost::make_shared<NSPresentationEditor::CTheme>());
	CTheme* pTheme = m_arThemes[lIndexTheme].get();

	std::vector<CRecordHeadersFootersContainer*> oArrayHeadersFootersInfo;
	pMaster->GetRecordsByType(&oArrayHeadersFootersInfo, true, false);

	if (0 != oArrayHeadersFootersInfo.size())
	{
		if (oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom)
		{
			pTheme->m_bHasDate			=	oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasDate ||
											oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasTodayDate || 
											oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasUserDate;
			pTheme->m_bHasFooter		=	oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasFooter;
			pTheme->m_bHasSlideNumber	=	oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasSlideNumber;
			
			if (oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasUserDate ) 	pTheme->m_nFormatDate = 2;
		}
		for(int i = 0 ; i< 3; i++) pTheme->m_PlaceholdersReplaceString[i] = oArrayHeadersFootersInfo[0]->m_HeadersFootersString[i];
	}
	else
	{
		pTheme->m_bHasDate			=	m_bHasDate;
		pTheme->m_bHasFooter		=	m_bHasFooter;
		pTheme->m_bHasSlideNumber	=	m_bHasSlideNumber;		
		pTheme->m_nFormatDate		=	m_nFormatDate;
		for (int i = 0 ; i < 3; i++) pTheme->m_PlaceholdersReplaceString[i] = m_PlaceholdersReplaceString[i];
	}
	std::vector<CRecordCString*> oArrayStrings;
	pMaster->GetRecordsByType(&oArrayStrings, false, false);
	
	for (size_t i = 0; i < oArrayStrings.size(); i++)
	{
		if (oArrayStrings[i]->m_oHeader.RecType == 0x0fba)
		{
			pTheme->m_sThemeName = oArrayStrings[i]->m_strText;
		}
	}

	CSlideInfo slide_info;
	m_arMasterWrapper.push_back(slide_info);
	CSlideInfo* pMasterWrapper = &m_arMasterWrapper[lIndexTheme];

	// записываем шрифты ---------------------------------------------------------------
	int nCountFonts = m_arrFonts.size();
	for (int i = 0; i < nCountFonts; ++i)
	{
		pTheme->m_arFonts.push_back(m_arrFonts[i]);
	}
	// ---------------------------------------------------------------------------------

	// читаем цветовую схему -----------------------------------------------------------
	std::vector<CRecordColorSchemeAtom*> oArrayColors;
	pMaster->GetRecordsByType(&oArrayColors, false);

	int nColorCount = oArrayColors.size();
	for (int i = 0; i < nColorCount; ++i)
	{
		if (0x01 == oArrayColors[i]->m_oHeader.RecInstance)
		{
			if ( m_oSchemeColors.empty())	
			{
				oArrayColors[i]->ToArray(&m_oSchemeColors);
				CorrectColorScheme(m_oSchemeColors);//??
			}
			if ( pTheme->m_arColorScheme.empty())
			{
				oArrayColors[i]->ToArray(&pTheme->m_arColorScheme);
				CorrectColorScheme(pTheme->m_arColorScheme);
			}
		}

		if (0x06 == oArrayColors[i]->m_oHeader.RecInstance)
		{
			std::vector<CColor> extra;
			oArrayColors[i]->ToArray(&extra);

			CorrectColorScheme(extra);
			pTheme->m_arExtraColorScheme.push_back(extra);
		}
	}
	if (pTheme->m_arColorScheme.empty() && !pTheme->m_arExtraColorScheme.empty())
	{
		pTheme->m_arColorScheme = pTheme->m_arExtraColorScheme[0];
	}

	// ---------------------------------------------------------------------------------
	std::map<_UINT32, CRecordSlide*>::iterator pPairMaster1 = m_mapMasters.find(dwMasterID);

	int indexUser = 0;
	if (pPairMaster1 != m_mapMasters.end())
	{
		indexUser = pPairMaster1->second->m_IndexUser;
		pMasterWrapper->m_arTextPlaceHolders = pPairMaster1->second->m_oPersist.m_arTextAttrs;
	}
	if (m_pDocumentInfo->m_arUsers[indexUser]->m_arOffsetPictures.empty() == false)
		pMasterWrapper->m_parEmptyPictures	= &m_pDocumentInfo->m_arUsers[indexUser]->m_arOffsetPictures;
	else
		pMasterWrapper->m_parEmptyPictures	= &m_pDocumentInfo->m_arUsers[0]->m_arOffsetPictures;

	pMasterWrapper->m_mapFilePictures	= &m_pDocumentInfo->m_mapStoreImageFile;

	// читаем настройки текстовых стилей -----------------------------------------------
	std::vector<CRecordTextMasterStyleAtom*> oArrayTextMasters;
	pMaster->GetRecordsByType(&oArrayTextMasters, true, false);

	for (size_t i = 0; i < oArrayTextMasters.size(); ++i)
	{
		LONG lType = (LONG)oArrayTextMasters[i]->m_oHeader.RecInstance; 
		if ((0 > lType) || (lType > 8))
			continue;

		pMasterWrapper->m_pStyles[lType] = new NSPresentationEditor::CTextStyles();
		pMasterWrapper->m_pStyles[lType]->SetStyles((NSPresentationEditor::CTextStyles*)oArrayTextMasters[i]);

		CTheme::CalculateStyle(pTheme, pMasterWrapper->m_pStyles[lType].get());
	}
	if (pMasterWrapper->m_pStyles[3].is_init())
		pMasterWrapper->m_pStyles[3]->ApplyBefore(m_oDefaultTextStyle);
	else
		pMasterWrapper->m_pStyles[3] = m_oDefaultTextStyle;

	CTextStyles oPPTDefaultStyle;
	CreateDefaultStyle(oPPTDefaultStyle, pTheme);
	oPPTDefaultStyle.ApplyAfter(m_oDefaultTextStyle);

	// выставим стили теме
	pTheme->m_pStyles[0] = oPPTDefaultStyle;
	pTheme->m_pStyles[1] = oPPTDefaultStyle;
	pTheme->m_pStyles[2] = oPPTDefaultStyle;
	pTheme->m_pStyles[3] = oPPTDefaultStyle;

	if (pMasterWrapper->m_pStyles[0].is_init())
		pTheme->m_pStyles[1].ApplyAfter(pMasterWrapper->m_pStyles[0].get());
	if (pMasterWrapper->m_pStyles[1].is_init())
		pTheme->m_pStyles[2].ApplyAfter(pMasterWrapper->m_pStyles[1].get());
	if (pMasterWrapper->m_pStyles[2].is_init())
		pTheme->m_pStyles[3].ApplyAfter(pMasterWrapper->m_pStyles[3].get());

	pTheme->CalculateStyles();
	
	CLayout* pLayout = NULL; // ну нету тут разметок ...!!
// ---------------------------------------------------------------------------------

	std::vector<CRecordDrawingContainer*> oArrayDrawing;
	pMaster->GetRecordsByType(&oArrayDrawing, true);

	m_current_level = 0;
	m_current_elements = &pTheme->m_arElements;
	
	if (!oArrayDrawing.empty())
	{
		for (size_t nIndex = 0; nIndex < oArrayDrawing[0]->m_arRecords.size(); ++nIndex)
		{
			CRecordGroupShapeContainer* pGroup = dynamic_cast<CRecordGroupShapeContainer*>(oArrayDrawing[0]->m_arRecords[nIndex]);

			if (pGroup)
			{
				LoadGroupShapeContainer(pGroup, NULL, pTheme, pLayout, pMasterWrapper, pMasterWrapper);
			}
			else
			{
				CRecordShapeContainer* pShapeGroup = dynamic_cast<CRecordShapeContainer*>(oArrayDrawing[0]->m_arRecords[nIndex]);
				if (pShapeGroup)
				{
					CElementPtr pElement = pShapeGroup->GetElement(false, &m_oExMedia, pTheme, pLayout, pMasterWrapper, pMasterWrapper);
					
					CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElement.get());
					if (NULL != pShape)
					{
						pShape->SetupProperties(NULL, pTheme, pLayout);

						pTheme->m_bIsBackground = true;
						pTheme->m_oBackground = pShape->m_oBrush;
					}
				}
			}			
		}
	}
}

void CPPTUserInfo::LoadMasters()
{
	for (size_t i = 0; i< m_arrMastersOrder.size(); i++)
	{
		std::map<_UINT32, CRecordSlide*>::iterator pPair = m_mapMasters.find(m_arrMastersOrder[i]);
		if (pPair == m_mapMasters.end())continue;			
	
		LoadMainMaster(pPair->first);
	}

	for (size_t i = 0; i< m_arrMastersOrder.size(); i++)
	{
		std::map<_UINT32, CRecordSlide*>::iterator pPair = m_mapMasters.find(m_arrMastersOrder[i]);
		if (pPair == m_mapMasters.end())continue;			
	
		LoadNoMainMaster(pPair->first);
	}

	LoadNotesMasterFromPrevUsers(0);
	if (!m_mapNotesMasters.empty())
	{
		std::map<_UINT32, CRecordSlide*>::iterator pPair = m_mapNotesMasters.begin();
	
		LoadMaster(typeNotesMaster, pPair->second, m_pNotesMasterWrapper, m_pNotesMaster);
	}

	LoadHandoutMasterFromPrevUsers(0);
	if (!m_mapHandoutMasters.empty())
	{
		std::map<_UINT32, CRecordSlide*>::iterator pPair = m_mapHandoutMasters.begin();
	
		LoadMaster(typeHandoutMaster, pPair->second, m_pHandoutMasterWrapper, m_pHandoutMaster);
	}
}
void CPPTUserInfo::LoadMaster(_typeMaster type, CRecordSlide* pMaster, CSlideInfo *& pMasterWrapper, CThemePtr & pTheme)
{
    if (pMaster == NULL)
        return; 
	
	bool bMasterColorScheme = false;
    bool bMasterBackGround	= false;
    bool bMasterObjects		= false;

	_UINT32 dwID = 0;

	std::vector<CRecordSlideAtom*> oArraySlideAtoms;
	pMaster->GetRecordsByType(&oArraySlideAtoms, true);
	
	if (!oArraySlideAtoms.empty())
	{
		dwID = (_UINT32)oArraySlideAtoms[0]->m_nMasterIDRef;

		bMasterColorScheme	= oArraySlideAtoms[0]->m_bMasterScheme;
		bMasterBackGround	= oArraySlideAtoms[0]->m_bMasterBackground;
		bMasterObjects		= oArraySlideAtoms[0]->m_bMasterObjects;
	}
	else
	{
		std::vector<CRecordNotesAtom*> oArrayNotesAtoms;
		pMaster->GetRecordsByType(&oArrayNotesAtoms, true);
		
		if (!oArrayNotesAtoms.empty())
		{
			dwID = (_UINT32)oArrayNotesAtoms[0]->m_nSlideIDRef;

			bMasterColorScheme	= oArrayNotesAtoms[0]->m_bMasterScheme;
			bMasterBackGround	= oArrayNotesAtoms[0]->m_bMasterBackground;
			bMasterObjects		= oArrayNotesAtoms[0]->m_bMasterObjects;
		}
	}

	pTheme = boost::make_shared<NSPresentationEditor::CTheme>(type);

	std::vector<CRecordHeadersFootersContainer*> oArrayHeadersFootersInfo;
	pMaster->GetRecordsByType(&oArrayHeadersFootersInfo, true, false);

	if (0 != oArrayHeadersFootersInfo.size())
	{
		if (oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom)
		{
			pTheme->m_bHasDate			=	oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasDate ||
											oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasTodayDate || 
											oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasUserDate;
			pTheme->m_bHasFooter		=	oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasFooter;
			pTheme->m_bHasSlideNumber	=	oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasSlideNumber;
			
			if (oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasUserDate ) 	pTheme->m_nFormatDate = 2;
		}
		for(int i = 0 ; i< 3; i++) pTheme->m_PlaceholdersReplaceString[i] = oArrayHeadersFootersInfo[0]->m_HeadersFootersString[i];
	}
	else
	{
		pTheme->m_bHasDate			=	m_bHasDate;
		pTheme->m_bHasFooter		=	m_bHasFooter;
		pTheme->m_bHasSlideNumber	=	m_bHasSlideNumber;		
		pTheme->m_nFormatDate		=	m_nFormatDate;
		for (int i = 0 ; i < 3; i++) pTheme->m_PlaceholdersReplaceString[i] = m_PlaceholdersReplaceString[i];
	}
	std::vector<CRecordCString*> oArrayStrings;
	pMaster->GetRecordsByType(&oArrayStrings, false, false);
	
	for (size_t i = 0; i < oArrayStrings.size(); i++)
	{
		if (oArrayStrings[i]->m_oHeader.RecType == 0x0fba)
		{
			pTheme->m_sThemeName = oArrayStrings[i]->m_strText;
		}
	}

	pMasterWrapper = new CSlideInfo();

	// записываем шрифты ---------------------------------------------------------------
	int nCountFonts = m_arrFonts.size();
	for (int i = 0; i < nCountFonts; ++i)
	{
		pTheme->m_arFonts.push_back(m_arrFonts[i]);
	}
	// ---------------------------------------------------------------------------------

	// читаем цветовую схему -----------------------------------------------------------
	std::vector<CRecordColorSchemeAtom*> oArrayColors;
	pMaster->GetRecordsByType(&oArrayColors, false);

	int nColorCount = oArrayColors.size();
	for (int i = 0; i < nColorCount; ++i)
	{
		if (0x01 == oArrayColors[i]->m_oHeader.RecInstance)
		{
			if ( m_oSchemeColors.empty())	
			{
				oArrayColors[i]->ToArray(&m_oSchemeColors);
				CorrectColorScheme(m_oSchemeColors);//??
			}
			if ( pTheme->m_arColorScheme.empty())
			{
				oArrayColors[i]->ToArray(&pTheme->m_arColorScheme);
				CorrectColorScheme(pTheme->m_arColorScheme);
			}
		}

		if (0x06 == oArrayColors[i]->m_oHeader.RecInstance)
		{
			std::vector<CColor> extra;
			oArrayColors[i]->ToArray(&extra);

			CorrectColorScheme(extra);
			pTheme->m_arExtraColorScheme.push_back(extra);
		}
	}
	if (pTheme->m_arColorScheme.empty() && !pTheme->m_arExtraColorScheme.empty())
	{
		pTheme->m_arColorScheme = pTheme->m_arExtraColorScheme[0];
	}

// ---------------------------------------------------------------------------------
	int indexUser = 0;
	//std::map<_UINT32, CRecordSlide*>::iterator pPairMaster1 = m_mapMasters.find(dwMasterID);
	//if (pPairMaster1 != m_mapMasters.end())
	//{
	//	indexUser = pPairMaster1->second->m_IndexUser;
	//	pMasterWrapper->m_arTextPlaceHolders = pPairMaster1->second->m_oPersist.m_arTextAttrs;
	//}
	pMasterWrapper->m_parEmptyPictures	= &m_pDocumentInfo->m_arUsers[indexUser]->m_arOffsetPictures;
	pMasterWrapper->m_mapFilePictures	= &m_pDocumentInfo->m_mapStoreImageFile;

	// читаем настройки текстовых стилей -----------------------------------------------
	std::vector<CRecordTextMasterStyleAtom*> oArrayTextMasters;
	pMaster->GetRecordsByType(&oArrayTextMasters, true, false);

	for (size_t i = 0; i < oArrayTextMasters.size(); ++i)
	{
		LONG lType = (LONG)oArrayTextMasters[i]->m_oHeader.RecInstance; 
		if ((0 > lType) || (lType > 8))
			continue;

		pMasterWrapper->m_pStyles[lType] = new NSPresentationEditor::CTextStyles();
		pMasterWrapper->m_pStyles[lType]->SetStyles((NSPresentationEditor::CTextStyles*)oArrayTextMasters[i]);

		CTheme::CalculateStyle(pTheme.get(), pMasterWrapper->m_pStyles[lType].get());
	}
	if (pMasterWrapper->m_pStyles[3].is_init())
		pMasterWrapper->m_pStyles[3]->ApplyBefore(m_oDefaultTextStyle);
	else
		pMasterWrapper->m_pStyles[3] = m_oDefaultTextStyle;

	CTextStyles oPPTDefaultStyle;
	CreateDefaultStyle(oPPTDefaultStyle, pTheme.get());
	oPPTDefaultStyle.ApplyAfter(m_oDefaultTextStyle);

	// выставим стили теме
	pTheme->m_pStyles[0] = oPPTDefaultStyle;
	pTheme->m_pStyles[1] = oPPTDefaultStyle;
	pTheme->m_pStyles[2] = oPPTDefaultStyle;
	pTheme->m_pStyles[3] = oPPTDefaultStyle;

	if (pMasterWrapper->m_pStyles[0].is_init())
		pTheme->m_pStyles[1].ApplyAfter(pMasterWrapper->m_pStyles[0].get());
	if (pMasterWrapper->m_pStyles[1].is_init())
		pTheme->m_pStyles[2].ApplyAfter(pMasterWrapper->m_pStyles[1].get());
	if (pMasterWrapper->m_pStyles[2].is_init())
		pTheme->m_pStyles[3].ApplyAfter(pMasterWrapper->m_pStyles[3].get());

	pTheme->CalculateStyles();
	
	CLayout* pLayout = NULL; // ну нету тут разметок ...!!
	
//-------------------------------------------------------------------------------------------------------
	std::vector<CRecordDrawingContainer*> oArrayDrawing;
	pMaster->GetRecordsByType(&oArrayDrawing, true);

	m_current_level = 0;
	m_current_elements = &pTheme->m_arElements;
	
	if (!oArrayDrawing.empty())
	{
		for (size_t nIndex = 0; nIndex < oArrayDrawing[0]->m_arRecords.size(); ++nIndex)
		{
			CRecordGroupShapeContainer* pGroup = dynamic_cast<CRecordGroupShapeContainer*>(oArrayDrawing[0]->m_arRecords[nIndex]);

			if (pGroup)
			{
				LoadGroupShapeContainer(pGroup, NULL, pTheme.get(), pLayout, pMasterWrapper, pMasterWrapper);
			}
			else
			{
				CRecordShapeContainer* pShapeGroup = dynamic_cast<CRecordShapeContainer*>(oArrayDrawing[0]->m_arRecords[nIndex]);
				if (pShapeGroup)
				{
					CElementPtr pElement = pShapeGroup->GetElement(false, &m_oExMedia, pTheme.get(), pLayout, pMasterWrapper, pMasterWrapper);
					
					CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElement.get());
					if (NULL != pShape && pShape->m_bIsBackground && !pShape->m_bHaveAnchor)
					{
						pShape->SetupProperties(NULL, pTheme.get(), pLayout);

						pTheme->m_bIsBackground = true;
						pTheme->m_oBackground = pShape->m_oBrush;
					}
				}
			}			
		}
	}
}
void CPPTUserInfo::LoadNoMainMaster(_UINT32 dwMasterID)
{
	std::map<_UINT32, CRecordSlide*>::iterator pPair = m_mapMasters.find(dwMasterID);

	if (pPair == m_mapMasters.end())
		return;

	CRecordSlide* pCurMaster = pPair->second;

	if (pCurMaster == NULL)
		return;
	_UINT32 dwCurID = pCurMaster->m_oPersist.m_nSlideID;

	std::vector<CRecordSlideAtom*> oArraySlideAtoms;

	pCurMaster->GetRecordsByType(&oArraySlideAtoms, true);
	if (0 == oArraySlideAtoms.size())
		return;

    bool bMasterColorScheme = oArraySlideAtoms[0]->m_bMasterScheme;
    bool bMasterBackGround	= oArraySlideAtoms[0]->m_bMasterBackground;
    bool bMasterObjects		= oArraySlideAtoms[0]->m_bMasterObjects;

	_UINT32 dwID = (_UINT32)oArraySlideAtoms[0]->m_nMasterIDRef;

	if (0 == dwID)
	{
		std::vector<CRecordTripCompositeMasterId12Atom*> oArrayCompId;
		pCurMaster->GetRecordsByType(&oArrayCompId, false, true);
		if (0 != oArrayCompId.size())
		{
			std::map<_UINT32, _UINT32>::iterator pPair1 = m_mapMasterOriginalIds.find(oArrayCompId[0]->m_dwID);
			if (m_mapMasterOriginalIds.end() != pPair1)
			{
				dwID = pPair1->second;
			}
		}
	}

	// проверяем, если это MainMaster - то его грузим как мастер
	if (0 == dwID)
	{
		// он уже загружен как тема !!!
		return;
	}

	// теперь аналог функции LoadSlide 
	CSlideInfo elm;
	m_arMasterWrapper.push_back(elm);

	CSlideInfo* pMasterWrapper	= &m_arMasterWrapper[m_arMasterWrapper.size() - 1];

	pMasterWrapper->m_arTextPlaceHolders	= pCurMaster->m_oPersist.m_arTextAttrs;		
	pMasterWrapper->m_mapFilePictures		= &m_pDocumentInfo->m_mapStoreImageFile;
	
	if (m_pDocumentInfo->m_arUsers[pCurMaster->m_IndexUser]->m_arOffsetPictures.empty() == false)
		pMasterWrapper->m_parEmptyPictures	= &m_pDocumentInfo->m_arUsers[pCurMaster->m_IndexUser]->m_arOffsetPictures;
	else
		pMasterWrapper->m_parEmptyPictures	= &m_pDocumentInfo->m_arUsers[0]->m_arOffsetPictures;

	std::map<_UINT32, LONG>::iterator pPairTheme = m_mapMasterToTheme.find(dwID);
	
	if (m_mapMasterToTheme.end() == pPairTheme)
		return;

	m_mapMasterToTheme.insert(std::pair<_UINT32, LONG>(dwMasterID, pPairTheme->second));

	CSlideInfo	* pThemeWrapper	= &m_arMasterWrapper[pPairTheme->second];
	CTheme		* pTheme		= m_arThemes		[pPairTheme->second].get();

	std::wstring strLayoutType = ConvertLayoutType(oArraySlideAtoms[0]->m_oLayout.m_nGeom, oArraySlideAtoms[0]->m_oLayout.m_pPlaceHolderID);
	
	CLayout* pLayout = NULL;

	int lLayoutID = AddNewLayout(pTheme, pCurMaster, false, false);
	
	pLayout							= pTheme->m_arLayouts[lLayoutID].get();
	pLayout->m_bShowMasterShapes	= false;
	pLayout->m_bIsTitleMaster		= true;

	pTheme->m_mapTitleLayout[dwMasterID] = lLayoutID;

	std::vector<NSPresentationEditor::CColor>* pArrayColorScheme = &pTheme->m_arColorScheme;
	// читаем цветовую схему -----------------------------------------------------------
	if (!bMasterColorScheme)
	{
		std::vector<CRecordColorSchemeAtom*> oArrayColors;
		pCurMaster->GetRecordsByType(&oArrayColors, false);

		pLayout->m_arColorScheme.clear();
		for (size_t i = 0; i < oArrayColors.size(); ++i)
		{
			if (0x01 == oArrayColors[i]->m_oHeader.RecInstance)
			{
				oArrayColors[i]->ToArray(&pLayout->m_arColorScheme);
				pLayout->m_bUseThemeColorScheme = false;
				CorrectColorScheme(pLayout->m_arColorScheme);

				// проверим на совпадение
				size_t nCountC = pLayout->m_arColorScheme.size();
				size_t nIndexC = 0;
				if (pArrayColorScheme && nCountC == pArrayColorScheme->size())
				{
					for (; nIndexC < nCountC; ++nIndexC)
					{
						if (pLayout->m_arColorScheme[i].IsEqual(pArrayColorScheme->at(i)))
							break;
					}
				}

				if (nIndexC == nCountC)
				{
					pLayout->m_bUseThemeColorScheme = true;
					pLayout->m_arColorScheme.clear();
				}

				break;
			}
		}
	}
	std::vector<CRecordHeadersFootersContainer*> oArrayHeadersFootersInfo;
	pCurMaster->GetRecordsByType(&oArrayHeadersFootersInfo, true, false);

	if (0 != oArrayHeadersFootersInfo.size())
	{
		if (oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom)
		{
			pLayout->m_bHasDate			=	oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasDate ||
											oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasTodayDate || 
											oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasUserDate;
			pLayout->m_bHasFooter		=	oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasFooter;
			pLayout->m_bHasSlideNumber	=	oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasSlideNumber;
			
			if (oArrayHeadersFootersInfo[0]->m_oHeadersFootersAtom->m_bHasUserDate) pLayout->m_nFormatDate = 2;

			for (int i = 0 ; i < 3; i++) pLayout->m_PlaceholdersReplaceString[i] = oArrayHeadersFootersInfo[0]->m_HeadersFootersString[i];
		}
	}
	else
	{//????
			pLayout->m_bHasDate			=	m_bHasDate;
			pLayout->m_bHasFooter		=	m_bHasFooter;
			pLayout->m_bHasSlideNumber	=	m_bHasSlideNumber;
			pLayout->m_nFormatDate		=	m_nFormatDate;
			for (int i = 0 ; i < 3; i++) pLayout->m_PlaceholdersReplaceString[i] = m_PlaceholdersReplaceString[i];
	}
	
	std::vector<CRecordCString*> oArrayStrings;
	pCurMaster->GetRecordsByType(&oArrayStrings, false, false);
	
	for (size_t i = 0; i < oArrayStrings.size(); i++)
	{
		if (oArrayStrings[i]->m_oHeader.RecType == 0x0fba)
		{
			pLayout->m_sName = oArrayStrings[i]->m_strText;
		}
	}
//-------------------------------------------------------------------------------------------------------
	std::vector<CRecordDrawingContainer*> oArrayDrawing;
	pCurMaster->GetRecordsByType(&oArrayDrawing, true);

	m_current_level = 0;
	m_current_elements = &pLayout->m_arElements;
	
	if (!oArrayDrawing.empty())
	{
		for (size_t nIndex = 0; nIndex < oArrayDrawing[0]->m_arRecords.size(); ++nIndex)
		{
			CRecordGroupShapeContainer* pGroup = dynamic_cast<CRecordGroupShapeContainer*>(oArrayDrawing[0]->m_arRecords[nIndex]);

			if (pGroup)
			{
				LoadGroupShapeContainer(pGroup, NULL, NULL, pLayout, pThemeWrapper, pMasterWrapper);
			}
			else
			{
				CRecordShapeContainer* pShapeGroup = dynamic_cast<CRecordShapeContainer*>(oArrayDrawing[0]->m_arRecords[nIndex]);
				if (pShapeGroup)
				{
					CElementPtr pElement = pShapeGroup->GetElement(false, &m_oExMedia, pTheme, pLayout, pThemeWrapper, pMasterWrapper);
					
					CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElement.get());
					
					if (NULL != pShape && pShape->m_bIsBackground && !pShape->m_bHaveAnchor)
					{
						pShape->SetupProperties(NULL, pTheme, pLayout);

						pLayout->m_bIsBackground = true;
						pLayout->m_oBackground	= pShape->m_oBrush;
					}
				}
			}			
		}
	}

	if (pLayout->m_bHasSlideNumber)
	{
		if (pLayout->m_mapPlaceholders.find(MasterSlideNumber) == pLayout->m_mapPlaceholders.end())
		{
			AddNewLayoutPlaceholder(pLayout, MasterSlideNumber, 2);
		}
	}
	if (pLayout->m_bHasDate && pLayout->m_nFormatDate == 1)
	{
		if (pLayout->m_mapPlaceholders.find(MasterDate) == pLayout->m_mapPlaceholders.end())
		{
			AddNewLayoutPlaceholder(pLayout, MasterDate, 2);
		}
	}
	if (pLayout->m_bHasFooter)
	{
		if (pLayout->m_mapPlaceholders.find(MasterFooter) == pLayout->m_mapPlaceholders.end())
		{
			AddNewLayoutPlaceholder(pLayout, MasterFooter, 1);
		}		
	}

}

void CPPTUserInfo::LoadSlideFromPrevUsers(_UINT32 dwSlideID)
{
	if ((NULL == m_pDocumentInfo) || (-1 == m_lIndexThisUser))
		return;

	std::map<_UINT32, CRecordSlide*>::iterator pPairSlide = m_mapSlides.find(dwSlideID);
	
	if (pPairSlide != m_mapSlides.end() && pPairSlide->second)
		return; //есть

	size_t lUsersCount = m_pDocumentInfo->m_arUsers.size();
		
	for (size_t lIndexUser = m_lIndexThisUser + 1; lIndexUser < lUsersCount; ++lIndexUser)
	{
		std::map<_UINT32, CRecordSlide*>::iterator pPair = m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapSlides.find(dwSlideID);
		if (pPair == m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapSlides.end())
			continue;

		CRecordSlide* pSlideCur = pPair->second;
		if (NULL != pSlideCur)
		{
			pSlideCur->AddRef();

			pSlideCur->m_IndexUser = lIndexUser;

			if (pPairSlide != m_mapSlides.end())
			{
				pPairSlide->second = pSlideCur;
			}
			else
			{
				m_mapSlides.insert(m_mapSlides.end(), std::pair<_UINT32, CRecordSlide*>(dwSlideID, pSlideCur));
				m_arrSlidesOrder.push_back(dwSlideID);
			}
			return;
		}
	}
}
void CPPTUserInfo::LoadMasterFromPrevUsers(_UINT32 dwMasterID)
{
	if ((NULL == m_pDocumentInfo) || (-1 == m_lIndexThisUser))
		return;

	std::map<_UINT32, CRecordSlide*>::iterator pPairMaster = m_mapMasters.find(dwMasterID);

	if (pPairMaster != m_mapMasters.end() && pPairMaster->second)
		return;//есть

	size_t lUsersCount = m_pDocumentInfo->m_arUsers.size();
	
	for (size_t lIndexUser = m_lIndexThisUser + 1; lIndexUser < lUsersCount; ++lIndexUser)
	{
		std::map<_UINT32, CRecordSlide*>::iterator pPair = m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapMasters.find(dwMasterID);

		if (pPair == m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapMasters.end())
			continue;
		
		CRecordSlide* pSlideCur = pPair->second;
		if (NULL != pSlideCur)
		{
			pSlideCur->AddRef();

			//для каждого пользователя СВОИ активные картинки !!!
			pSlideCur->m_IndexUser = lIndexUser;

			if (pPairMaster != m_mapMasters.end())
			{
				//был найден ранее нулевым
				pPairMaster->second = pSlideCur;
			}
			else
			{
				m_mapMasters.insert(m_mapMasters.end(), std::pair<_UINT32, CRecordSlide*>(dwMasterID, pSlideCur));
				m_arrMastersOrder.push_back(dwMasterID);
			}
			return;
		}
	}
}
void CPPTUserInfo::LoadNotesFromPrevUsers(_UINT32 dwSlideID)
{
	if ((NULL == m_pDocumentInfo) || (-1 == m_lIndexThisUser))
		return;

	std::map<_UINT32, CRecordSlide*>::iterator pPairSlide = m_mapNotes.find(dwSlideID);
	
	if (pPairSlide != m_mapNotes.end() && pPairSlide->second)
		return; //есть

	size_t lUsersCount = m_pDocumentInfo->m_arUsers.size();
		
	for (size_t lIndexUser = m_lIndexThisUser + 1; lIndexUser < lUsersCount; ++lIndexUser)
	{
		std::map<_UINT32, CRecordSlide*>::iterator pPair = m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapNotes.find(dwSlideID);
		if (pPair == m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapNotes.end())
			continue;

		CRecordSlide* pSlideCur = pPair->second;
		if (NULL != pSlideCur)
		{
			pSlideCur->AddRef();

			pSlideCur->m_IndexUser = lIndexUser;

			if (pPairSlide != m_mapNotes.end())
			{
				pPairSlide->second = pSlideCur;
			}
			else
			{
				m_mapNotes.insert(m_mapSlides.end(), std::pair<_UINT32, CRecordSlide*>(dwSlideID, pSlideCur));
				m_arrNotesOrder.push_back(dwSlideID);
			}
			return;
		}
	}
}

void CPPTUserInfo::LoadNotesMasterFromPrevUsers(_UINT32 dwMasterID)
{
	if ((NULL == m_pDocumentInfo) || (-1 == m_lIndexThisUser))
		return;

	std::map<_UINT32, CRecordSlide*>::iterator pPairMaster = m_mapNotesMasters.find(dwMasterID);

	if (pPairMaster != m_mapNotesMasters.end() && pPairMaster->second)
		return;//есть

	size_t lUsersCount = m_pDocumentInfo->m_arUsers.size();
	
	for (size_t lIndexUser = m_lIndexThisUser + 1; lIndexUser < lUsersCount; ++lIndexUser)
	{
		std::map<_UINT32, CRecordSlide*>::iterator pPair = m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapNotesMasters.find(dwMasterID);

		if (pPair == m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapNotesMasters.end())
			continue;
		
		CRecordSlide* pSlideCur = pPair->second;
		if (NULL != pSlideCur)
		{
			pSlideCur->AddRef();

			//для каждого пользователя СВОИ активные картинки !!!
			pSlideCur->m_IndexUser = lIndexUser;

			if (pPairMaster != m_mapNotesMasters.end())
			{
				//был найден ранее нулевым
				pPairMaster->second = pSlideCur;
			}
			else
			{
				m_mapNotesMasters.insert(m_mapNotesMasters.end(), std::pair<_UINT32, CRecordSlide*>(dwMasterID, pSlideCur));
			}
			return;
		}
	}
}
void CPPTUserInfo::LoadHandoutMasterFromPrevUsers(_UINT32 dwMasterID)
{
	if ((NULL == m_pDocumentInfo) || (-1 == m_lIndexThisUser))
		return;

	std::map<_UINT32, CRecordSlide*>::iterator pPairMaster = m_mapHandoutMasters.find(dwMasterID);

	if (pPairMaster != m_mapHandoutMasters.end() && pPairMaster->second)
		return;//есть

	size_t lUsersCount = m_pDocumentInfo->m_arUsers.size();
	
	for (size_t lIndexUser = m_lIndexThisUser + 1; lIndexUser < lUsersCount; ++lIndexUser)
	{
		std::map<_UINT32, CRecordSlide*>::iterator pPair = m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapHandoutMasters.find(dwMasterID);

		if (pPair == m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapHandoutMasters.end())
			continue;
		
		CRecordSlide* pSlideCur = pPair->second;
		if (NULL != pSlideCur)
		{
			pSlideCur->AddRef();

			//для каждого пользователя СВОИ активные картинки !!!
			pSlideCur->m_IndexUser = lIndexUser;

			if (pPairMaster != m_mapHandoutMasters.end())
			{
				//был найден ранее нулевым
				pPairMaster->second = pSlideCur;
			}
			else
			{
				m_mapHandoutMasters.insert(m_mapHandoutMasters.end(), std::pair<_UINT32, CRecordSlide*>(dwMasterID, pSlideCur));
			}
			return;
		}
	}
}
void CPPTUserInfo::LoadExternal(CRecordExObjListContainer* pExObjects)
{
	// читаем SoundCollection
	std::vector<CRecordSoundCollectionContainer*> oArrayCollection;
	m_oDocument.GetRecordsByType(&oArrayCollection, false, true);

	if (0 != oArrayCollection.size())
	{
		std::vector<CRecordSoundContainer*> oArraySounds;
		oArrayCollection[0]->GetRecordsByType(&oArraySounds, false);

		int nSize = oArraySounds.size();

		for (int i = 0; i < nSize; ++i)
		{
			std::vector<CRecordCString*>		oArrayStrings;
			std::vector<CRecordSoundDataBlob*> oArrayData;

			oArraySounds[i]->GetRecordsByType(&oArrayStrings, false);
			oArraySounds[i]->GetRecordsByType(&oArrayData, false);

			if ((3 <= oArrayStrings.size()) && (1 == oArrayData.size()))
			{
				NSPresentationEditor::CExFilesInfo oInfo;

				oInfo.m_strFilePath = m_oExMedia.m_strPresentationDirectory + FILE_SEPARATOR_STR + oArrayStrings[0]->m_strText + _T(".audio");
				oInfo.m_dwID		= (_UINT32)XmlUtils::GetInteger(oArrayStrings[2]->m_strText.c_str());

				oArrayData[0]->SaveToFile(oInfo.m_strFilePath);

				m_oExMedia.m_arAudioCollection.push_back(oInfo);
			}
		}
	}

	if (NULL == pExObjects)
		return;

	// читаем видео ----------------------------------------------
	std::vector<CRecordExVideoContainer*> oArray;
	pExObjects->GetRecordsByType(&oArray, true);

	for (size_t nIndex = 0; nIndex < oArray.size(); ++nIndex)
	{
		LoadExVideo(oArray[nIndex]);
	}

	oArray.clear();
	// -----------------------------------------------------------

	// читаем аудио ----------------------------------------------
	std::vector<CRecordExCDAudioContainer*>			oArrayAudioCD;
	std::vector<CRecordExMIDIAudioContainer*>		oArrayAudioMIDI;
	std::vector<CRecordWAVAudioLinkContainer*>		oArrayWAVLink;
	std::vector<CRecordWAVAudioEmbeddedContainer*>	oArrayAudioEmbedded;

	pExObjects->GetRecordsByType(&oArrayAudioCD			, true);
	pExObjects->GetRecordsByType(&oArrayAudioMIDI		, true);
	pExObjects->GetRecordsByType(&oArrayWAVLink			, true);
	pExObjects->GetRecordsByType(&oArrayAudioEmbedded	, true);

	for (size_t nIndex = 0; nIndex < oArrayAudioMIDI.size(); ++nIndex)
	{
		LoadExAudio(oArrayAudioMIDI[nIndex]);
	}
	for (size_t nIndex = 0; nIndex < oArrayWAVLink.size(); ++nIndex)
	{
		LoadExAudio(oArrayWAVLink[nIndex]);
	}
	for (size_t nIndex = 0; nIndex < oArrayAudioEmbedded.size(); ++nIndex)
	{
		_UINT32 dwKeySound	= oArrayAudioEmbedded[nIndex]->m_nSoundID;
		_UINT32 dwKeyObj		= oArrayAudioEmbedded[nIndex]->m_oMedia.m_nExObjID;

		NSPresentationEditor::CExFilesInfo* pInfo = m_oExMedia.LockAudioFromCollection(dwKeySound);
		if (NULL != pInfo)
		{
			NSPresentationEditor::CExFilesInfo oAudio;

			oAudio.m_dwID			= dwKeyObj;
			oAudio.m_strFilePath	= pInfo->m_strFilePath;
			oAudio.m_bLoop			= oArrayAudioEmbedded[nIndex]->m_oMedia.m_bLoop;

			m_oExMedia.m_arAudios.push_back(oAudio);
		}
	}
	for (size_t nIndex = 0; nIndex < oArrayAudioCD.size(); ++nIndex)
	{
		_UINT32 dwKeyObj			= oArrayAudioCD[nIndex]->m_oMedia.m_nExObjID;

		NSPresentationEditor::CExFilesInfo* pInfo		= m_oExMedia.LockAudio(dwKeyObj);

		if (NULL != pInfo)
		{
			pInfo->m_dStartTime		= oArrayAudioCD[nIndex]->m_dStartTime;
			pInfo->m_dEndTime		= oArrayAudioCD[nIndex]->m_dEndTime;
			pInfo->m_bLoop			= oArrayAudioCD[nIndex]->m_oMedia.m_bLoop;
		}	
	}

//--------------------------------------------------------------------
	
	std::vector<CRecordExHyperlinkContainer*>	oArrayHyperlinkContainer;

	pExObjects->GetRecordsByType(&oArrayHyperlinkContainer		, true);
	
	for (size_t nIndex = 0; nIndex < oArrayHyperlinkContainer.size(); ++nIndex)
	{
		std::vector<CRecordExHyperlinkAtom*>	oArrayHyperlink;		
		std::vector<CRecordCString*>			oArrayCString;		
		
		oArrayHyperlinkContainer[nIndex]->GetRecordsByType	(&oArrayHyperlink, false);
		oArrayHyperlinkContainer[nIndex]->GetRecordsByType	(&oArrayCString, false);

		if (oArrayCString.size() > 0 && oArrayHyperlink.size() > 0)
		{
			NSPresentationEditor::CExFilesInfo oInfo;

			oInfo.m_dwID = oArrayHyperlink[0]->m_nHyperlinkID;
			for (size_t i = 0 ; i < oArrayCString.size(); i++)
			{
				if (oArrayCString[i]->m_oHeader.RecInstance == 1)
					oInfo.m_strFilePath		= oArrayCString[i]->m_strText;
			}
			m_oExMedia.m_arHyperlinks.push_back(oInfo);
		}
	}
}

void CPPTUserInfo::LoadExVideo(CRecordsContainer* pExObject)
{
	std::vector<CRecordExMediaAtom*> oArrayExMedia;
	std::vector<CRecordCString*> oArrayCString;

	pExObject->GetRecordsByType(&oArrayExMedia, false);
	pExObject->GetRecordsByType(&oArrayCString, false);

	if ((1 == oArrayExMedia.size()) && (1 == oArrayCString.size()))
	{
		NSPresentationEditor::CExFilesInfo oInfo;

		oInfo.m_dwID			= oArrayExMedia[0]->m_nExObjID;
		oInfo.m_strFilePath		= oArrayCString[0]->m_strText;

		oInfo.m_bLoop			= oArrayExMedia[0]->m_bLoop;

		m_oExMedia.m_arVideos.push_back(oInfo);
	}

	oArrayExMedia.clear();
	oArrayCString.clear();
}
void CPPTUserInfo::LoadExAudio(CRecordsContainer* pExObject)
{
	std::vector<CRecordExMediaAtom*> oArrayExMedia;
	std::vector<CRecordCString*> oArrayCString;

	pExObject->GetRecordsByType(&oArrayExMedia, false);
	pExObject->GetRecordsByType(&oArrayCString, false);

	if ((1 == oArrayExMedia.size()) && (1 == oArrayCString.size()))
	{
		NSPresentationEditor::CExFilesInfo oInfo;

		oInfo.m_dwID			= oArrayExMedia[0]->m_nExObjID;
		oInfo.m_strFilePath		= oArrayCString[0]->m_strText;

		oInfo.m_bLoop			= oArrayExMedia[0]->m_bLoop;

		m_oExMedia.m_arAudios.push_back(oInfo);
	}

	oArrayExMedia.clear();
	oArrayCString.clear();
}

void CPPTUserInfo::AddAnimation ( _UINT32 dwSlideID, double Width, double Height, CElementPtr pElement )
{
	std::map <_UINT32, Animations::CSlideTimeLine*>::iterator pPair =	m_mapAnimations.find( dwSlideID );

	if (pPair == m_mapAnimations.end()) return;

	Animations::CSlideTimeLine* pTimeLine	= pPair->second;
	if (pTimeLine == NULL) return;

	std::map <_UINT32, Animations::Effects*>::iterator pPairA = pTimeLine->GetAnimation().find ( pElement->m_lID );
	if (pPairA == pTimeLine->GetAnimation().end()) return;
	
	Animations::Effects* arEffects = pPairA->second;
	if (arEffects == NULL) return;

	for ( long i = 0; i < (long)arEffects->size(); ++i )
	{								
		CAnimationSimple oEffect	=	arEffects->at(i);
		CAnimationSimple oAnimation	=	oEffect;

		oAnimation.m_dTimeAccel		=	oEffect.m_nDuration * oEffect.m_dTimeAccel;
		oAnimation.m_dTimeDecel		=	oEffect.m_nDuration * oEffect.m_dTimeDecel;

		if (0 == oEffect.m_nSchemeColor)		//	RGB
		{
			oAnimation.m_nColorTo	=	oEffect.m_nColorTo;
		}
		else if (2 == oEffect.m_nSchemeColor)	//	Index From Table
		{
			if ((int)oAnimation.m_nColorTo >= (int)m_oSchemeColors.size())
				continue;

			oAnimation.m_nColorTo	=	m_oSchemeColors[oEffect.m_nColorTo].GetLONG();
		}

		pElement->m_oAnimations.m_dSlideWidth		=	Width;
		pElement->m_oAnimations.m_dSlideHeight		=	Height;

		pElement->m_oAnimations.m_arAnimations.push_back ( oAnimation );
	}
}

void CPPTUserInfo::AddAudioTransition (_UINT32 dwSlideID, CTransition* pTransition, const std::wstring& strFilePath)
{
	if (NULL==pTransition) 
		return;

	CAudioElement* pAudio = new CAudioElement ();
	if (pAudio)
	{
		pAudio->m_strAudioFileName	=	strFilePath;				
	}
	// ??? недоделка ???

	delete pAudio;
}

void CPPTUserInfo::CreateDefaultStyle(NSPresentationEditor::CTextStyles& pStyle, NSPresentationEditor::CTheme* pTheme)
{
	for (int i = 0; i < 10; ++i)
	{
		if (!pStyle.m_pLevels[i].is_init())
			pStyle.m_pLevels[i] = new NSPresentationEditor::CTextStyleLevel();

		NSPresentationEditor::CTextPFRun* pPF = &pStyle.m_pLevels[i]->m_oPFRun;
		NSPresentationEditor::CTextCFRun* pCF = &pStyle.m_pLevels[i]->m_oCFRun;

		pCF->Language		= m_wLanguage;

		pCF->Size			= 18;

		pCF->FontProperties = new NSPresentationEditor::CFontProperties();
		pCF->FontProperties->SetFont(pTheme->m_arFonts[0]);
	}
}

