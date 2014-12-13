#include "stdafx.h"
#include "PPTDocumentInfo.h"

#include "Records\ExMIDIAudioContainer.h"
#include "Records\ExCDAudioContainer.h"
#include "Records\ExWAVAudioLinkContainer.h"
#include "Records\ExWAVAudioEmbeddedContainer.h"

#include "Records\ExObjListContainer.h"
#include "Records\SoundCollectionContainer.h"
#include "Records\SoundContainer.h"

CPPTUserInfo::CPPTUserInfo() : CDocument(),
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
	m_bIsSetupEmpty = FALSE;
}

CPPTUserInfo::~CPPTUserInfo()
{
	Clear();
}

void CPPTUserInfo::Clear()
{
	for (int nIndex = 0; nIndex < m_mapSlides.GetSize(); ++nIndex)
	{
		RELEASEINTERFACE(m_mapSlides.GetValueAt(nIndex));
	}
	m_mapSlides.RemoveAll();

	for (int nIndex = 0; nIndex < m_mapMasters.GetSize(); ++nIndex)
	{
		RELEASEINTERFACE(m_mapMasters.GetValueAt(nIndex));
	}
	m_mapMasters.RemoveAll();

	for (int nIndex = 0; nIndex < m_mapNotes.GetSize(); ++nIndex)
	{
		RELEASEINTERFACE(m_mapNotes.GetValueAt(nIndex));
	}
	m_mapNotes.RemoveAll();

	CDocument::Clear();

	m_oExMedia.Clear();
	m_arrFonts.RemoveAll();

	for (int nIndex = 0; nIndex < m_mapAnimations.GetSize(); ++nIndex)
	{
		RELEASEOBJECT ( m_mapAnimations.GetValueAt(nIndex) );
	}

	m_mapAnimations.RemoveAll();
	m_mapTransitions.RemoveAll ();
	m_arEmptyPictures.RemoveAll();
}

