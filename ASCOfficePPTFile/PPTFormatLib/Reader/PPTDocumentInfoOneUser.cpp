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
								m_arEmptyPictures()
{
	m_pDocumentInfo = NULL;
	m_lIndexThisUser = -1;

	m_nWriteSlideTimeOffset		=	0.0;
	m_nWriteSlideTime			=	0.0;

	m_strFileDirectory = _T("");
    m_bIsSetupEmpty = false;
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
	m_arEmptyPictures.clear();
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

	UpdateXMLInfo();
	//FromDocument();
	// FromDocument - должен вызываться после того, как загрузятся все (!!!) юзеры

	// теперь смотрим на пустые картинки
	// теперь заполним пустые картинки
	std::vector<CRecordBlipStoreContainer*> oArray;
	m_oDocument.GetRecordsByType(&oArray, true, true);
	if (0 < oArray.size())
	{
		m_bIsSetupEmpty = TRUE;
		oArray[0]->SetUpEmptyInfos(&m_arEmptyPictures);
	}

	return TRUE;
}

void CPPTUserInfo::FromDocument()
{
	UpdateXMLInfo();
	m_arSlides.clear();

	// здесь из документа читаем слайды в
	// нормальном формате, и сразу их правильно расставляем...
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
	}

	LONG lOriginWidth		=	oArrayDoc[0]->m_oSlideSize.X;
	LONG lOriginHeight		=	oArrayDoc[0]->m_oSlideSize.Y;

	m_oInfo.m_lUnitsHor			= lOriginWidth;
	m_oInfo.m_lUnitsVer			= lOriginHeight;
	m_oInfo.m_lMillimetresHor	= (LONG)(c_dMasterUnitsToMillimetreKoef * lOriginWidth);
	m_oInfo.m_lMillimetresVer	= (LONG)(c_dMasterUnitsToMillimetreKoef * lOriginHeight);

	LoadMasters(lOriginWidth, lOriginHeight);

	double DurationSlide	=	PPT_DEFAULT_SLIDE_DURATION;

	for (int i=0; i< m_arrSlidesOrder.size(); i++)
	{
		std::map<DWORD, CRecordSlide*>::iterator pPair = m_mapSlides.find(m_arrSlidesOrder[i]);
		
		if (pPair ==  m_mapSlides.end()) 
			continue;

		LoadSlideFromPrevUsers	( pPair->first );

		DurationSlide	=	PPT_DEFAULT_SLIDE_DURATION;

		CSlide elm;
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

		CSlide* pSlide = &m_arSlides.back();

		pSlide->m_dStartTime		= 0.0;
		pSlide->m_dEndTime			= DurationSlide;
		pSlide->m_dDuration			= DurationSlide;

		pSlide->m_lOriginalWidth	= lOriginWidth;
		pSlide->m_lOriginalHeight	= lOriginHeight;

		pSlide->m_lWidth			= (LONG)(c_dMasterUnitsToMillimetreKoef * lOriginWidth);
		pSlide->m_lHeight			= (LONG)(c_dMasterUnitsToMillimetreKoef * lOriginHeight);

		LoadSlide ( pPair->first, &m_arSlides.back());
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
			pTransition->m_oAudio.m_strFileName = pInfo->m_strFilePath;
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

	pSlideWrapper->m_parEmptyPictures = &m_arEmptyPictures;

	// вот, грузим placeholder
	pSlideWrapper->m_arTextPlaceHolders = pRecordSlide->m_oPersist.m_arTextAttrs;

	// записываем шрифты
	std::vector<CRecordSlideAtom*> oArrayAtoms;

	pRecordSlide->GetRecordsByType(&oArrayAtoms, false, true);
	if (0 == oArrayAtoms.size())
	{
		// ошибка!!!
		return;
	}

	std::map<DWORD, LONG>::iterator pPair = m_mapMasterToTheme.find(oArrayAtoms[0]->m_nMasterIDRef);

	if (pPair == m_mapMasterToTheme.end())
	{
		//?????
	}
	
	pSlide->m_lThemeID = pPair->second;

	pPair = m_mapMasterToLayout.find(oArrayAtoms[0]->m_nMasterIDRef);
	if (pPair == m_mapMasterToLayout.end())
	{
		//?????
	}
	pSlide->m_lLayoutID = pPair->second;

	CSlideInfo* pThemeWrapper = &m_arMasterWrapper[pSlide->m_lThemeID];

	CTheme* pTheme = &m_arThemes[pSlide->m_lThemeID];
	CLayout* pLayout = &pTheme->m_arLayouts[pSlide->m_lLayoutID];

	std::vector<NSPresentationEditor::CColor>* pArrayColorScheme = &pTheme->m_arColorScheme;
	if (!pLayout->m_bUseThemeColorScheme)
		pArrayColorScheme = &pLayout->m_arColorScheme;

	////todoooooo разобраться с нмерами
	//int idxTheme = pSlide->m_lThemeID;

	//CSlideInfo* pThemeWrapper	= NULL; 
	//CTheme*     pTheme			= NULL;
	//CLayout*    pLayout			= NULL;

	//if (idxTheme < m_arMasterWrapper.size())
	//	pThemeWrapper = &m_arMasterWrapper[pSlide->m_lThemeID];

	//if (idxTheme < m_arThemes.size())
	//   pTheme = &m_arThemes[idxTheme];

	//int idxLayout = pSlide->m_lLayoutID;

	//if (pTheme)
	//{
	//	if (idxLayout < pTheme->m_arLayouts.size())
	//		pLayout = &pTheme->m_arLayouts[idxLayout];
	//}

	//std::vector<NSPresentationEditor::CColor>* pArrayColorScheme;
	//if (pLayout)
	//{
	//	pArrayColorScheme = &pTheme->m_arColorScheme;
	//	if (!pLayout->m_bUseThemeColorScheme)
	//		pArrayColorScheme = &pLayout->m_arColorScheme;
	//}

	// читаем цветовую схему -----------------------------------------------------------
	pSlide->m_bUseLayoutColorScheme = true;
	if (!oArrayAtoms[0]->m_bMasterScheme)
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
	// ---------------------------------------------------------------------------------

	// читаем все элементы...
	std::vector<CRecordShapeContainer*> oArrayShapes;

	pRecordSlide->GetRecordsByType(&oArrayShapes, true);

	pSlide->m_bShowMasterShapes = oArrayAtoms[0]->m_bMasterObjects;
	pSlide->m_bIsBackground		= false;

	for (int nShape = 0; nShape < oArrayShapes.size(); ++nShape)
	{
		IElement* pElem = NULL;
		oArrayShapes[nShape]->GetElement(&pElem, &m_oExMedia, pSlide->m_lOriginalWidth, pSlide->m_lOriginalHeight,
			pTheme, pLayout, pThemeWrapper, pSlideWrapper, pSlide);

		if (NULL != pElem)
		{
			AddAnimation ( dwSlideID, pSlide->m_lOriginalWidth, pSlide->m_lOriginalHeight, pElem );

			if (pElem->m_bIsBackground)
			{
				if (!oArrayAtoms[0]->m_bMasterBackground)
				{
					CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElem);
					if (NULL != pShape)
					{
						pShape->SetupProperties(pSlide, pTheme, pLayout);

						pSlide->m_bIsBackground = true;
						pSlide->m_oBackground	= pShape->m_oShape.m_oBrush;
					}
				}
				RELEASEINTERFACE(pElem);
				continue;
			}

			pSlide->m_arElements.push_back(pElem);
		}
	}
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

	if (m_mapMasters.end() == pPairMaster)
		return;

	CRecordSlide* pMaster = pPairMaster->second;

    if (pMaster == NULL)
        return; //todooo 20080720.ppt

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
	m_mapMasterToLayout.insert(std::pair<DWORD, LONG>(dwMasterID, 0));

	CTheme theme;
	m_arThemes.push_back(theme);
	CTheme* pTheme = &m_arThemes[lIndexTheme];

	std::vector<CRecordCString*> oArrayStrings;
	pMaster->GetRecordsByType(&oArrayStrings, false, false);
	for (int i=0; i < oArrayStrings.size(); i++)
	{
		if (oArrayStrings[i]->m_oHeader.RecType == 0x0fba)
		{
			pTheme->m_sThemeName = oArrayStrings[i]->m_strText;
		}
	}

	CLayout layout;
	pTheme->m_arLayouts.push_back(layout);
	CLayout* pLayout = &pTheme->m_arLayouts.back();

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
		if (0x01 == oArrayColors[i]->m_oHeader.RecInstance && m_oSchemeColors.size() < 1)
		{
			oArrayColors[i]->ToArray(&pTheme->m_arColorScheme);
			oArrayColors[i]->ToArray(&m_oSchemeColors);
		}
		if (0x06 == oArrayColors[i]->m_oHeader.RecInstance)
		{
			std::vector<CColor> extra;
			oArrayColors[i]->ToArray(&extra);

			CorrectColorScheme(extra);
			pTheme->m_arExtraColorScheme.push_back(extra);
		}

	}

	CorrectColorScheme(pTheme->m_arColorScheme);
	//CorrectColorScheme(m_oSchemeColors);//??

	// ---------------------------------------------------------------------------------

	// вот, грузим placeholder

	std::map<DWORD, CRecordSlide*>::iterator pPairMaster1 = m_mapMasters.find(dwMasterID);
	if (pPairMaster1 != m_mapMasters.end())
	{
		pMasterWrapper->m_arTextPlaceHolders = pPairMaster1->second->m_oPersist.m_arTextAttrs;
	}
	
	pMasterWrapper->m_parEmptyPictures = &m_arEmptyPictures;

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
	pLayout->m_lOriginalWidth	= lOriginWidth;
	pLayout->m_lOriginalHeight	= lOriginHeight;

	pLayout->m_lWidth			= (LONG)(c_dMasterUnitsToMillimetreKoef * lOriginWidth);
	pLayout->m_lHeight			= (LONG)(c_dMasterUnitsToMillimetreKoef * lOriginHeight);

	pLayout->m_bUseThemeColorScheme = true;
	pLayout->m_bShowMasterShapes	= bMasterObjects;
	pLayout->m_strLayoutType		= ConvertLayoutType(oArraySlideAtoms[0]->m_oLayout.m_nGeom, oArraySlideAtoms[0]->m_oLayout.m_pPlaceHolderID);

	// читаем все элементы...-----------------------------------------------------------
	std::vector<CRecordShapeContainer*> oArrayShapes;
	pMaster->GetRecordsByType(&oArrayShapes, true);

	pTheme->CalculateStyles();

	for (int nShape = 0; nShape < oArrayShapes.size(); ++nShape)
	{
		NSPresentationEditor::IElement* pElem = NULL;
		oArrayShapes[nShape]->GetElement(&pElem, &m_oExMedia, lOriginWidth, lOriginHeight, pTheme, pLayout, pMasterWrapper, pMasterWrapper);

		if (NULL != pElem)
		{
			AddAnimation ( dwMasterID, lOriginWidth, lOriginHeight, pElem );

			if (-1 == pElem->m_lPlaceholderType)
			{
				if (pElem->m_bIsBackground)
				{
					CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElem);
					if (NULL != pShape)
					{
						pShape->SetupProperties(NULL, pTheme, pLayout);

						pTheme->m_bIsBackground = true;
						pTheme->m_oBackground = pShape->m_oShape.m_oBrush;
					}

					RELEASEINTERFACE(pElem);
					continue;
				}
				pTheme->m_arElements.push_back(pElem);
			}
			else
			{
				if (pElem->m_bIsBackground)
				{
					if (!bMasterBackGround)
					{
						CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElem);

						if (NULL != pShape)
						{
							pShape->SetupProperties(NULL, pTheme, pLayout);

							pLayout->m_bIsBackground = true;
							pLayout->m_oBackground	= pShape->m_oShape.m_oBrush;
						}
					}
					RELEASEINTERFACE(pElem);
					continue;
				}
				pLayout->m_arElements.push_back(pElem);
			}
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
		// он уже загружен!!!
		return;
	}

	// теперь аналог функции LoadSlide
	std::map<DWORD, LONG>::iterator pPairTheme = m_mapMasterToTheme.find(dwID);
	
	if (m_mapMasterToTheme.end() == pPairTheme)
		return;

	CTheme* pTheme = &m_arThemes[pPairTheme->second];
	LONG lIndexLayout = (LONG)pTheme->m_arLayouts.size();

	m_mapMasterToTheme[dwCurID] =  pPairTheme->second;
	m_mapMasterToLayout[dwCurID] = lIndexLayout;

	CLayout layout_elm;
	pTheme->m_arLayouts.push_back(layout_elm);
	CLayout* pLayout = &pTheme->m_arLayouts[lIndexLayout];

	CSlideInfo elm;
	m_arMasterWrapper.push_back(elm);

	CSlideInfo* pMasterWrapper = &m_arMasterWrapper[m_arMasterWrapper.size() - 1];
	CSlideInfo* pThemeWrapper = &m_arMasterWrapper[pPairTheme->second];

	// вот, грузим placeholder
	pMasterWrapper->m_arTextPlaceHolders = pCurMaster->m_oPersist.m_arTextAttrs;
	
	pMasterWrapper->m_parEmptyPictures = &m_arEmptyPictures;

	pLayout->m_bUseThemeColorScheme = true;
	pLayout->m_bShowMasterShapes	= bMasterObjects;
	pLayout->m_strLayoutType		= ConvertLayoutType(oArraySlideAtoms[0]->m_oLayout.m_nGeom, oArraySlideAtoms[0]->m_oLayout.m_pPlaceHolderID);

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
	// ---------------------------------------------------------------------------------
	pLayout->m_lOriginalWidth	= lOriginWidth;
	pLayout->m_lOriginalHeight	= lOriginHeight;

	pLayout->Clear();

	// ---------------------------------------------------------------------------------

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

			if (pElem->m_bIsBackground)
			{
				if (!bMasterBackGround)
				{
					CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElem);

					if (NULL != pShape)
					{
						pShape->SetupProperties(NULL, pTheme, pLayout);

						pLayout->m_bIsBackground = true;
						pLayout->m_oBackground	= pShape->m_oShape.m_oBrush;
					}
				}
				RELEASEINTERFACE(pElem);
				continue;
			}

			pLayout->m_arElements.push_back(pElem);
		}
	}
}

