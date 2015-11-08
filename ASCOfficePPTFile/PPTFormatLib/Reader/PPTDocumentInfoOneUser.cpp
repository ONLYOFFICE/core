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
								m_arOffsetPictures()
{
	m_pDocumentInfo			= NULL;
	m_lIndexThisUser		= -1;

	m_nWriteSlideTimeOffset		=	0.0;
	m_nWriteSlideTime			=	0.0;

	m_strFileDirectory		= _T("");
    m_bIsSetupEmpty			= false;

	m_bRtl					= false;
	m_bShowComments			= false;

	m_bHasDate				= false;
	m_bHasSlideNumber		= false;
	m_bHasFooter			= false;
	m_nFormatDate			= 1;
}

CPPTUserInfo::~CPPTUserInfo()
{
	Clear();
}

void CPPTUserInfo::Clear()
{
	CDocument::Clear();
	
	for (std::map<DWORD, CRecordSlide*>::iterator pPair = m_mapSlides.begin(); pPair != m_mapSlides.end(); ++pPair)
	{
		RELEASEINTERFACE(pPair->second);
	}
	m_mapSlides.clear();
	m_arrSlidesOrder.clear();

	for (std::map<DWORD, CRecordSlide*>::iterator pPair = m_mapMasters.begin(); pPair != m_mapMasters.end(); ++pPair)
	{
		RELEASEINTERFACE(pPair->second);
	}
	m_mapMasters.clear();
	m_arrMastersOrder.clear();

	for (std::map<DWORD, CRecordSlide*>::iterator pPair = m_mapNotes.begin(); pPair != m_mapNotes.end(); ++pPair)
	{
		RELEASEINTERFACE(pPair->second);
	}
	m_mapNotes.clear();
	m_arrNotesOrder.clear();


	m_oExMedia.Clear();
	m_arrFonts.clear();

	for (int nIndex = 0; nIndex < m_mapAnimations.size(); ++nIndex)
	{
		RELEASEOBJECT ( m_mapAnimations[nIndex]);
	}

	m_mapAnimations.clear();
	m_mapTransitions.clear ();
	m_arOffsetPictures.clear();
}