BOOL CPPTUserInfo::ReadFromStream(CRecordUserEditAtom* pUser, IStream* pStream, CStringW strFolderMem)
{
	m_oUser.FromAtom(pUser);

	SRecordHeader oHeader;
	CRecordUserEditAtom oUserAtom;

	StreamUtils::StreamSeek(m_oUser.m_nOffsetPersistDirectory, pStream);

	pStream->Read(&oHeader, sizeof(SRecordHeader), NULL);
	if (RECORD_TYPE_PERSISTPTRINCREMENTALBLOCK != oHeader.RecType)
	{
		return FALSE;
	}

	CRecordPersistDirectoryAtom oPersist;
	oPersist.ReadFromStream(oHeader, pStream);
	oPersist.ToMap(&m_mapOffsetInPIDs);

	if (-1 == m_mapOffsetInPIDs.FindKey(m_oUser.m_nDocumentRef))
	{
		return FALSE;
	}

	StreamUtils::StreamSeek(m_mapOffsetInPIDs.Lookup(m_oUser.m_nDocumentRef), pStream);
	pStream->Read(&oHeader, sizeof(SRecordHeader), NULL);
	if (RECORD_TYPE_DOCUMENT != oHeader.RecType)
	{
		return FALSE;
	}

	m_oDocument.ReadFromStream(oHeader, pStream);

	Clear();

	for (size_t index = 0; index < m_oDocument.m_arMasterPersists.GetCount(); ++index)
	{
		int nIndexPsrRef = m_mapOffsetInPIDs.FindKey(m_oDocument.m_arMasterPersists[index].m_nPsrRef);
		if (-1 != nIndexPsrRef)
		{
			long offset = (long)m_mapOffsetInPIDs.GetValueAt(nIndexPsrRef);
			StreamUtils::StreamSeek(offset, pStream);

			pStream->Read(&oHeader, sizeof(SRecordHeader), NULL);
			CRecordSlide* pSlide = new CRecordSlide();
			pSlide->ReadFromStream(oHeader, pStream);
			pSlide->m_oPersist = m_oDocument.m_arMasterPersists[index];

			m_mapMasters.Add(pSlide->m_oPersist.m_nSlideID, pSlide);
			pSlide = NULL;
		}
		else
		{
			m_mapMasters.Add(m_oDocument.m_arMasterPersists[index].m_nSlideID, NULL);
		}
	}

	for (size_t index = 0; index < m_oDocument.m_arNotePersists.GetCount(); ++index)
	{
		int nIndexPsrRef = m_mapOffsetInPIDs.FindKey(m_oDocument.m_arNotePersists[index].m_nPsrRef);
		if (-1 != nIndexPsrRef)
		{
			long offset = (long)m_mapOffsetInPIDs.GetValueAt(nIndexPsrRef);
			StreamUtils::StreamSeek(offset, pStream);

			pStream->Read(&oHeader, sizeof(SRecordHeader), NULL);
			CRecordSlide* pSlide = new CRecordSlide();
			pSlide->ReadFromStream(oHeader, pStream);
			pSlide->m_oPersist = m_oDocument.m_arNotePersists[index];

			m_mapNotes.Add(pSlide->m_oPersist.m_nSlideID, pSlide);
			pSlide = NULL;
		}
		else
		{
			m_mapNotes.Add(m_oDocument.m_arNotePersists[index].m_nSlideID, NULL);
		}
	}

	for (size_t index = 0; index < m_oDocument.m_arSlidePersists.GetCount(); ++index)
	{
		int nIndexPsrRef = m_mapOffsetInPIDs.FindKey(m_oDocument.m_arSlidePersists[index].m_nPsrRef);
		if (-1 != nIndexPsrRef)
		{
			long offset = (long)m_mapOffsetInPIDs.GetValueAt(nIndexPsrRef);
			StreamUtils::StreamSeek(offset, pStream);

			pStream->Read(&oHeader, sizeof(SRecordHeader), NULL);
			CRecordSlide* pSlide = new CRecordSlide();
			pSlide->ReadFromStream(oHeader, pStream);
			pSlide->m_oPersist = m_oDocument.m_arSlidePersists[index];

			m_mapSlides.Add ( pSlide->m_oPersist.m_nSlideID, pSlide );

			if ( pSlide->m_bExistsTransition )
			{
				m_mapTransitions.Add ( (DWORD)index, pSlide->m_oSlideShowSlideInfoAtom );
			}

			if ( pSlide->m_pSlideProgTagsContainer )
			{
				Animations::CSlideTimeLine* pEffects = pSlide->m_pSlideProgTagsContainer->GetTimeLine ();
				if (pEffects)
				{
					m_mapAnimations.Add((DWORD)index, pEffects);
				}
			}

			pSlide = NULL;
		}
		else
		{
			m_mapSlides.Add(m_oDocument.m_arSlidePersists[index].m_nSlideID, NULL);
		}
	}

	// так... теперь берем всю инфу о ExObject -----------------------------
	m_oExMedia.m_strPresentationDirectory	= strFolderMem;
	m_oExMedia.m_strSourceDirectory			= m_strFileDirectory;

	NSPresentationEditor::CExFilesInfo oInfo;

	oInfo.m_strFilePath = m_oExMedia.m_strPresentationDirectory;
	oInfo.m_dwID		= 0xFFFFFFFF;
	m_oExMedia.m_arImages.push_back(oInfo);

	// читаем все внешние объекты
	CSimpleArray<CRecordExObjListContainer*> oArrayExObjects;
	m_oDocument.GetRecordsByType(&oArrayExObjects, false, true);

	if (0 != oArrayExObjects.GetSize())
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
	CSimpleArray<CRecordFontEntityAtom*> oArrayFonts;
	m_oDocument.GetRecordsByType(&oArrayFonts, true);

	for (int nIndex = 0; nIndex < oArrayFonts.GetSize(); ++nIndex)
	{
		CFont oFont;
		oFont.Name = (CString)oArrayFonts[nIndex]->m_strFaceName;
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

		m_arrFonts.Add(oFont);
	}

	UpdateXMLInfo();
	//FromDocument();
	// FromDocument - должен вызываться после того, как загрузятся все (!!!) юзеры

	// теперь смотрим на пустые картинки
	// теперь заполним пустые картинки
	CSimpleArray<CRecordBlipStoreContainer*> oArray;
	m_oDocument.GetRecordsByType(&oArray, true, true);
	if (0 < oArray.GetSize())
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
	CSimpleArray<CRecordDocumentAtom*> oArrayDoc;
	m_oDocument.GetRecordsByType(&oArrayDoc, true, true);

	if (0 == oArrayDoc.GetSize())
		return;

	CSimpleArray<CRecordDocumentTextInfo*> oArrayInfo;
	m_oDocument.GetRecordsByType(&oArrayInfo, false, true);

	if (0 != oArrayInfo.GetSize())
	{
		CSimpleArray<CRecordTextMasterStyleAtom*> oStyles;
		oArrayInfo[0]->GetRecordsByType(&oStyles, false, false);

		if (0 != oStyles.GetSize())
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

	for ( int nIndex = 0; nIndex < m_mapSlides.GetSize(); ++nIndex )
	{
		DurationSlide	=	PPT_DEFAULT_SLIDE_DURATION;

		CSlide elm;
		m_arSlides.push_back(elm);
		// тут его заполняем...

		// если на слайде есть анимации
		Animations::CSlideTimeLine* pTimeLine		=	m_mapAnimations.Lookup ( (DWORD)nIndex );

		if ( NULL != pTimeLine )
		{
			if ( 0.0 != pTimeLine->GetTime () )
			{
				DurationSlide	=	pTimeLine->GetTime ();
			}
		}

		CSlide* pSlide = &m_arSlides[nIndex];

		pSlide->m_dStartTime		= 0.0;
		pSlide->m_dEndTime			= DurationSlide;
		pSlide->m_dDuration			= DurationSlide;

		pSlide->m_lOriginalWidth	= lOriginWidth;
		pSlide->m_lOriginalHeight	= lOriginHeight;

		pSlide->m_lWidth			= (LONG)(c_dMasterUnitsToMillimetreKoef * lOriginWidth);
		pSlide->m_lHeight			= (LONG)(c_dMasterUnitsToMillimetreKoef * lOriginHeight);

		LoadSlideFromPrevUsers ( nIndex );
		LoadSlide ( nIndex );
	}

	CalculateEditor(m_oInfo);
}


void CPPTUserInfo::LoadSlide(int nIndex)
{
	CRecordSlide* pRecordSlide = m_mapSlides.GetValueAt(nIndex);

	if (NULL == pRecordSlide)
		return;

	// транзишн
	CTransition* pTransition = &m_arSlides[nIndex].m_oSlideShow.m_oTransition;

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

		m_arSlides[nIndex].m_oSlideShow.m_dSlideDuration = pAtom->m_nSlideTime;

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
			AddAudioTransition (nIndex, pTransition, pInfo->m_strFilePath);
	}

	// анимации

	if (NULL == m_mapSlides.GetValueAt(nIndex))
		return;


	size_t lCountSlides = m_arSlides.size();
	CSlide* pSlide = &m_arSlides[lCountSlides - 1];

	pSlide->m_bUseLayoutColorScheme = true;

	m_arSlideWrapper.Add();
	CSlideInfo* pSlideWrapper = &m_arSlideWrapper[lCountSlides - 1];

	pSlideWrapper->m_parEmptyPictures = &m_arEmptyPictures;

	// вот, грузим placeholder
	pSlideWrapper->m_arTextPlaceHolders.Copy(m_mapSlides.GetValueAt(nIndex)->m_oPersist.m_arTextAttrs);

	// записываем шрифты
	CSimpleArray<CRecordSlideAtom*> oArrayAtoms;

	m_mapSlides.GetValueAt(nIndex)->GetRecordsByType(&oArrayAtoms, false, true);
	if (0 == oArrayAtoms.GetSize())
	{
		// ошибка!!!
		return;
	}

	CAtlMap<DWORD, LONG>::CPair* pPair = m_mapMasterToTheme.Lookup(oArrayAtoms[0]->m_nMasterIDRef);
	pSlide->m_lThemeID = pPair->m_value;
	pPair = m_mapMasterToLayout.Lookup(oArrayAtoms[0]->m_nMasterIDRef);
	pSlide->m_lLayoutID = pPair->m_value;

	CSlideInfo* pThemeWrapper = &m_arMasterWrapper[pSlide->m_lThemeID];

	CTheme* pTheme = &m_arThemes[pSlide->m_lThemeID];
	CLayout* pLayout = &pTheme->m_arLayouts[pSlide->m_lLayoutID];

	std::vector<NSPresentationEditor::CColor>* pArrayColorScheme = &pTheme->m_arColorScheme;
	if (!pLayout->m_bUseThemeColorScheme)
		pArrayColorScheme = &pLayout->m_arColorScheme;

	// читаем цветовую схему -----------------------------------------------------------
	pSlide->m_bUseLayoutColorScheme = true;
	if (!oArrayAtoms[0]->m_bMasterScheme)
	{
		CSimpleArray<CRecordColorSchemeAtom*> oArrayColors;
		m_mapSlides.GetValueAt(nIndex)->GetRecordsByType(&oArrayColors, false);

		for (int i = 0; i < oArrayColors.GetSize(); ++i)
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
	CSimpleArray<CRecordShapeContainer*> oArrayShapes;
	m_mapSlides.GetValueAt(nIndex)->GetRecordsByType(&oArrayShapes, true);

	pSlide->m_bShowMasterShapes = (TRUE == oArrayAtoms[0]->m_bMasterObjects);
	pSlide->m_bIsBackground		= false;

	for (int nShape = 0; nShape < oArrayShapes.GetSize(); ++nShape)
	{
		IElement* pElem = NULL;
		oArrayShapes[nShape]->GetElement(&pElem, &m_oExMedia, pSlide->m_lOriginalWidth, pSlide->m_lOriginalHeight,
			pTheme, pLayout, pThemeWrapper, pSlideWrapper, pSlide);

		if (NULL != pElem)
		{
			AddAnimation ( nIndex, pSlide->m_lOriginalWidth, pSlide->m_lOriginalHeight, pElem );

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

void CPPTUserInfo::LoadMainMaster(int nIndexMaster, const LONG& lOriginWidth, const LONG& lOriginHeight)
{
	DWORD dwKey = m_mapMasters.GetKeyAt(nIndexMaster);
	CAtlMap<DWORD, LONG>::CPair* pPair = m_mapMasterToTheme.Lookup(dwKey);
	if (NULL != pPair)
	{
		// мастер уже загружен
		return;
	}

	LoadMasterFromPrevUsers(nIndexMaster);
	CRecordSlide* pMaster = m_mapMasters.Lookup(dwKey);

	if (NULL == pMaster)
		return;

	CSimpleArray<CRecordSlideAtom*> oArraySlideAtoms;

	pMaster->GetRecordsByType(&oArraySlideAtoms, true);
	if (0 == oArraySlideAtoms.GetSize())
		return;

	DWORD dwID = (DWORD)oArraySlideAtoms[0]->m_nMasterIDRef;

	if (0 != dwID)
	{
		// этот мастер - не main!!!
		// сначала загрузим все main, а потом - title
		// title нужно грузить как обычный слайд.
		return;
	}

	CSimpleArray<CRecordTripOriginalMasterId12Atom*> oArrayOrigId;
	pMaster->GetRecordsByType(&oArrayOrigId, false, true);
	if (0 != oArrayOrigId.GetSize())
		m_mapMasterOriginalIds.SetAt(oArrayOrigId[0]->m_dwID, dwKey);

	CSimpleArray<CRecordTripCompositeMasterId12Atom*> oArrayCompId;
	pMaster->GetRecordsByType(&oArrayCompId, false, true);
	if (0 != oArrayCompId.GetSize())
	{
		// этот мастер - не main!!!
		// сначала загрузим все main, а потом - title
		// title нужно грузить как обычный слайд.
		return;
	}

	LONG lIndexTheme = (LONG)m_arThemes.size();
	m_mapMasterToTheme.SetAt(dwKey, lIndexTheme);
	m_mapMasterToLayout.SetAt(dwKey, 0);

	CTheme elm;
	m_arThemes.push_back(elm);
	CTheme* pTheme = &m_arThemes[lIndexTheme];

	CLayout elm1;
	pTheme->m_arLayouts.push_back(elm1);
	CLayout* pLayout = &pTheme->m_arLayouts[0];

	m_arMasterWrapper.Add();
	CSlideInfo* pMasterWrapper = &m_arMasterWrapper[lIndexTheme];

	// записываем шрифты ---------------------------------------------------------------
	int nCountFonts = m_arrFonts.GetSize();
	for (int i = 0; i < nCountFonts; ++i)
	{
		pTheme->m_arFonts.push_back(m_arrFonts[i]);
	}
	// ---------------------------------------------------------------------------------

	// читаем цветовую схему -----------------------------------------------------------
	CSimpleArray<CRecordColorSchemeAtom*> oArrayColors;
	pMaster->GetRecordsByType(&oArrayColors, false);

	int nColorCount = oArrayColors.GetSize();
	for (int i = 0; i < nColorCount; ++i)
	{
		if (0x01 == oArrayColors[i]->m_oHeader.RecInstance)
		{
			oArrayColors[i]->ToArray(&pTheme->m_arColorScheme);
			oArrayColors[i]->ToArray(&m_oSchemeColors);
		}
	}

	CorrectColorScheme(pTheme->m_arColorScheme);

	// ---------------------------------------------------------------------------------

	// вот, грузим placeholder
	pMasterWrapper->m_arTextPlaceHolders.Copy(m_mapMasters.GetValueAt(nIndexMaster)->m_oPersist.m_arTextAttrs);
	pMasterWrapper->m_parEmptyPictures = &m_arEmptyPictures;

	// читаем настройки текстовых стилей -----------------------------------------------
	CSimpleArray<CRecordTextMasterStyleAtom*> oArrayTextMasters;
	pMaster->GetRecordsByType(&oArrayTextMasters, true, false);

	for (int i = 0; i < oArrayTextMasters.GetSize(); ++i)
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
	pLayout->m_bShowMasterShapes	= true;
	pLayout->m_strLayoutType = ConvertLayoutType(oArraySlideAtoms[0]->m_oLayout.m_nGeom, oArraySlideAtoms[0]->m_oLayout.m_pPlaceHolderID);

	// читаем все элементы...-----------------------------------------------------------
	CSimpleArray<CRecordShapeContainer*> oArrayShapes;
	pMaster->GetRecordsByType(&oArrayShapes, true);

	pTheme->CalculateStyles();

	for (int nShape = 0; nShape < oArrayShapes.GetSize(); ++nShape)
	{
		NSPresentationEditor::IElement* pElem = NULL;
		oArrayShapes[nShape]->GetElement(&pElem, &m_oExMedia, lOriginWidth, lOriginHeight, pTheme, pLayout, pMasterWrapper, pMasterWrapper);

		if (NULL != pElem)
		{
			AddAnimation ( nIndexMaster, lOriginWidth, lOriginHeight, pElem );

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

			if (-1 == pElem->m_lPlaceholderType)
				pTheme->m_arElements.push_back(pElem);
			else
				pLayout->m_arElements.push_back(pElem);
		}
	}
}

void CPPTUserInfo::LoadMasters(const LONG& lOriginWidth, const LONG& lOriginHeight)
{
	for (int nIndexMaster = 0; nIndexMaster < m_mapMasters.GetSize(); ++nIndexMaster)
	{
		LoadMainMaster(nIndexMaster, lOriginWidth, lOriginHeight);
	}

	for (int nIndexMaster = 0; nIndexMaster < m_mapMasters.GetSize(); ++nIndexMaster)
	{
		LoadNoMainMaster(nIndexMaster, lOriginWidth, lOriginHeight);
	}
}

void CPPTUserInfo::LoadNoMainMaster(int nIndex, const LONG& lOriginWidth, const LONG& lOriginHeight)
{
	CRecordSlide* pCurMaster = m_mapMasters.GetValueAt(nIndex);

	if (NULL == pCurMaster)
		return;

	DWORD dwCurID = pCurMaster->m_oPersist.m_nSlideID;

	CSimpleArray<CRecordSlideAtom*> oArraySlideAtoms;

	pCurMaster->GetRecordsByType(&oArraySlideAtoms, true);
	if (0 == oArraySlideAtoms.GetSize())
		return;

	BOOL bMasterColorScheme = oArraySlideAtoms[0]->m_bMasterScheme;
	BOOL bMasterBackGround	= oArraySlideAtoms[0]->m_bMasterBackground;
	BOOL bMasterObjects		= oArraySlideAtoms[0]->m_bMasterObjects;

	DWORD dwID = (DWORD)oArraySlideAtoms[0]->m_nMasterIDRef;

	if (0 == dwID)
	{
		CSimpleArray<CRecordTripCompositeMasterId12Atom*> oArrayCompId;
		pCurMaster->GetRecordsByType(&oArrayCompId, false, true);
		if (0 != oArrayCompId.GetSize())
		{
			CAtlMap<DWORD, DWORD>::CPair* pPair = m_mapMasterOriginalIds.Lookup(oArrayCompId[0]->m_dwID);
			if (NULL != pPair)
			{
				dwID = pPair->m_value;
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
	CAtlMap<DWORD, LONG>::CPair* pPair = m_mapMasterToTheme.Lookup(dwID);
	if (NULL == pPair)
		return;

	CTheme* pTheme = &m_arThemes[pPair->m_value];
	LONG lIndexLayout = (LONG)pTheme->m_arLayouts.size();

	m_mapMasterToTheme.SetAt(dwCurID, pPair->m_value);
	m_mapMasterToLayout.SetAt(dwCurID, lIndexLayout);

	CLayout elm;
	pTheme->m_arLayouts.push_back(elm);
	CLayout* pLayout = &pTheme->m_arLayouts[lIndexLayout];

	m_arMasterWrapper.Add();
	CSlideInfo* pMasterWrapper = &m_arMasterWrapper[m_arMasterWrapper.GetCount() - 1];
	CSlideInfo* pThemeWrapper = &m_arMasterWrapper[pPair->m_value];

	// вот, грузим placeholder
	pMasterWrapper->m_arTextPlaceHolders.Copy(pCurMaster->m_oPersist.m_arTextAttrs);
	pMasterWrapper->m_parEmptyPictures = &m_arEmptyPictures;

	pLayout->m_bUseThemeColorScheme = true;
	pLayout->m_bShowMasterShapes	= (bMasterObjects == TRUE) ? true : false;
	pLayout->m_strLayoutType = ConvertLayoutType(oArraySlideAtoms[0]->m_oLayout.m_nGeom, oArraySlideAtoms[0]->m_oLayout.m_pPlaceHolderID);

	std::vector<NSPresentationEditor::CColor>* pArrayColorScheme = &pTheme->m_arColorScheme;
	// читаем цветовую схему -----------------------------------------------------------
	if (!bMasterColorScheme)
	{
		CSimpleArray<CRecordColorSchemeAtom*> oArrayColors;
		pCurMaster->GetRecordsByType(&oArrayColors, false);

		pLayout->m_arColorScheme.clear();
		for (int i = 0; i < oArrayColors.GetSize(); ++i)
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

	//if (!bMasterObjects)
	//	pTheme->m_arElements.RemoveAll();

	// ---------------------------------------------------------------------------------

	// читаем все элементы...
	CSimpleArray<CRecordShapeContainer*> oArrayShapes;
	pCurMaster->GetRecordsByType(&oArrayShapes, true);

	for (int nShape = 0; nShape < oArrayShapes.GetSize(); ++nShape)
	{
		IElement* pElem = NULL;
		oArrayShapes[nShape]->GetElement(&pElem, &m_oExMedia, lOriginWidth, lOriginHeight, pTheme, pLayout, pThemeWrapper, pMasterWrapper);

		if (NULL != pElem)
		{
			AddAnimation ( nIndex, lOriginWidth, lOriginHeight, pElem );

			if (pElem->m_bIsBackground)
			{
				if (!bMasterBackGround)
				{
					CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElem);
					pShape->SetupProperties(NULL, pTheme, pLayout);

					if (NULL != pShape)
					{
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
	if (0 != m_mapSlides.GetSize())
	{
		oWriter.WriteNodeBegin(_T("Slides"));

		for (int nIndex = 0; nIndex < m_mapSlides.GetSize(); ++nIndex)
		{
			CRecordSlide* pSlide = m_mapSlides.GetValueAt(nIndex);
			if (NULL != pSlide)
			{
				oWriter.WriteString(pSlide->ToString(m_mapSlides.GetKeyAt(nIndex)));
			}
		}

		oWriter.WriteNodeEnd(_T("Slides"));
	}

	// Masters
	if (0 != m_mapMasters.GetSize())
	{
		oWriter.WriteNodeBegin(_T("Masters"));

		for (int nIndex = 0; nIndex < m_mapMasters.GetSize(); ++nIndex)
		{
			CRecordSlide* pSlide = m_mapMasters.GetValueAt(nIndex);
			if (NULL != pSlide)
			{
				oWriter.WriteString(pSlide->ToString(m_mapMasters.GetKeyAt(nIndex)));
			}
		}

		oWriter.WriteNodeEnd(_T("Masters"));
	}

	// Notes
	if (0 != m_mapNotes.GetSize())
	{
		oWriter.WriteNodeBegin(_T("Notes"));

		for (int nIndex = 0; nIndex < m_mapNotes.GetSize(); ++nIndex)
		{
			CRecordSlide* pSlide = m_mapNotes.GetValueAt(nIndex);
			if (NULL != pSlide)
			{
				oWriter.WriteString(pSlide->ToString(m_mapNotes.GetKeyAt(nIndex)));
			}
		}

		oWriter.WriteNodeEnd(_T("Notes"));
	}

	oWriter.WriteNodeEnd(_T("User"));

	m_strXmlInfo = oWriter.GetXmlString();

	//CDirectory::SaveToFile(_T("C:\\PPTInfo.xml"), m_strXmlInfo);
}

void CPPTUserInfo::LoadSlideFromPrevUsers(int nSlideIndex)
{
	if ((NULL == m_pDocumentInfo) || (-1 == m_lIndexThisUser))
		return;

	DWORD lSlideID = m_mapSlides.GetKeyAt(nSlideIndex);
	CRecordSlide* pSlideCur = m_mapSlides.GetValueAt(nSlideIndex);

	if (NULL != pSlideCur)
		return;

	size_t lUsersCount = m_pDocumentInfo->m_arUsers.GetCount();
	// у нас чем раньше - тем больше индекс
	for (size_t lIndexUser = m_lIndexThisUser + 1; lIndexUser < lUsersCount; ++lIndexUser)
	{
		pSlideCur = m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapSlides.Lookup(lSlideID);
		if (NULL != pSlideCur)
		{
			pSlideCur->AddRef();
			m_mapSlides.m_aVal[nSlideIndex] = pSlideCur;
			return;
		}
	}
}
void CPPTUserInfo::LoadMasterFromPrevUsers(int nSlideIndex)
{
	if ((NULL == m_pDocumentInfo) || (-1 == m_lIndexThisUser))
		return;

	DWORD lSlideID = m_mapMasters.GetKeyAt(nSlideIndex);
	CRecordSlide* pSlideCur = m_mapMasters.GetValueAt(nSlideIndex);

	if (NULL != pSlideCur)
		return;

	size_t lUsersCount = m_pDocumentInfo->m_arUsers.GetCount();
	// у нас чем раньше - тем больше индекс
	for (size_t lIndexUser = m_lIndexThisUser + 1; lIndexUser < lUsersCount; ++lIndexUser)
	{
		pSlideCur = m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapMasters.Lookup(lSlideID);
		if (NULL != pSlideCur)
		{
			pSlideCur->AddRef();
			m_mapMasters.m_aVal[nSlideIndex] = pSlideCur;
			return;
		}
	}
}
void CPPTUserInfo::LoadNoteFromPrevUsers(int nSlideIndex)
{
	if ((NULL == m_pDocumentInfo) || (-1 == m_lIndexThisUser))
		return;

	DWORD lSlideID = m_mapNotes.GetKeyAt(nSlideIndex);
	CRecordSlide* pSlideCur = m_mapNotes.GetValueAt(nSlideIndex);

	if (NULL != pSlideCur)
		return;

	size_t lUsersCount = m_pDocumentInfo->m_arUsers.GetCount();
	// у нас чем раньше - тем больше индекс
	for (size_t lIndexUser = m_lIndexThisUser + 1; lIndexUser < lUsersCount; ++lIndexUser)
	{
		pSlideCur = m_pDocumentInfo->m_arUsers[lIndexUser]->m_mapNotes.Lookup(lSlideID);
		if (NULL != pSlideCur)
		{
			pSlideCur->AddRef();
			m_mapNotes.m_aVal[nSlideIndex] = pSlideCur;
			return;
		}
	}
}

void CPPTUserInfo::LoadExternal(CRecordExObjListContainer* pExObjects)
{
	// читаем SoundCollection
	CSimpleArray<CRecordSoundCollectionContainer*> oArrayCollection;
	m_oDocument.GetRecordsByType(&oArrayCollection, false, true);

	if (0 != oArrayCollection.GetSize())
	{
		CSimpleArray<CRecordSoundContainer*> oArraySounds;
		oArrayCollection[0]->GetRecordsByType(&oArraySounds, false);

		int nSize = oArraySounds.GetSize();

		for (int i = 0; i < nSize; ++i)
		{
			CSimpleArray<CRecordCString*>		oArrayStrings;
			CSimpleArray<CRecordSoundDataBlob*> oArrayData;

			oArraySounds[i]->GetRecordsByType(&oArrayStrings, false);
			oArraySounds[i]->GetRecordsByType(&oArrayData, false);

			if ((3 <= oArrayStrings.GetSize()) && (1 == oArrayData.GetSize()))
			{
				NSPresentationEditor::CExFilesInfo oInfo;

				oInfo.m_strFilePath = m_oExMedia.m_strPresentationDirectory + oArrayStrings[0]->m_strText + _T(".audio");
				oInfo.m_dwID		= (DWORD)XmlUtils::GetInteger(oArrayStrings[2]->m_strText);

				oArrayData[0]->SaveToFile(oInfo.m_strFilePath);

				m_oExMedia.m_arAudioCollection.push_back(oInfo);
			}
		}
	}

	if (NULL == pExObjects)
		return;

	// читаем видео ----------------------------------------------
	CSimpleArray<CRecordExVideoContainer*> oArray;
	pExObjects->GetRecordsByType(&oArray, true);

	for (int nIndex = 0; nIndex < oArray.GetSize(); ++nIndex)
	{
		LoadExVideo(oArray[nIndex]);
	}

	oArray.RemoveAll();
	// -----------------------------------------------------------

	// читаем аудио ----------------------------------------------
	CSimpleArray<CRecordExCDAudioContainer*>		oArrayAudioCD;
	CSimpleArray<CRecordExMIDIAudioContainer*>		oArrayAudioMIDI;
	CSimpleArray<CRecordWAVAudioLinkContainer*>		oArrayWAVLink;
	CSimpleArray<CRecordWAVAudioEmbeddedContainer*> oArrayAudioEmbedded;

	pExObjects->GetRecordsByType(&oArrayAudioCD			, true);
	pExObjects->GetRecordsByType(&oArrayAudioMIDI		, true);
	pExObjects->GetRecordsByType(&oArrayWAVLink			, true);
	pExObjects->GetRecordsByType(&oArrayAudioEmbedded	, true);

	int nSize1 = oArrayAudioCD.GetSize();
	int nSize2 = oArrayAudioMIDI.GetSize();
	int nSize3 = oArrayWAVLink.GetSize();
	int nSize4 = oArrayAudioEmbedded.GetSize();

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

	oArrayAudioCD.RemoveAll();
	oArrayAudioMIDI.RemoveAll();
	oArrayWAVLink.RemoveAll();
	oArrayAudioEmbedded.RemoveAll();
}

void CPPTUserInfo::LoadExVideo(CRecordsContainer* pExObject)
{
	CSimpleArray<CRecordExMediaAtom*> oArrayExMedia;
	CSimpleArray<CRecordCString*> oArrayCString;

	pExObject->GetRecordsByType(&oArrayExMedia, false);
	pExObject->GetRecordsByType(&oArrayCString, false);

	if ((1 == oArrayExMedia.GetSize()) && (1 == oArrayCString.GetSize()))
	{
		NSPresentationEditor::CExFilesInfo oInfo;

		oInfo.m_dwID			= oArrayExMedia[0]->m_nExObjID;
		oInfo.m_strFilePath		= oArrayCString[0]->m_strText;

		oInfo.m_bLoop			= oArrayExMedia[0]->m_bLoop;

		m_oExMedia.m_arVideos.push_back(oInfo);
	}

	oArrayExMedia.RemoveAll();
	oArrayCString.RemoveAll();
}
void CPPTUserInfo::LoadExAudio(CRecordsContainer* pExObject)
{
	CSimpleArray<CRecordExMediaAtom*> oArrayExMedia;
	CSimpleArray<CRecordCString*> oArrayCString;

	pExObject->GetRecordsByType(&oArrayExMedia, false);
	pExObject->GetRecordsByType(&oArrayCString, false);

	if ((1 == oArrayExMedia.GetSize()) && (1 == oArrayCString.GetSize()))
	{
		NSPresentationEditor::CExFilesInfo oInfo;

		oInfo.m_dwID			= oArrayExMedia[0]->m_nExObjID;
		oInfo.m_strFilePath		= oArrayCString[0]->m_strText;

		oInfo.m_bLoop			= oArrayExMedia[0]->m_bLoop;

		m_oExMedia.m_arAudios.push_back(oInfo);
	}

	oArrayExMedia.RemoveAll();
	oArrayCString.RemoveAll();
}

void CPPTUserInfo::AddAnimation ( int SlideIndex, double Width, double Height, IElement* pElement )
{
	Animations::CSlideTimeLine* pTimeLine	=	m_mapAnimations.Lookup ( SlideIndex );
	if (pTimeLine)
	{
		Animations::Effects* arEffects		=	pTimeLine->GetAnimation().Lookup ( pElement->m_lID );
		if (arEffects)
		{
			for ( long i = 0; i < (long)arEffects->GetCount(); ++i )
			{								
				CAnimationSimple oEffect	=	arEffects->GetAt(i);
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

				pElement->m_oAnimations.m_arAnimations.push_back( oAnimation );
			}
		}
	}
}

void CPPTUserInfo::AddAudioTransition (long nSlide, CTransition* pTransition, const CString& strFilePath)
{
	if (NULL==pTransition) 
		return;

	CAudioElement* pAudio		=	new CAudioElement ();
	if (pAudio)
	{
		pAudio->m_strFileName	=	strFilePath;				
	}
}