void CPPTUserInfo::UpdateXMLInfo()
{
	if (!PPT_DUMP_LOG)
		return;

	XmlUtils::CXmlWriter oWriter;

	oWriter.WriteNodeBegin(_T("User"));

	oWriter.WriteString(m_oDocument.ToString());

	// Slides...
	if (0 != m_mapSlides.size())
	{
		oWriter.WriteNodeBegin(_T("Slides"));

		//for (std::map<DWORD, CRecordSlide*>::iterator pPair = m_mapSlides.begin(); pPair != m_mapSlides.end(); ++pPair)
		for (int i=0; i< m_arrSlidesOrder.size(); i++)
		{
			std::map<DWORD, CRecordSlide*>::iterator pPair = m_mapSlides.find(m_arrSlidesOrder[i]);
			
			if (pPair ==  m_mapSlides.end()) 
				continue;

			CRecordSlide* pSlide = pPair->second;
			if (NULL != pSlide)
			{
				oWriter.WriteString(pSlide->ToString(pPair->first));
			}
		}

		oWriter.WriteNodeEnd(_T("Slides"));
	}

	// Masters
	if (0 != m_mapMasters.size())
	{
		oWriter.WriteNodeBegin(_T("Masters"));

		//for (std::map<DWORD, CRecordSlide*>::iterator pPair = m_mapMasters.begin(); pPair != m_mapMasters.end(); ++pPair)
		for (long i=0; i< m_arrMastersOrder.size(); i++)
		{
			std::map<DWORD, CRecordSlide*>::iterator pPair = m_mapMasters.find(m_arrMastersOrder[i]);
			if (pPair == m_mapMasters.end())continue;	

			CRecordSlide* pSlide = pPair->second;
			if (NULL != pSlide)
			{
				oWriter.WriteString(pSlide->ToString(pPair->first));
			}
		}

		oWriter.WriteNodeEnd(_T("Masters"));
	}

	// Notes
	if (0 != m_mapNotes.size())
	{
		oWriter.WriteNodeBegin(_T("Notes"));

		//for (std::map<DWORD, CRecordSlide*>::iterator pPair = m_mapNotes.begin(); pPair != m_mapNotes.end(); ++pPair)
		for (long i=0; i< m_arrNotesOrder.size(); i++)
		{
			std::map<DWORD, CRecordSlide*>::iterator pPair = m_mapNotes.find(m_arrNotesOrder[i]);
			if (pPair == m_mapMasters.end())continue;	
		
			CRecordSlide* pSlide = pPair->second;
			if (NULL != pSlide)
			{
				oWriter.WriteString(pSlide->ToString(pPair->first));
			}
		}

		oWriter.WriteNodeEnd(_T("Notes"));
	}

	oWriter.WriteNodeEnd(_T("User"));

	m_strXmlInfo = oWriter.GetXmlString();

	//CDirectory::SaveToFile(_T("C:\\PPTInfo.xml"), m_strXmlInfo);
}