bool CPPTUserInfo::ReadFromStream(CRecordUserEditAtom* pUser, POLE::Stream* pStream, CStringW strFolderMem)
{
	m_oUser.FromAtom(pUser);

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

	std::map<DWORD, DWORD>::iterator pPair = m_mapOffsetInPIDs.find(m_oUser.m_nDocumentRef);

	if (pPair == m_mapOffsetInPIDs.end())
        return false;

	StreamUtils::StreamSeek(pPair->second, pStream);
	oHeader.ReadFromStream(pStream);

	if (RECORD_TYPE_DOCUMENT != oHeader.RecType)
	{
        return false;
	}

	m_oDocument.ReadFromStream(oHeader, pStream);

	Clear();

	for (size_t index = 0; index < m_oDocument.m_arMasterPersists.size(); ++index)
	{
		std::map<DWORD, DWORD>::iterator nPsrRef = m_mapOffsetInPIDs.find(m_oDocument.m_arMasterPersists[index].m_nPsrRef);
		
		if (nPsrRef != m_mapOffsetInPIDs.end())
		{
			long offset = (long)nPsrRef->second;

			StreamUtils::StreamSeek(offset, pStream);

			oHeader.ReadFromStream(pStream);
			CRecordSlide* pSlide = new CRecordSlide();
			pSlide->ReadFromStream(oHeader, pStream);
			pSlide->m_oPersist = m_oDocument.m_arMasterPersists[index];

			pSlide->m_Index		= m_mapMasters.size();			
			m_mapMasters.insert(m_mapMasters.end(), std::pair<DWORD, CRecordSlide*>(m_oDocument.m_arMasterPersists[index].m_nSlideID, pSlide));
			pSlide = NULL;
		}
		else
		{
			m_mapMasters.insert(m_mapMasters.end(), std::pair<DWORD, CRecordSlide*>(m_oDocument.m_arMasterPersists[index].m_nSlideID, NULL));
		}
		m_arrMastersOrder.push_back(m_oDocument.m_arMasterPersists[index].m_nSlideID);
	}

	for (size_t index = 0; index < m_oDocument.m_arNotePersists.size(); ++index)
	{
		std::map<DWORD, DWORD>::iterator nIndexPsrRef = m_mapOffsetInPIDs.find(m_oDocument.m_arNotePersists[index].m_nPsrRef);
		
		if (m_mapOffsetInPIDs.end() != nIndexPsrRef)
		{
			long offset = (long)nIndexPsrRef->second;
			StreamUtils::StreamSeek(offset, pStream);

			oHeader.ReadFromStream(pStream);
			CRecordSlide* pSlide = new CRecordSlide();
			pSlide->ReadFromStream(oHeader, pStream);
			pSlide->m_oPersist = m_oDocument.m_arNotePersists[index];

			pSlide->m_Index		= m_mapNotes.size();			
			m_mapNotes.insert(std::pair<DWORD, CRecordSlide*>(m_oDocument.m_arNotePersists[index].m_nSlideID, pSlide));
			pSlide = NULL;
		}
		else
		{
			m_mapNotes.insert(std::pair<DWORD, CRecordSlide*>(m_oDocument.m_arNotePersists[index].m_nSlideID, NULL));
		}
		m_arrNotesOrder.push_back(m_oDocument.m_arNotePersists[index].m_nSlideID);
	}

	for (size_t index = 0; index < m_oDocument.m_arSlidePersists.size(); ++index)
	{
		std::map<DWORD, DWORD>::iterator nIndexPsrRef = m_mapOffsetInPIDs.find(m_oDocument.m_arSlidePersists[index].m_nPsrRef);
		
		if (m_mapOffsetInPIDs.end() != nIndexPsrRef)
		{
			long offset = (long)nIndexPsrRef->second;
			
			StreamUtils::StreamSeek(offset, pStream);

			oHeader.ReadFromStream(pStream);

			CRecordSlide* pSlide = new CRecordSlide();
			pSlide->ReadFromStream(oHeader, pStream);
			pSlide->m_oPersist	= m_oDocument.m_arSlidePersists[index];
			
			pSlide->m_Index		= m_mapSlides.size(); // in m_arrSlidesOrder			
			
			m_mapSlides.insert( std::pair<DWORD, CRecordSlide*>(m_oDocument.m_arSlidePersists[index].m_nSlideID, pSlide ));

			if ( pSlide->m_bExistsTransition )
			{
				m_mapTransitions.insert (std::pair<DWORD, CSlideShowSlideInfoAtom>( (DWORD)index, pSlide->m_oSlideShowSlideInfoAtom ));
			}

			if ( pSlide->m_pSlideProgTagsContainer )
			{
				Animations::CSlideTimeLine* pEffects = pSlide->m_pSlideProgTagsContainer->GetTimeLine ();
				if (pEffects)
				{
					m_mapAnimations.insert(std::pair<DWORD, Animations::CSlideTimeLine*>((DWORD)index, pEffects));
				}
			}
		}
		else
		{
			m_mapSlides.insert( std::pair<DWORD, CRecordSlide*>(m_oDocument.m_arSlidePersists[index].m_nSlideID, NULL));
		}
		m_arrSlidesOrder.push_back(m_oDocument.m_arSlidePersists[index].m_nSlideID);
	}

	// так... теперь берем всю инфу о ExObject -----------------------------
	m_oExMedia.m_strPresentationDirectory	= strFolderMem;
	m_oExMedia.m_strSourceDirectory			= m_strFileDirectory;

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

	for (int nIndex = 0; nIndex < oArrayFonts.size(); ++nIndex)
	{
		CFont oFont;
		oFont.Name = oArrayFonts[nIndex]->m_strFaceName;
		oFont.Charset	= oArrayFonts[nIndex]->m_lfCharSet;

		switch (oArrayFonts[nIndex]->m_lfPitchAndFamily / 0x10)
		{
		case 1: {oFont.PitchFamily = _T("roman"); break;}
		case 2: {oFont.PitchFamily = _T("swiss"); break;}
		case 3: {oFont.PitchFamily = _T("modern"); break;}
		case 4: {oFont.PitchFamily = _T("script"); break;}
		case 5: {oFont.PitchFamily = _T("decorative"); break;}
		default: {oFont.PitchFamily = _T("unknown"); break;}
		}

		m_arrFonts.push_back(oFont);
	}

	//FromDocument();
	// FromDocument - должен вызываться после того, как загрузятся все (!!!) юзеры

	// теперь заполним пустые картинки
	//std::vector<CRecordBlipStoreContainer*> oArray;
	m_oDocument.GetRecordsByType(&m_arrBlipStore, true, true);
	if (0 < m_arrBlipStore.size())
	{
		m_bIsSetupEmpty = TRUE;
		m_arrBlipStore[0]->SetUpPicturesInfos(&m_arOffsetPictures);
	}

	return TRUE;
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
		std::vector<CRecordHeadersFootersAtom*> oHeadersFootersInfo;
		oArrayHeadersFootersInfo[0]->GetRecordsByType(&oHeadersFootersInfo, true, false);

		if (!oHeadersFootersInfo.empty())
		{
			m_bHasDate			= oHeadersFootersInfo[0]->m_bHasDate || oHeadersFootersInfo[0]->m_bHasTodayDate || oHeadersFootersInfo[0]->m_bHasUserDate;
			m_bHasFooter		= oHeadersFootersInfo[0]->m_bHasFooter;
			m_bHasSlideNumber	= oHeadersFootersInfo[0]->m_bHasSlideNumber;

			if (oHeadersFootersInfo[0]->m_bHasUserDate)	m_nFormatDate = 2;
		}

		std::vector<CRecordCString*> oHeadersFootersStr;
		oArrayHeadersFootersInfo[0]->GetRecordsByType(&oHeadersFootersStr, true, false);

		for (int i = 0 ;i < oHeadersFootersStr.size(); i++)
		{
			m_oFootersHeaderString.push_back(oHeadersFootersStr[i]->m_strText);
		}
	}

	LONG lOriginWidth		=	oArrayDoc[0]->m_oSlideSize.X;
	LONG lOriginHeight		=	oArrayDoc[0]->m_oSlideSize.Y;

	m_oInfo.m_lUnitsHor			= lOriginWidth;
	m_oInfo.m_lUnitsVer			= lOriginHeight;
	m_oInfo.m_lMillimetresHor	= (LONG)(c_dMasterUnitsToMillimetreKoef * lOriginWidth);
	m_oInfo.m_lMillimetresVer	= (LONG)(c_dMasterUnitsToMillimetreKoef * lOriginHeight);

	m_bRtl						= (oArrayDoc[0]->m_bRightToLeft!=0);
	m_bShowComments				= (oArrayDoc[0]->m_bShowComments!=0);

	LoadMasters(lOriginWidth, lOriginHeight);

	double DurationSlide	=	PPT_DEFAULT_SLIDE_DURATION;

	m_arSlides.reserve(m_arrSlidesOrder.size());
	for (int i=0; i< m_arrSlidesOrder.size(); i++)
	{
		std::map<DWORD, CRecordSlide*>::iterator pPair = m_mapSlides.find(m_arrSlidesOrder[i]);
		
		if (pPair ==  m_mapSlides.end()) 
			continue;

		LoadSlideFromPrevUsers	( pPair->first );

		DurationSlide	=	PPT_DEFAULT_SLIDE_DURATION;

		CSlide *elm = new CSlide();
		m_arSlides.push_back(elm);
		// тут его заполняем...

		// если на слайде есть анимации
		std::map <DWORD, Animations::CSlideTimeLine*>::iterator pTimeLine		=	m_mapAnimations.find( pPair->first);

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

		pSlide->m_lOriginalWidth	= lOriginWidth;
		pSlide->m_lOriginalHeight	= lOriginHeight;

		pSlide->m_lWidth			= (LONG)(c_dMasterUnitsToMillimetreKoef * lOriginWidth);
		pSlide->m_lHeight			= (LONG)(c_dMasterUnitsToMillimetreKoef * lOriginHeight);

		LoadSlide ( pPair->first, m_arSlides.back());
	}

	CalculateEditor(m_oInfo);
}


void CPPTUserInfo::LoadSlide(DWORD dwSlideID, CSlide* pSlide)
{
	std::map<DWORD, CRecordSlide*>::iterator pPairSlide = m_mapSlides.find(dwSlideID);

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

		pSlide->m_oSlideShow.m_dSlideDuration = pAtom->m_nSlideTime;

		switch (pAtom->m_nSpeed)
		{
		case 0x00:
			pTransition->m_dSpeed = 750.0;
			break;
		case 0x01:
			pTransition->m_dSpeed = 500.0;	
			break;
		case 0x02:
			pTransition->m_dSpeed = 250.0;
			break;
		default:
			pTransition->m_dSpeed = 500.0;
			break;
		};
	
	}
	else
	{
		pTransition->m_dSpeed = 0.0;
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

	int indexUser						= pRecordSlide->m_IndexUser;

	pSlideWrapper->m_parEmptyPictures	= &m_pDocumentInfo->m_arUsers[indexUser]->m_arOffsetPictures;
	pSlideWrapper->m_mapFilePictures	= &m_pDocumentInfo->m_mapStoreImageFile;

	// вот, грузим placeholder
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

	std::map<DWORD, LONG>::iterator pPairTheme = m_mapMasterToTheme.find(oArraySlideAtoms[0]->m_nMasterIDRef);

	if (pPairTheme == m_mapMasterToTheme.end())
	{
		//????? слайду не присвоена тема !!!
		pPairTheme = m_mapMasterToTheme.begin();
	}
//-----------------	
	pSlide->m_lThemeID			= pPairTheme->second;
	
	CTheme		* pTheme		= &m_arThemes		[pSlide->m_lThemeID];
	CSlideInfo	* pThemeWrapper	= &m_arMasterWrapper[pSlide->m_lThemeID];

	CLayout* pLayout	= NULL;

	std::map<DWORD, LONG>::iterator		pPairLayoutTitle	= pTheme->m_mapTitleLayout.find(oArraySlideAtoms[0]->m_nMasterIDRef);
	if (pPairLayoutTitle != pTheme->m_mapTitleLayout.end())
	{
		//основан на заголовочном шаблоне
		pSlide->m_lLayoutID = pPairLayoutTitle->second;
		pLayout				= &pTheme->m_arLayouts[pSlide->m_lLayoutID];
	}
	else
	{
		//основан на типовом шаблоне
		std::map<_UINT64, LONG>::iterator	pPairLayoutGeom	= pTheme->m_mapGeomToLayout.find(oArraySlideAtoms[0]->m_oLayout.m_hash);
		
		if (pPairLayoutGeom == pTheme->m_mapGeomToLayout.end())
		{
			pSlide->m_lLayoutID = AddNewLayout(pTheme, pRecordSlide, true, bMasterObjects);

			pLayout				= &pTheme->m_arLayouts[pSlide->m_lLayoutID];
			pLayout->m_bShowMasterShapes	= true;
		}
		else
		{
			pSlide->m_lLayoutID = pPairLayoutGeom->second;
			pLayout				= &pTheme->m_arLayouts[pSlide->m_lLayoutID];
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

		for (int i = 0; i < oArrayColors.size(); ++i)
		{
			if (0x01 == oArrayColors[i]->m_oHeader.RecInstance)
			{
				pSlide->m_bUseLayoutColorScheme = false;
				oArrayColors[i]->ToArray(&pSlide->m_arColorScheme);
				CorrectColorScheme(pSlide->m_arColorScheme);

				// проверим на совпадение
				size_t nCountC = pSlide->m_arColorScheme.size();
				size_t nIndexC = 0;
				if (nCountC == pArrayColorScheme->size())
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
	pSlide->m_bHasDate			= pLayout->m_bHasDate;
	pSlide->m_bHasFooter		= pLayout->m_bHasFooter;
	pSlide->m_bHasSlideNumber	= pLayout->m_bHasSlideNumber;
	pSlide->m_nFormatDate		= m_nFormatDate;

	std::vector<std::wstring> users_placeholders = m_oFootersHeaderString;
//------------------------------------------------------------------------------------
	std::vector<CRecordHeadersFootersContainer*> oArrayHeadersFootersInfo;
	pRecordSlide->GetRecordsByType(&oArrayHeadersFootersInfo, false, true);

	if (0 != oArrayHeadersFootersInfo.size())
	{
		std::vector<CRecordHeadersFootersAtom*> oHeadersFootersInfo;
		oArrayHeadersFootersInfo[0]->GetRecordsByType(&oHeadersFootersInfo, true, false);

		if (!oHeadersFootersInfo.empty())
		{
			pSlide->m_bHasDate			= oHeadersFootersInfo[0]->m_bHasDate || oHeadersFootersInfo[0]->m_bHasTodayDate || oHeadersFootersInfo[0]->m_bHasUserDate;
			pSlide->m_bHasFooter		= oHeadersFootersInfo[0]->m_bHasFooter;
			pSlide->m_bHasSlideNumber	= oHeadersFootersInfo[0]->m_bHasSlideNumber;

			if (oHeadersFootersInfo[0]->m_bHasUserDate)	pSlide->m_nFormatDate = 2;
			else pSlide->m_nFormatDate = 1;
		}

		std::vector<CRecordCString*> oHeadersFootersStr;
		oArrayHeadersFootersInfo[0]->GetRecordsByType(&oHeadersFootersStr, true, false);

		if (!oHeadersFootersStr.empty()) users_placeholders.clear();
		for (int i = 0 ;i < oHeadersFootersStr.size(); i++)
		{
			users_placeholders.push_back(oHeadersFootersStr[i]->m_strText);
		}
	}
//-------------------------------------------------------------------------------------------------------
	std::vector<CRecordCString*> oArrayStrings;
	pRecordSlide->GetRecordsByType(&oArrayStrings, false, false);
	
	for (int i=0; i < oArrayStrings.size(); i++)
	{
		if (oArrayStrings[i]->m_oHeader.RecType == 0x0fba)
		{
			pSlide->m_sName = oArrayStrings[i]->m_strText;
		}
	}
//------------- читаем все элементы ------------------------------------------------------------------------------------------
	std::vector<CRecordShapeContainer*> oArrayShapes;

	pRecordSlide->GetRecordsByType(&oArrayShapes, true);

	pSlide->m_bShowMasterShapes = bMasterObjects;
	pSlide->m_bIsBackground		= false;

	std::map<int,int>	slidePlaceholders;

	for (int nShape = 0; nShape < oArrayShapes.size(); ++nShape)
	{
		IElement* pElem = NULL;
		
		oArrayShapes[nShape]->GetElement(&pElem, &m_oExMedia, pSlide->m_lOriginalWidth, pSlide->m_lOriginalHeight,
												pTheme, pLayout, pThemeWrapper, pSlideWrapper, pSlide);
		
		if (NULL != pElem)
		{
			if (pElem->m_bIsBackground && !pElem->m_bHaveAnchor && !bMasterBackGround)
			{
				CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElem);
				if (NULL != pShape)
				{
					pShape->SetupProperties(pSlide, pTheme, pLayout);

					pSlide->m_bIsBackground = true;
					pSlide->m_oBackground	= pShape->m_oBrush;
				}
				RELEASEOBJECT(pElem);
				continue;			

			}else
				AddAnimation ( dwSlideID, pSlide->m_lOriginalWidth, pSlide->m_lOriginalHeight, pElem );

			if (pElem->m_bHaveAnchor)
			{
				pSlide->m_arElements.push_back(pElem);
			}
			if (pElem->m_lPlaceholderType > 0 )
			{
				if (pElem->m_lPlaceholderUserStr >= 0 && pElem->m_lPlaceholderUserStr < users_placeholders.size())
				{
					CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElem);
					if (NULL != pShape)
						pShape->SetUpText(users_placeholders[pElem->m_lPlaceholderUserStr]);
				}				
				slidePlaceholders.insert(std::pair<int,int>(pElem->m_lPlaceholderType, pSlide->m_arElements.size() - 1) );
			}
		}
	}
//------------- колонтитулы на слайде 
	std::map<int, int>::iterator it;
	IElement* pElement = NULL;
	
	if (pSlide->m_bHasSlideNumber)
	{
		it = pLayout->m_mapPlaceholders.find(NSOfficePPT::MasterSlideNumber);
		if ( it != pLayout->m_mapPlaceholders.end() && 
			slidePlaceholders.find(NSOfficePPT::MasterSlideNumber) == slidePlaceholders.end())
		{
			pElement  = pLayout->m_arElements[it->second]->CreateDublicate();
			pSlide->m_arElements.push_back(pElement);
		}
	}
	
	if (pSlide->m_bHasDate)
	{
		it = pLayout->m_mapPlaceholders.find(MasterDate);
		if ( it != pLayout->m_mapPlaceholders.end() && 
			slidePlaceholders.find(NSOfficePPT::MasterDate) == slidePlaceholders.end())
		{
			pElement  = pLayout->m_arElements[it->second]->CreateDublicate();
			pSlide->m_arElements.push_back(pElement);
		}
		else
		{
			it = slidePlaceholders.find(NSOfficePPT::MasterDate);
			pElement = pSlide->m_arElements[it->second];
		}
		pElement->m_nFormatDate = pSlide->m_nFormatDate;
	}

	if (pSlide->m_bHasFooter)
	{
		it = pLayout->m_mapPlaceholders.find(MasterFooter);
		if ( it != pLayout->m_mapPlaceholders.end() && 
			slidePlaceholders.find(NSOfficePPT::MasterFooter) == slidePlaceholders.end())
		{
			pElement  = pLayout->m_arElements[it->second]->CreateDublicate();
			pSlide->m_arElements.push_back(pElement);
		}
	}
}
	
IElement* CPPTUserInfo::AddThemeLayoutPlaceholder (CLayout *pLayout, int placeholderType, NSPresentationEditor::CTheme* pTheme)
{
	IElement* pElement  = NULL;
	std::map<int, int>::iterator it = pTheme->m_mapPlaceholders.find(placeholderType);
	
	if ((it != pTheme->m_mapPlaceholders.end()) && (pLayout->m_mapPlaceholders.find(placeholderType) == pLayout->m_mapPlaceholders.end())) 
	{
		pElement = pTheme->m_arElements[it->second]->CreateDublicate();
		pLayout->m_arElements.push_back(dynamic_cast<IElement*>(pElement));
		pLayout->m_mapPlaceholders.insert(std::pair<int, int>(it->first, pLayout->m_arElements.size()-1)); 
	}
	return pElement;
}

IElement* CPPTUserInfo::AddNewLayoutPlaceholder (CLayout *pLayout, int placeholderType, int placeholderSizePreset)
{
	if (placeholderType < 1) return NULL;

	CShapeElement* pShape = new CShapeElement(NSBaseShape::ppt, PPTShapes::sptCRect);

	pShape->m_lPlaceholderType			= placeholderType;
	pShape->m_lPlaceholderSizePreset	= placeholderSizePreset;
	
	//if (pShape->m_lPlaceholderSizePreset > 0)
	//	pShape->m_bPlaceholderSet		= true;
	//else
		pShape->m_bPlaceholderSet		= false;
	pShape->m_bLine						= false;
	pShape->m_bBoundsEnabled			= false;

	CorrectPlaceholderType(pShape->m_lPlaceholderType);

	pLayout->m_arElements.push_back(dynamic_cast<IElement*>(pShape));
	pLayout->m_mapPlaceholders.insert(std::pair<int, int>(pShape->m_lPlaceholderType, pLayout->m_arElements.size()-1)); 

	return pShape;
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
	
	CLayout layout;
	pTheme->m_arLayouts.push_back(layout);
	CLayout *pLayout = &pTheme->m_arLayouts.back();
	
	pLayout = &pTheme->m_arLayouts.back();

	pLayout->m_lOriginalWidth	= pTheme->m_lOriginalWidth;
	pLayout->m_lOriginalHeight	= pTheme->m_lOriginalHeight;

	pLayout->m_lWidth			= (LONG)(c_dMasterUnitsToMillimetreKoef * pLayout->m_lOriginalWidth);
	pLayout->m_lHeight			= (LONG)(c_dMasterUnitsToMillimetreKoef * pLayout->m_lOriginalHeight);

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

	pLayout->m_bHasDate			= pTheme->m_bHasDate;
	pLayout->m_bHasFooter		= pTheme->m_bHasFooter;
	pLayout->m_bHasSlideNumber	= pTheme->m_bHasSlideNumber;
	pLayout->m_nFormatDate		= pTheme->m_nFormatDate;

	if (pLayout->m_bHasSlideNumber)
	{
		AddThemeLayoutPlaceholder(pLayout, MasterSlideNumber, pTheme);
	}
	if (pLayout->m_bHasDate)
	{
		AddThemeLayoutPlaceholder(pLayout, MasterDate, pTheme);
	}
	if (pLayout->m_bHasFooter)
	{
		AddThemeLayoutPlaceholder(pLayout, MasterFooter, pTheme);
	}	


	return ind;
}

IElement* CPPTUserInfo::AddNewThemePlaceholder (CTheme* pTheme, int placeholderType, int placeholderSizePreset)
{
	if (placeholderType < 1) return NULL;

	CShapeElement* pShape = new CShapeElement(NSBaseShape::ppt, PPTShapes::sptCRect);

	pShape->m_lPlaceholderType			= placeholderType;
	pShape->m_lPlaceholderSizePreset	= placeholderSizePreset;
	
	//if (pShape->m_lPlaceholderSizePreset > 0)
	//	pShape->m_bPlaceholderSet		= true;
	//else
		pShape->m_bPlaceholderSet		= false;
	pShape->m_bLine						= false;
	pShape->m_bBoundsEnabled			= false;

	CorrectPlaceholderType(pShape->m_lPlaceholderType);

	pTheme->m_arElements.push_back(dynamic_cast<IElement*>(pShape));
	pTheme->m_mapPlaceholders.insert(std::pair<int, int>(pShape->m_lPlaceholderType, pTheme->m_arElements.size()-1)); 

	return pShape;
}
void CPPTUserInfo::LoadMainMaster(DWORD dwMasterID, const LONG& lOriginWidth, const LONG& lOriginHeight)
{
	std::map<DWORD, LONG>::iterator pPair = m_mapMasterToTheme.find(dwMasterID);
	if (pPair != m_mapMasterToTheme.end())
	{
		// мастер уже загружен
		return;
	}

	LoadMasterFromPrevUsers(dwMasterID);

	std::map<DWORD, CRecordSlide*>::iterator pPairMaster = m_mapMasters.find(dwMasterID);

	if (m_mapMasters.end() == pPairMaster)//??? не может быть 
		return;

	CRecordSlide* pMaster = pPairMaster->second;

    if (pMaster == NULL)//??? не может быть 
        return; 

	std::vector<CRecordSlideAtom*> oArraySlideAtoms;
	pMaster->GetRecordsByType(&oArraySlideAtoms, true);
	if (0 == oArraySlideAtoms.size())
		return;

	DWORD dwID				= (DWORD)oArraySlideAtoms[0]->m_nMasterIDRef;
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
		m_mapMasterOriginalIds.insert(std::pair<DWORD, DWORD>(oArrayOrigId[0]->m_dwID, dwMasterID));

	LONG lIndexTheme = (LONG)m_arThemes.size();
	
	m_mapMasterToTheme.insert(std::pair<DWORD, LONG>(dwMasterID, lIndexTheme));

	CTheme theme;
	m_arThemes.push_back(theme);
	CTheme* pTheme = &m_arThemes[lIndexTheme];

	pTheme->m_lOriginalWidth	= lOriginWidth;
	pTheme->m_lOriginalHeight	= lOriginHeight;

	pTheme->m_bHasDate			= m_bHasDate;
	pTheme->m_bHasFooter		= m_bHasFooter;
	pTheme->m_bHasSlideNumber	= m_bHasSlideNumber;
	pTheme->m_nFormatDate		= m_nFormatDate;

	std::vector<std::wstring> users_placeholders = m_oFootersHeaderString;

	std::vector<CRecordHeadersFootersContainer*> oArrayHeadersFootersInfo;
	pMaster->GetRecordsByType(&oArrayHeadersFootersInfo, false, true);

	if (0 != oArrayHeadersFootersInfo.size())
	{
		std::vector<CRecordHeadersFootersAtom*> oHeadersFootersInfo;
		oArrayHeadersFootersInfo[0]->GetRecordsByType(&oHeadersFootersInfo, true, false);

		if (!oHeadersFootersInfo.empty())
		{
			pTheme->m_bHasDate			= oHeadersFootersInfo[0]->m_bHasDate || oHeadersFootersInfo[0]->m_bHasTodayDate || oHeadersFootersInfo[0]->m_bHasUserDate;
			pTheme->m_bHasFooter		= oHeadersFootersInfo[0]->m_bHasFooter;
			pTheme->m_bHasSlideNumber	= oHeadersFootersInfo[0]->m_bHasSlideNumber;
			
			if (oHeadersFootersInfo[0]->m_bHasUserDate ) 
				m_nFormatDate = 2;
			else m_nFormatDate = 1;
		}

		std::vector<CRecordCString*> oHeadersFootersStr;
		oArrayHeadersFootersInfo[0]->GetRecordsByType(&oHeadersFootersStr, true, false);

		if (!oHeadersFootersStr.empty()) users_placeholders.clear();
		for (int i = 0 ;i < oHeadersFootersStr.size(); i++)
		{
			users_placeholders.push_back(oHeadersFootersStr[i]->m_strText);
		}
	}
	std::vector<CRecordCString*> oArrayStrings;
	pMaster->GetRecordsByType(&oArrayStrings, false, false);
	for (int i=0; i < oArrayStrings.size(); i++)
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
	std::map<DWORD, CRecordSlide*>::iterator pPairMaster1 = m_mapMasters.find(dwMasterID);

	int indexUser = 0;
	if (pPairMaster1 != m_mapMasters.end())
	{
		indexUser = pPairMaster1->second->m_IndexUser;
		pMasterWrapper->m_arTextPlaceHolders = pPairMaster1->second->m_oPersist.m_arTextAttrs;
	}
	pMasterWrapper->m_parEmptyPictures	= &m_pDocumentInfo->m_arUsers[indexUser]->m_arOffsetPictures;
	pMasterWrapper->m_mapFilePictures	= &m_pDocumentInfo->m_mapStoreImageFile;

	// читаем настройки текстовых стилей -----------------------------------------------
	std::vector<CRecordTextMasterStyleAtom*> oArrayTextMasters;
	pMaster->GetRecordsByType(&oArrayTextMasters, true, false);

	for (int i = 0; i < oArrayTextMasters.size(); ++i)
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

	pTheme->m_oFootersHeaderString = m_oFootersHeaderString;

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

	// ---------------------------------------------------------------------------------
	// читаем все элементы...-----------------------------------------------------------
	std::vector<CRecordShapeContainer*> oArrayShapes;
	pMaster->GetRecordsByType(&oArrayShapes, true);

	pTheme->CalculateStyles();
	
	CLayout* pLayout = NULL; // ну нету тут разметок ...!!

	for (int nShape = 0; nShape < oArrayShapes.size(); ++nShape)
	{
		NSPresentationEditor::IElement* pElem = NULL;
		oArrayShapes[nShape]->GetElement(&pElem, &m_oExMedia, lOriginWidth, lOriginHeight, pTheme, pLayout, pMasterWrapper, pMasterWrapper);

		if (NULL != pElem)
		{
			AddAnimation ( dwMasterID, lOriginWidth, lOriginHeight, pElem );
			
			if (pElem->m_bIsBackground && !pElem->m_bHaveAnchor)
			{
				CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElem);
				if (NULL != pShape)
				{
					pShape->SetupProperties(NULL, pTheme, pLayout);

					pTheme->m_bIsBackground = true;
					pTheme->m_oBackground = pShape->m_oBrush;
				}

				RELEASEINTERFACE(pElem);
				continue;
			}
			pTheme->m_arElements.push_back(pElem);
			
			if ( pElem->m_lPlaceholderType >0)
			{
				if (pElem->m_lPlaceholderUserStr >= 0 && pElem->m_lPlaceholderUserStr < users_placeholders.size())
				{
					CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElem);
					if (NULL != pShape)
						pShape->SetUpText(users_placeholders[pElem->m_lPlaceholderUserStr]);
				}	
				pTheme->m_mapPlaceholders.insert(std::pair<int, int>(pElem->m_lPlaceholderType, pTheme->m_arElements.size()-1)); 
			}
		}
	}

	if (pTheme->m_bHasSlideNumber)
	{
		if (pTheme->m_mapPlaceholders.find(MasterSlideNumber) == pTheme->m_mapPlaceholders.end())
		{
			AddNewThemePlaceholder(pTheme, MasterSlideNumber, 2);
		}
	}
	if (pTheme->m_bHasDate && pTheme->m_nFormatDate == 1)
	{
		if (pTheme->m_mapPlaceholders.find(MasterDate) == pTheme->m_mapPlaceholders.end())
		{
			AddNewThemePlaceholder(pTheme, MasterDate, 2); 
		}
	}
	if (pTheme->m_bHasFooter)
	{
		if (pTheme->m_mapPlaceholders.find(MasterFooter) == pTheme->m_mapPlaceholders.end())
		{
			AddNewThemePlaceholder(pTheme, MasterFooter, 1);
		}		
	}
}