void CPPTUserInfo::LoadSlideFromPrevUsers(DWORD dwSlideID)
{
	if ((NULL == m_pDocumentInfo) || (-1 == m_lIndexThisUser))
		return;

	std::map<DWORD, CRecordSlide*>::iterator pPairSlide = m_mapSlides.find(dwSlideID);
	
	if (pPairSlide != m_mapSlides.end() && pPairSlide->second)
		return; //есть

	size_t lUsersCount = m_pDocumentInfo->m_arUsers.size();
	// у нас чем раньше - тем больше индекс
	
	for (size_t lIndexUser = m_lIndexThisUser + 1; lIndexUser < lUsersCount; ++lIndexUser)
	//for (size_t lIndexUser = lUsersCount - 1; lIndexUser > m_lIndexThisUser; lIndexUser--)
	{
		std::map<DWORD, CRecordSlide*>::iterator pPair = m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapSlides.find(dwSlideID);
		if (pPair == m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapSlides.end())
			continue;

		CRecordSlide* pSlideCur = pPair->second;
		if (NULL != pSlideCur)
		{
			pSlideCur->AddRef();
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
	
	// у нас чем раньше - тем больше индекс
	
	for (size_t lIndexUser = m_lIndexThisUser + 1; lIndexUser < lUsersCount; ++lIndexUser)
	//for (size_t lIndexUser = lUsersCount - 1; lIndexUser > m_lIndexThisUser; lIndexUser--)
	{
		std::map<DWORD, CRecordSlide*>::iterator pPair = m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapMasters.find(dwMasterID);

		if (pPair == m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapMasters.end())
			continue;
		
		CRecordSlide* pSlideCur = pPair->second;
		if (NULL != pSlideCur)
		{
			pSlideCur->AddRef();

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
	std::vector<CRecordExCDAudioContainer*>		oArrayAudioCD;
	std::vector<CRecordExMIDIAudioContainer*>		oArrayAudioMIDI;
	std::vector<CRecordWAVAudioLinkContainer*>		oArrayWAVLink;
	std::vector<CRecordWAVAudioEmbeddedContainer*> oArrayAudioEmbedded;

	pExObjects->GetRecordsByType(&oArrayAudioCD			, true);
	pExObjects->GetRecordsByType(&oArrayAudioMIDI		, true);
	pExObjects->GetRecordsByType(&oArrayWAVLink			, true);
	pExObjects->GetRecordsByType(&oArrayAudioEmbedded	, true);

	int nSize1 = oArrayAudioCD.size();
	int nSize2 = oArrayAudioMIDI.size();
	int nSize3 = oArrayWAVLink.size();
	int nSize4 = oArrayAudioEmbedded.size();

	for (int nIndex = 0; nIndex < nSize2; ++nIndex)
	{
		LoadExAudio(oArrayAudioMIDI[nIndex]);
	}
	for (int nIndex = 0; nIndex < nSize3; ++nIndex)
	{
		LoadExAudio(oArrayWAVLink[nIndex]);
	}
	for (int nIndex = 0; nIndex < nSize4; ++nIndex)
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
	for (int nIndex = 0; nIndex < nSize1; ++nIndex)
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
	// -----------------------------------------------------------

	oArrayAudioCD.clear();
	oArrayAudioMIDI.clear();
	oArrayWAVLink.clear();
	oArrayAudioEmbedded.clear();
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
		pAudio->m_strFileName	=	strFilePath;				
	}
	// ??? недоделка ???
}