void CPPTUserInfo::LoadMasters(const LONG& lOriginWidth, const LONG& lOriginHeight)
{
	for (long i=0; i< m_arrMastersOrder.size(); i++)
	{
		std::map<DWORD, CRecordSlide*>::iterator pPair = m_mapMasters.find(m_arrMastersOrder[i]);
		if (pPair == m_mapMasters.end())continue;			
	
		LoadMainMaster(pPair->first, lOriginWidth, lOriginHeight);
	}

	for (long i=0; i< m_arrMastersOrder.size(); i++)
	{
		std::map<DWORD, CRecordSlide*>::iterator pPair = m_mapMasters.find(m_arrMastersOrder[i]);
		if (pPair == m_mapMasters.end())continue;			
	
		LoadNoMainMaster(pPair->first, lOriginWidth, lOriginHeight);
	}
}

void CPPTUserInfo::LoadNoMainMaster(DWORD dwMasterID, const LONG& lOriginWidth, const LONG& lOriginHeight)
{
	std::map<DWORD, CRecordSlide*>::iterator pPair = m_mapMasters.find(dwMasterID);

	if (pPair == m_mapMasters.end())
		return;

	CRecordSlide* pCurMaster = pPair->second;

	if (pCurMaster == NULL)
		return;
	DWORD dwCurID = pCurMaster->m_oPersist.m_nSlideID;

	std::vector<CRecordSlideAtom*> oArraySlideAtoms;

	pCurMaster->GetRecordsByType(&oArraySlideAtoms, true);
	if (0 == oArraySlideAtoms.size())
		return;

    bool bMasterColorScheme = oArraySlideAtoms[0]->m_bMasterScheme;
    bool bMasterBackGround	= oArraySlideAtoms[0]->m_bMasterBackground;
    bool bMasterObjects		= oArraySlideAtoms[0]->m_bMasterObjects;

	DWORD dwID = (DWORD)oArraySlideAtoms[0]->m_nMasterIDRef;

	if (0 == dwID)
	{
		std::vector<CRecordTripCompositeMasterId12Atom*> oArrayCompId;
		pCurMaster->GetRecordsByType(&oArrayCompId, false, true);
		if (0 != oArrayCompId.size())
		{
			std::map<DWORD, DWORD>::iterator pPair1 = m_mapMasterOriginalIds.find(oArrayCompId[0]->m_dwID);
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

	// вот, грузим placeholder
	pMasterWrapper->m_arTextPlaceHolders	= pCurMaster->m_oPersist.m_arTextAttrs;	
	
	pMasterWrapper->m_parEmptyPictures		= &m_pDocumentInfo->m_arUsers[pCurMaster->m_IndexUser]->m_arOffsetPictures;
	pMasterWrapper->m_mapFilePictures		= &m_pDocumentInfo->m_mapStoreImageFile;

	std::map<DWORD, LONG>::iterator pPairTheme = m_mapMasterToTheme.find(dwID);
	
	if (m_mapMasterToTheme.end() == pPairTheme)
		return;

	m_mapMasterToTheme.insert(std::pair<DWORD, LONG>(dwMasterID, pPairTheme->second));

	CSlideInfo	* pThemeWrapper	= &m_arMasterWrapper[pPairTheme->second];
	CTheme		* pTheme		= &m_arThemes		[pPairTheme->second];

	std::wstring strLayoutType = ConvertLayoutType(oArraySlideAtoms[0]->m_oLayout.m_nGeom, oArraySlideAtoms[0]->m_oLayout.m_pPlaceHolderID);
	
	CLayout* pLayout = NULL;

	int lLayoutID	= AddNewLayout(pTheme, pCurMaster, false, false);
	
	pLayout							= &pTheme->m_arLayouts[lLayoutID];
	pLayout->m_bShowMasterShapes	=	false;

	pTheme->m_mapTitleLayout[dwMasterID] = lLayoutID;

	std::vector<NSPresentationEditor::CColor>* pArrayColorScheme = &pTheme->m_arColorScheme;
	// читаем цветовую схему -----------------------------------------------------------
	if (!bMasterColorScheme)
	{
		std::vector<CRecordColorSchemeAtom*> oArrayColors;
		pCurMaster->GetRecordsByType(&oArrayColors, false);

		pLayout->m_arColorScheme.clear();
		for (int i = 0; i < oArrayColors.size(); ++i)
		{
			if (0x01 == oArrayColors[i]->m_oHeader.RecInstance)
			{
				oArrayColors[i]->ToArray(&pLayout->m_arColorScheme);
				pLayout->m_bUseThemeColorScheme = false;
				CorrectColorScheme(pLayout->m_arColorScheme);

				// проверим на совпадение
				size_t nCountC = pLayout->m_arColorScheme.size();
				size_t nIndexC = 0;
				if (nCountC == pArrayColorScheme->size())
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
	pCurMaster->GetRecordsByType(&oArrayHeadersFootersInfo, false, true);

	if (0 != oArrayHeadersFootersInfo.size())
	{
		std::vector<CRecordHeadersFootersAtom*> oHeadersFootersInfo;
		oArrayHeadersFootersInfo[0]->GetRecordsByType(&oHeadersFootersInfo, true, false);

		if (!oHeadersFootersInfo.empty())
		{
			pLayout->m_bHasDate			= oHeadersFootersInfo[0]->m_bHasDate || oHeadersFootersInfo[0]->m_bHasTodayDate || oHeadersFootersInfo[0]->m_bHasUserDate;
			pLayout->m_bHasFooter		= oHeadersFootersInfo[0]->m_bHasFooter;
			pLayout->m_bHasSlideNumber	= oHeadersFootersInfo[0]->m_bHasSlideNumber;
			
			if (oHeadersFootersInfo[0]->m_bHasUserDate) pLayout->m_nFormatDate = 2;
		}

		std::vector<CRecordCString*> oHeadersFootersStr;
		oArrayHeadersFootersInfo[0]->GetRecordsByType(&oHeadersFootersStr, true, false);

		for (int i = 0 ;i < oHeadersFootersStr.size(); i++)
		{
			m_oFootersHeaderString.push_back(oHeadersFootersStr[i]->m_strText);
		}
	}	
	
	std::vector<CRecordCString*> oArrayStrings;
	pCurMaster->GetRecordsByType(&oArrayStrings, false, false);
	
	for (int i=0; i < oArrayStrings.size(); i++)
	{
		if (oArrayStrings[i]->m_oHeader.RecType == 0x0fba)
		{
			pLayout->m_sName = oArrayStrings[i]->m_strText;
		}
	}
	// читаем все элементы...
	std::vector<CRecordShapeContainer*> oArrayShapes;
	pCurMaster->GetRecordsByType(&oArrayShapes, true);

	for (int nShape = 0; nShape < oArrayShapes.size(); ++nShape)
	{
		IElement* pElem = NULL;
		oArrayShapes[nShape]->GetElement(&pElem, &m_oExMedia, lOriginWidth, lOriginHeight, pTheme, pLayout, pThemeWrapper, pMasterWrapper);

		if (NULL != pElem)
		{
			AddAnimation ( dwMasterID, lOriginWidth, lOriginHeight, pElem );

			if (pElem->m_bIsBackground && !pElem->m_bHaveAnchor)
			{
				CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElem);

				if (NULL != pShape)
				{
					pShape->SetupProperties(NULL, pTheme, pLayout);

					pLayout->m_bIsBackground = true;
					pLayout->m_oBackground	= pShape->m_oBrush;
				}
				RELEASEINTERFACE(pElem);
				continue;
			}

			pElem->m_bPlaceholderSet = true;
			pLayout->m_arElements.push_back(pElem);
			
			if ( pElem->m_lPlaceholderType >0)
				pLayout->m_mapPlaceholders.insert(std::pair<int, int>(pElem->m_lPlaceholderType, pLayout->m_arElements.size()-1)); 

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

void CPPTUserInfo::LoadSlideFromPrevUsers(DWORD dwSlideID)
{
	if ((NULL == m_pDocumentInfo) || (-1 == m_lIndexThisUser))
		return;

	std::map<DWORD, CRecordSlide*>::iterator pPairSlide = m_mapSlides.find(dwSlideID);
	
	if (pPairSlide != m_mapSlides.end() && pPairSlide->second)
		return; //есть

	size_t lUsersCount = m_pDocumentInfo->m_arUsers.size();
		
	for (size_t lIndexUser = m_lIndexThisUser + 1; lIndexUser < lUsersCount; ++lIndexUser)
	{
		std::map<DWORD, CRecordSlide*>::iterator pPair = m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapSlides.find(dwSlideID);
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
				m_mapSlides.insert(m_mapSlides.end(), std::pair<DWORD, CRecordSlide*>(dwSlideID, pSlideCur));
				m_arrSlidesOrder.push_back(dwSlideID);
			}
			return;
		}
	}
}
void CPPTUserInfo::LoadMasterFromPrevUsers(DWORD dwMasterID)
{
	if ((NULL == m_pDocumentInfo) || (-1 == m_lIndexThisUser))
		return;

	std::map<DWORD, CRecordSlide*>::iterator pPairMaster = m_mapMasters.find(dwMasterID);

	if (pPairMaster != m_mapMasters.end() && pPairMaster->second)
		return;//есть

	size_t lUsersCount = m_pDocumentInfo->m_arUsers.size();
	
	for (int lIndexUser = m_lIndexThisUser + 1; lIndexUser < lUsersCount; ++lIndexUser)
	{
		std::map<DWORD, CRecordSlide*>::iterator pPair = m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapMasters.find(dwMasterID);

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
				m_mapMasters.insert(m_mapMasters.end(), std::pair<DWORD, CRecordSlide*>(dwMasterID, pSlideCur));
				m_arrMastersOrder.push_back(dwMasterID);
			}
			return;
		}
	}
}
void CPPTUserInfo::LoadNoteFromPrevUsers(DWORD dwSlideID)
{
	if ((NULL == m_pDocumentInfo) || (-1 == m_lIndexThisUser))
		return;

	std::map<DWORD, CRecordSlide*>::iterator pPair = m_mapNotes.find(dwSlideID);
	if (pPair != m_mapNotes.end())
		return; //есть
	
	size_t lUsersCount = m_pDocumentInfo->m_arUsers.size();
	
	// у нас чем раньше - тем больше индекс
	for (size_t lIndexUser = m_lIndexThisUser + 1; lIndexUser < lUsersCount; lIndexUser++)
	//for (size_t lIndexUser = lUsersCount - 1; lIndexUser > m_lIndexThisUser; lIndexUser--)
	{
		pPair = m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapNotes.find(dwSlideID);
		
		if (pPair == m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapNotes.end())
			continue;

		CRecordSlide* pSlideCur = pPair->second;
		if (NULL != pSlideCur)
		{
			pSlideCur->AddRef();
			m_mapNotes.insert(std::pair<DWORD, CRecordSlide*>(dwSlideID, pSlideCur));
			m_arrNotesOrder.push_back(dwSlideID);
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
				oInfo.m_dwID		= (DWORD)XmlUtils::GetInteger(oArrayStrings[2]->m_strText.c_str());

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

	for (int nIndex = 0; nIndex < oArray.size(); ++nIndex)
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

	for (int nIndex = 0; nIndex < oArrayAudioMIDI.size(); ++nIndex)
	{
		LoadExAudio(oArrayAudioMIDI[nIndex]);
	}
	for (int nIndex = 0; nIndex < oArrayWAVLink.size(); ++nIndex)
	{
		LoadExAudio(oArrayWAVLink[nIndex]);
	}
	for (int nIndex = 0; nIndex < oArrayAudioEmbedded.size(); ++nIndex)
	{
		DWORD dwKeySound	= oArrayAudioEmbedded[nIndex]->m_nSoundID;
		DWORD dwKeyObj		= oArrayAudioEmbedded[nIndex]->m_oMedia.m_nExObjID;

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
	for (int nIndex = 0; nIndex < oArrayAudioCD.size(); ++nIndex)
	{
		DWORD dwKeyObj			= oArrayAudioCD[nIndex]->m_oMedia.m_nExObjID;

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
	
	for (int nIndex = 0; nIndex < oArrayHyperlinkContainer.size(); ++nIndex)
	{
		std::vector<CRecordExHyperlinkAtom*>	oArrayHyperlink;		
		std::vector<CRecordCString*>			oArrayCString;		
		
		oArrayHyperlinkContainer[nIndex]->GetRecordsByType	(&oArrayHyperlink, false);
		oArrayHyperlinkContainer[nIndex]->GetRecordsByType	(&oArrayCString, false);

		if (oArrayCString.size() > 0 && oArrayHyperlink.size() > 0)
		{
			NSPresentationEditor::CExFilesInfo oInfo;

			oInfo.m_dwID			= oArrayHyperlink[0]->m_nHyperlinkID;
			oInfo.m_strFilePath		= oArrayCString[0]->m_strText;

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

void CPPTUserInfo::AddAnimation ( DWORD dwSlideID, double Width, double Height, IElement* pElement )
{
	std::map <DWORD, Animations::CSlideTimeLine*>::iterator pPair =	m_mapAnimations.find( dwSlideID );

	if (pPair == m_mapAnimations.end()) return;

	Animations::CSlideTimeLine* pTimeLine	= pPair->second;
	if (pTimeLine == NULL) return;

	std::map <DWORD, Animations::Effects*>::iterator pPairA = pTimeLine->GetAnimation().find ( pElement->m_lID );
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

void CPPTUserInfo::AddAudioTransition (DWORD dwSlideID, CTransition* pTransition, const std::wstring& strFilePath)
{
	if (NULL==pTransition) 
		return;

	CAudioElement* pAudio		=	new CAudioElement ();
	if (pAudio)
	{
		pAudio->m_strAudioFileName	=	strFilePath;				
	}
	// ??? недоделка ???
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

