#include "stdafx.h"
#include "PPTDocumentInfo.h"

#include "Records\ExMIDIAudioContainer.h"
#include "Records\ExCDAudioContainer.h"
#include "Records\ExWAVAudioLinkContainer.h"
#include "Records\ExWAVAudioEmbeddedContainer.h"

#include "Records\ExObjListContainer.h"
#include "Records\SoundCollectionContainer.h"
#include "Records\SoundContainer.h"

CPPTUserInfo::CPPTUserInfo() : 
	m_oUser(),
	m_mapOffsetInPIDs(), 
	m_oDocument(),
	m_arNormalSlides(),
	m_mapMasters(),
	m_mapNotes(),
	m_mapSlides(),
	m_mapExFiles(),
	m_mapTextStyles(),
	m_arEmptyPictures()
{
	m_strXmlVideoSource = _T("");
	m_strXmlTransforms = _T("");
	m_strXmlOverlays = _T("");

	m_strXmlInfo = _T("");

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

	// вот это - обязательно ДО удаления мастеров
	m_arNormalSlides.RemoveAll();
	
	CSlide* pNormalSlide = NULL;
	int nCountPIDs = m_arNormalMasters.GetSize();
	while(0 != nCountPIDs) 
	{ 
		pNormalSlide = m_arNormalMasters.GetValueAt(0);
		if (NULL != pNormalSlide)
			delete pNormalSlide; 
		
		m_arNormalMasters.RemoveAt(0);
		pNormalSlide = NULL;
		--nCountPIDs;
	}

	int nCountEx = m_mapExFiles.GetSize();
	for (int nIndex = 0; nIndex < nCountEx; ++nIndex)
	{
		CExFilesInfo* pInfo = m_mapExFiles.GetValueAt(nIndex);
		RELEASEOBJECT(pInfo);
	}

	m_mapExFiles.RemoveAll();
	m_arrFonts.RemoveAll();
	m_mapSoundCollection.RemoveAll();
	
	for (int nIndex = 0; nIndex < m_mapAnimations.GetSize(); ++nIndex)
	{
		RELEASEOBJECT ( m_mapAnimations.GetValueAt(nIndex) );
	}

	m_mapAnimations.RemoveAll();

	m_mapTransitions.RemoveAll ();

	m_mapTextStyles.RemoveAll();

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
				Animations::CSlideTimeLine* pTimeLine = pSlide->m_pSlideProgTagsContainer->CreateTimeLine ();
				if ( NULL != pTimeLine )
				{
					m_mapAnimations.Add ( (DWORD)index, pTimeLine );
				}
			}

			pSlide = NULL;
		}
		else
		{
			m_mapSlides.Add(m_oDocument.m_arSlidePersists[index].m_nSlideID, NULL);
		}
	}

	// здесь читаем стили текста
	CSimpleArray<CRecordTextMasterStyleAtom*> oArrayTextStyles;
	m_oDocument.GetRecordsByType(&oArrayTextStyles, true, false);

	m_mapTextStyles.RemoveAll();
	for (int nIndex = 0; nIndex < oArrayTextStyles.GetSize(); ++nIndex)
	{
		NSPPT::AddToSimpleMap(&m_mapTextStyles, (DWORD)oArrayTextStyles[nIndex]->m_oHeader.RecInstance,
			(CTextMasterStyle*)oArrayTextStyles[nIndex]);
	}

	// так... теперь берем всю инфу о ExObject -----------------------------
	CExFilesInfo* pInfoImages = new CExFilesInfo();
	pInfoImages->m_strFilePath	= strFolderMem;

	m_mapExFiles.Add(0xFFFFFFFF, pInfoImages);
	
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
		CFont_ oFont;
		oFont.m_strFontName = (CString)oArrayFonts[nIndex]->m_strFaceName;
		oFont.m_lCharset	= oArrayFonts[nIndex]->m_lfCharSet;

		switch (oArrayFonts[nIndex]->m_lfPitchAndFamily / 0x10)
        {
		case 1: {oFont.m_strPitchFamily = _T("roman"); break;}
		case 2: {oFont.m_strPitchFamily = _T("swiss"); break;}
		case 3: {oFont.m_strPitchFamily = _T("modern"); break;}
		case 4: {oFont.m_strPitchFamily = _T("script"); break;}
		case 5: {oFont.m_strPitchFamily = _T("decorative"); break;}
		default: {oFont.m_strPitchFamily = _T("unknown"); break;}
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
	//UpdateXMLInfo();
	m_arNormalSlides.RemoveAll();

	// здесь из документа читаем слайды в
	// нормальном формате, и сразу их правильно расставляем...
	CSimpleArray<CRecordDocumentAtom*> oArrayDoc;
	m_oDocument.GetRecordsByType(&oArrayDoc, true, true);

	if (0 == oArrayDoc.GetSize())
	{
		return;
	}

	LONG lOriginWidth		=	oArrayDoc[0]->m_oSlideSize.X;
	LONG lOriginHeight		=	oArrayDoc[0]->m_oSlideSize.Y;

	LoadMasters ( lOriginWidth, lOriginHeight );

	double DurationSlide	=	PPT_DEFAULT_SLIDE_DURATION;

	for ( int nIndex = 0; nIndex < m_mapSlides.GetSize(); ++nIndex )
	{
		DurationSlide	=	PPT_DEFAULT_SLIDE_DURATION;

		m_arNormalSlides.Add();
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

		m_arNormalSlides[nIndex].m_dStartTime		=	0.0;
		m_arNormalSlides[nIndex].m_dEndTime			=	DurationSlide;
		m_arNormalSlides[nIndex].m_dDuration		=	DurationSlide;

		m_arNormalSlides[nIndex].m_lOriginalWidth	=	lOriginWidth;
		m_arNormalSlides[nIndex].m_lOriginalHeight	=	lOriginHeight;

		m_arNormalSlides[nIndex].m_lWidth			=	(LONG)(c_dMasterUnitsToMillimetreKoef * lOriginWidth);
		m_arNormalSlides[nIndex].m_lHeight			=	(LONG)(c_dMasterUnitsToMillimetreKoef * lOriginHeight);

		m_arNormalSlides[nIndex].m_eSlideType = CSlide::stSlide;

		LoadSlideFromPrevUsers ( nIndex );
		LoadSlide ( nIndex );
	}

	UpdateXMLSource();

	// вот по-идее больше ничего не нужно...
	// но пока удалять не будем. потом, на close все удалится...
	// Clear();
}


void CPPTUserInfo::LoadSlide(int nIndex)
{
	if (NULL == m_mapSlides.GetValueAt(nIndex))
		return;
	
	m_arNormalSlides[nIndex].m_parEmptyPictures = &m_arEmptyPictures;
	m_arNormalSlides[nIndex].m_arElements.RemoveAll();

	// вот, грузим placeholder
	m_arNormalSlides[nIndex].m_arTextPlaceHolders.RemoveAll();
	m_arNormalSlides[nIndex].m_arTextPlaceHolders.Append(m_mapSlides.GetValueAt(nIndex)->m_oPersist.m_arTextAttrs);

	// записываем шрифты
	m_arNormalSlides[nIndex].m_pFonts = &m_arrFonts;
	
	CSimpleArray<CRecordSlideAtom*> oArrayAtoms;
	
	m_mapSlides.GetValueAt(nIndex)->GetRecordsByType(&oArrayAtoms, false, true);
	if (0 == oArrayAtoms.GetSize())
	{
		// ошибка!!!
		return;
	}

	CSlide* pMaster = m_arNormalMasters.Lookup(oArrayAtoms[0]->m_nMasterIDRef);

	// теперь читаем все мастерские настройки текста...
	if (NULL != pMaster)
	{
		NSPPT::AppendSimpleMap(&m_arNormalSlides[nIndex].m_mapTextStyles, &pMaster->m_mapTextStyles);
	}
	else
	{
		NSPPT::AppendSimpleMap(&m_arNormalSlides[nIndex].m_mapTextStyles, &m_mapTextStyles);
	}

	/*CSimpleArray<CRecordColorSchemeAtom*> oArrayColors;
	m_mapSlides.GetValueAt(nIndex)->GetRecordsByType(&oArrayColors, false);*/
	
	// читаем цветовую схему -----------------------------------------------------------
	if (oArrayAtoms[0]->m_bMasterScheme && (NULL != pMaster))
	{
		m_arNormalSlides[nIndex].m_arColorScheme.RemoveAll();
		for (int i = 0; i < pMaster->m_arColorScheme.GetSize(); ++i)
		{
			m_arNormalSlides[nIndex].m_arColorScheme.Add(pMaster->m_arColorScheme[i]);
		}
	}
	else
	{
		CSimpleArray<CRecordColorSchemeAtom*> oArrayColors;
		m_mapSlides.GetValueAt(nIndex)->GetRecordsByType(&oArrayColors, false);

		m_arNormalSlides[nIndex].m_arColorScheme.RemoveAll();
		for (int i = 0; i < oArrayColors.GetSize(); ++i)
		{
			if (0x01 == oArrayColors[i]->m_oHeader.RecInstance)
			{
				oArrayColors[i]->ToArray(&m_arNormalSlides[nIndex].m_arColorScheme);
			}
		}
	}
	// ---------------------------------------------------------------------------------

	// рисуем все, что ТОЧНО нужно отрисовать от мастера -------------------------------
	// вот тут по-ходу нужно учитывать мастер - бэкграунд, или нет,
	// мы же пока на это забиваем

	int nCountMasterShapes = 0;

	if (NULL != pMaster && oArrayAtoms[0]->m_bMasterObjects)
	{
		for (int nIndexMasterElem = 0; nIndexMasterElem < pMaster->m_arElements.GetSize(); ++nIndexMasterElem)
		{
			if ((-1 == pMaster->m_arElements[nIndexMasterElem]->m_lPlaceholderPosition) || (pMaster->m_arElements[nIndexMasterElem]->m_bIsBackground))
			{
				IElement* pElem = pMaster->m_arElements[nIndexMasterElem]->CreateDublicate();
				if (NULL == pElem)
					continue;

				m_arNormalSlides[nIndex].m_arElements.Add(pElem);

				m_arNormalSlides[nIndex].m_arElements[nCountMasterShapes]->m_dStartTime = 
					m_arNormalSlides[nIndex].m_dStartTime;
				m_arNormalSlides[nIndex].m_arElements[nCountMasterShapes]->m_dEndTime = 
					m_arNormalSlides[nIndex].m_dEndTime;

				++nCountMasterShapes;
			}
		}
	}

	// ---------------------------------------------------------------------------------

	// читаем все элементы...
	CSimpleArray<CRecordShapeContainer*> oArrayShapes;
	m_mapSlides.GetValueAt(nIndex)->GetRecordsByType(&oArrayShapes, true);

	// читаем длительность слайда, если она была указана. По умолчанию - (m_dEndTime - m_dStartTime)
	m_arNormalSlides[nIndex].m_dReadDuration = m_mapSlides.GetValueAt(nIndex)->m_oSlideShowSlideInfoAtom.GetTimeSlide();

	int nMin = 0;

	for (int nShape = 0; nShape < oArrayShapes.GetSize(); ++nShape)
	{
		IElement* pElem = NULL;
		oArrayShapes[nShape]->GetElement(&pElem, &m_mapExFiles, 
			m_arNormalSlides[nIndex].m_lOriginalWidth, m_arNormalSlides[nIndex].m_lOriginalHeight,
			&m_arNormalSlides[nIndex], pMaster);

		if (NULL != pElem)
		{
			m_arNormalSlides[nIndex].m_arElements.Add(pElem);
			/*if (FALSE == pElem->m_bIsBackground)
			{
				m_arNormalSlides[nIndex].m_arElements.Add(pElem);
			}
			else if ((false == oArrayAtoms[0]->m_bMasterBackground) || (NULL == pMaster))
			{
				m_arNormalSlides[nIndex].m_arElements.Add(pElem);
				nMin = 1;
			}*/
			if (pElem->m_bIsBackground)
				nMin = 1;
		}
	}

	// теперь - нужно последний - на первое место...
	int nCount_master_elements = m_arNormalSlides[nIndex].m_arElements.GetSize();
	if (nCount_master_elements > 1)
	{
		for (int i = nCount_master_elements - 1; i > nMin; --i)
		{
			// background поменян в мастере
			if (0 != nMin)
			{
				IElement* pTemp = m_arNormalSlides[nIndex].m_arElements[i];
				m_arNormalSlides[nIndex].m_arElements[i] = m_arNormalSlides[nIndex].m_arElements[i - 1];
				m_arNormalSlides[nIndex].m_arElements[i - 1] = pTemp;
			}

			// а заодно и прочитаем (если они есть)
			// и тектовые настройки (у последнего (теперь он будет первым) - их нет)
			//m_arNormalSlides[nIndex].m_arElements[i]->LoadTextStyle(&m_arNormalSlides[nIndex]);
			m_arNormalSlides[nIndex].SetUpTextStyle(m_arNormalSlides[nIndex].m_arElements[i]);
		}
	}
	
	// а вот это уже лишнее, эта коррекция уже есть в GetElement
	//m_arNormalSlides[nIndex].NormalizeCoordinates();
}

void CPPTUserInfo::LoadMainMaster(int nIndexMaster, const LONG& lOriginWidth, const LONG& lOriginHeight)
{
	DWORD dwKey = m_mapMasters.GetKeyAt(nIndexMaster);
	CSlide* pMasterSlide = m_arNormalMasters.Lookup(dwKey);
	if (NULL != pMasterSlide)
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

	CSlide* pNormalMaster = new CSlide();
	pNormalMaster->m_eSlideType = CSlide::stMaster;

	pNormalMaster->m_parEmptyPictures = &m_arEmptyPictures;
	
	// записываем шрифты
	pNormalMaster->m_pFonts = &m_arrFonts;

	// вот, грузим placeholder
	pNormalMaster->m_arTextPlaceHolders.RemoveAll();
	pNormalMaster->m_arTextPlaceHolders.Append(m_mapMasters.GetValueAt(nIndexMaster)->m_oPersist.m_arTextAttrs);
				
	// читаем цветовую схему -----------------------------------------------------------
	CSimpleArray<CRecordColorSchemeAtom*> oArrayColors;
	pMaster->GetRecordsByType(&oArrayColors, false);

	pNormalMaster->m_arColorScheme.RemoveAll();
	for (int i = 0; i < oArrayColors.GetSize(); ++i)
	{
		if (0x01 == oArrayColors[i]->m_oHeader.RecInstance)
		{
			oArrayColors[i]->ToArray(&pNormalMaster->m_arColorScheme);
		}
	}
	// ---------------------------------------------------------------------------------

	// читаем настройки текстовых стилей -----------------------------------------------
	CSimpleArray<CRecordTextMasterStyleAtom*> oArrayTextMasters;
	pMaster->GetRecordsByType(&oArrayTextMasters, true, false);
	
	pNormalMaster->m_mapTextStyles.RemoveAll();
	//NSPPT::AppendSimpleMap(&pNormalMaster->m_mapTextStyles, &m_mapTextStyles);

	for (int i = 0; i < oArrayTextMasters.GetSize(); ++i)
	{
		NSPPT::AddToSimpleMap(&(pNormalMaster->m_mapTextStyles), (DWORD)oArrayTextMasters[i]->m_oHeader.RecInstance,
												(CTextMasterStyle*)oArrayTextMasters[i]);
	}

	NSPPT::AppendSimpleMap(&pNormalMaster->m_mapTextStyles, &m_mapTextStyles);
	// ---------------------------------------------------------------------------------

	// ---------------------------------------------------------------------------------
	// для мастера - время не важно
	pNormalMaster->m_dStartTime = 0;
	pNormalMaster->m_dEndTime = 0;
	pNormalMaster->m_dDuration = 0;

	pNormalMaster->m_lOriginalWidth = lOriginWidth;
	pNormalMaster->m_lOriginalHeight = lOriginHeight;

	pNormalMaster->m_lWidth = (LONG)(c_dMasterUnitsToMillimetreKoef * lOriginWidth);
	pNormalMaster->m_lHeight = (LONG)(c_dMasterUnitsToMillimetreKoef * lOriginHeight);

	// читаем все элементы...-----------------------------------------------------------
	CSimpleArray<CRecordShapeContainer*> oArrayShapes;
	pMaster->GetRecordsByType(&oArrayShapes, true);

	for (int nShape = 0; nShape < oArrayShapes.GetSize(); ++nShape)
	{
		IElement* pElem = NULL;
		oArrayShapes[nShape]->GetElement(&pElem, &m_mapExFiles, 
			pNormalMaster->m_lOriginalWidth, pNormalMaster->m_lOriginalHeight, pNormalMaster);

		if (NULL != pElem)
		{
			pNormalMaster->m_arElements.Add(pElem);
		}
	}

	// теперь - нужно последний - на первое место...
	int nCount_master_elements = pNormalMaster->m_arElements.GetSize();
	if (nCount_master_elements > 1)
	{
		for (int i = nCount_master_elements - 1; i > 0; --i)
		{
			IElement* pTemp = pNormalMaster->m_arElements[i];
			pNormalMaster->m_arElements[i] = pNormalMaster->m_arElements[i - 1];
			pNormalMaster->m_arElements[i - 1] = pTemp;

			// а заодно и прочитаем (если они есть)
			// и тектовые настройки
			//pNormalMaster->m_arElements[i - 1]->LoadTextStyle(pNormalMaster);
			pNormalMaster->SetUpTextStyle(pNormalMaster->m_arElements[i]);
		}
	}

	m_arNormalMasters.Add(dwKey, pNormalMaster);
}

void CPPTUserInfo::LoadMasters(const LONG& lOriginWidth, const LONG& lOriginHeight)
{
	for (int nIndexMaster = 0; nIndexMaster < m_mapMasters.GetSize(); ++nIndexMaster)
	{
		LoadMainMaster(nIndexMaster, lOriginWidth, lOriginHeight);
		//LoadMasterFromPrevUsers(nIndexMaster);

		//DWORD dwKey = m_mapMasters.GetKeyAt(nIndexMaster);
		//CRecordSlide* pMaster = m_mapMasters.Lookup(dwKey);

		//if (NULL == pMaster)
		//	continue;

		//CSlide* pNormalMaster = new CSlide();
		//pNormalMaster->m_eSlideType = CSlide::stMaster;
		//
		//// записываем шрифты
		//pNormalMaster->m_pFonts = &m_arrFonts;

		//// вот, грузим placeholder
		//pNormalMaster->m_arTextPlaceHolders.RemoveAll();
		//pNormalMaster->m_arTextPlaceHolders.Append(m_mapMasters.GetValueAt(nIndexMaster)->m_oPersist.m_arTextAttrs);
		//			
		//// читаем цветовую схему -----------------------------------------------------------
		//CSimpleArray<CRecordColorSchemeAtom*> oArrayColors;
		//pMaster->GetRecordsByType(&oArrayColors, false);

		//pNormalMaster->m_arColorScheme.RemoveAll();
		//for (int i = 0; i < oArrayColors.GetSize(); ++i)
		//{
		//	if (0x01 == oArrayColors[i]->m_oHeader.RecInstance)
		//	{
		//		oArrayColors[i]->ToArray(&pNormalMaster->m_arColorScheme);
		//	}
		//}
		//// ---------------------------------------------------------------------------------

		//// читаем настройки текстовых стилей -----------------------------------------------
		//CSimpleArray<CRecordTextMasterStyleAtom*> oArrayTextMasters;
		//pMaster->GetRecordsByType(&oArrayTextMasters, true, false);
		//
		//pNormalMaster->m_mapTextStyles.RemoveAll();
		////NSPPT::AppendSimpleMap(&pNormalMaster->m_mapTextStyles, &m_mapTextStyles);

		//for (int i = 0; i < oArrayTextMasters.GetSize(); ++i)
		//{
		//	NSPPT::AddToSimpleMap(&(pNormalMaster->m_mapTextStyles), (DWORD)oArrayTextMasters[i]->m_oHeader.RecInstance,
		//											(CTextMasterStyle*)oArrayTextMasters[i]);
		//}

		//NSPPT::AppendSimpleMap(&pNormalMaster->m_mapTextStyles, &m_mapTextStyles);
		//// ---------------------------------------------------------------------------------

		//// ---------------------------------------------------------------------------------
		//// для мастера - время не важно
		//pNormalMaster->m_dStartTime = 0;
		//pNormalMaster->m_dEndTime = 0;
		//pNormalMaster->m_dDuration = 0;

		//pNormalMaster->m_lOriginalWidth = lOriginWidth;
		//pNormalMaster->m_lOriginalHeight = lOriginHeight;

		//pNormalMaster->m_lWidth = (LONG)(c_dMasterUnitsToMillimetreKoef * lOriginWidth);
		//pNormalMaster->m_lHeight = (LONG)(c_dMasterUnitsToMillimetreKoef * lOriginHeight);

		//// читаем все элементы...-----------------------------------------------------------
		//CSimpleArray<CRecordShapeContainer*> oArrayShapes;
		//pMaster->GetRecordsByType(&oArrayShapes, true);

		//for (int nShape = 0; nShape < oArrayShapes.GetSize(); ++nShape)
		//{
		//	IElement* pElem = NULL;
		//	oArrayShapes[nShape]->GetElement(&pElem, &m_mapExFiles, 
		//		pNormalMaster->m_lOriginalWidth, pNormalMaster->m_lOriginalHeight, pNormalMaster);

		//	if (NULL != pElem)
		//	{
		//		pNormalMaster->m_arElements.Add(pElem);
		//	}
		//}

		//// теперь - нужно последний - на первое место...
		//int nCount_master_elements = pNormalMaster->m_arElements.GetSize();
		//if (nCount_master_elements > 1)
		//{
		//	for (int i = nCount_master_elements - 1; i > 0; --i)
		//	{
		//		IElement* pTemp = pNormalMaster->m_arElements[i];
		//		pNormalMaster->m_arElements[i] = pNormalMaster->m_arElements[i - 1];
		//		pNormalMaster->m_arElements[i - 1] = pTemp;

		//		// а заодно и прочитаем (если они есть)
		//		// и тектовые настройки
		//		//pNormalMaster->m_arElements[i - 1]->LoadTextStyle(pNormalMaster);
		//		pNormalMaster->SetUpTextStyle(pNormalMaster->m_arElements[i]);
		//	}
		//}
	
		//m_arNormalMasters.Add(dwKey, pNormalMaster);
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

	// проверяем, если это MainMaster - то его грузим как мастер
	if (0 == dwID)
	{
		// он уже загружен!!!
		return;
	}

	// теперь аналог функции LoadSlide

	CSlide* pMaster = m_arNormalMasters.Lookup(dwID);
	if (NULL == pMaster)
		return;

	CSlide* pSlide = new CSlide();
	pSlide->m_eSlideType = CSlide::stMaster;
	pSlide->m_parEmptyPictures = &m_arEmptyPictures;
	
	pSlide->m_arElements.RemoveAll();
	
	// вот, грузим placeholder
	pSlide->m_arTextPlaceHolders.RemoveAll();
	pSlide->m_arTextPlaceHolders.Append(pCurMaster->m_oPersist.m_arTextAttrs);

	// записываем шрифты
	pSlide->m_pFonts = &m_arrFonts;
	
	CSimpleArray<CRecordSlideAtom*> oArrayAtoms;
	
	// теперь читаем все мастерские настройки текста...
	NSPPT::AppendSimpleMap(&pSlide->m_mapTextStyles, &pMaster->m_mapTextStyles);
	
	// читаем цветовую схему -----------------------------------------------------------
	if (bMasterColorScheme)
	{
		pSlide->m_arColorScheme.RemoveAll();
		for (int i = 0; i < pMaster->m_arColorScheme.GetSize(); ++i)
		{
			pSlide->m_arColorScheme.Add(pMaster->m_arColorScheme[i]);
		}
	}
	else
	{
		CSimpleArray<CRecordColorSchemeAtom*> oArrayColors;
		pCurMaster->GetRecordsByType(&oArrayColors, false);

		pSlide->m_arColorScheme.RemoveAll();
		for (int i = 0; i < oArrayColors.GetSize(); ++i)
		{
			if (0x01 == oArrayColors[i]->m_oHeader.RecInstance)
			{
				oArrayColors[i]->ToArray(&pSlide->m_arColorScheme);
			}
		}
	}
	// ---------------------------------------------------------------------------------

	pSlide->m_dStartTime = 0;
	pSlide->m_dEndTime = 0;
	pSlide->m_dDuration = 0;

	pSlide->m_lOriginalWidth = lOriginWidth;
	pSlide->m_lOriginalHeight = lOriginHeight;

	pSlide->m_lWidth = (LONG)(c_dMasterUnitsToMillimetreKoef * lOriginWidth);
	pSlide->m_lHeight = (LONG)(c_dMasterUnitsToMillimetreKoef * lOriginHeight);

	// рисуем все, что ТОЧНО нужно отрисовать от мастера -------------------------------
	// вот тут по-ходу нужно учитывать мастер - бэкграунд, или нет,
	// мы же пока на это забиваем

	int nCountMasterShapes = 0;

	if (NULL != pMaster && bMasterObjects)
	{
		for (int nIndexMasterElem = 0; nIndexMasterElem < pMaster->m_arElements.GetSize(); ++nIndexMasterElem)
		{
			if (-1 == pMaster->m_arElements[nIndexMasterElem]->m_lPlaceholderPosition)
			{
				pSlide->m_arElements.Add(pMaster->m_arElements[nIndexMasterElem]->CreateDublicate());

				pSlide->m_arElements[nCountMasterShapes]->m_dStartTime	= pMaster->m_dStartTime;
				pSlide->m_arElements[nCountMasterShapes]->m_dEndTime	= pMaster->m_dEndTime;

				++nCountMasterShapes;
			}
		}
	}

	// ---------------------------------------------------------------------------------

	// читаем все элементы...
	CSimpleArray<CRecordShapeContainer*> oArrayShapes;
	pCurMaster->GetRecordsByType(&oArrayShapes, true);

	int nMin = 0;

	for (int nShape = 0; nShape < oArrayShapes.GetSize(); ++nShape)
	{
		IElement* pElem = NULL;
		oArrayShapes[nShape]->GetElement(&pElem, &m_mapExFiles, pMaster->m_lOriginalWidth, pMaster->m_lOriginalHeight, pSlide, pMaster);

		if (NULL != pElem)
		{
			/*if (FALSE == pElem->m_bIsBackground)
			{
				pSlide->m_arElements.Add(pElem);
			}
			else if (false == bMasterBackGround && !bMasterObjects)
			{
				pSlide->m_arElements.Add(pElem);
				nMin = 1;
			}*/
			pSlide->m_arElements.Add(pElem);
		}
	}

	if (0 < nCountMasterShapes)
		++nMin;

	// теперь - нужно последний - на первое место...
	int nCount_master_elements = pSlide->m_arElements.GetSize();
	if (nCount_master_elements > 1)
	{
		for (int i = nCount_master_elements - 1; i > nMin; --i)
		{
			// background поменян в мастере
			if (0 != nMin || !bMasterObjects)
			{
				IElement* pTemp = pSlide->m_arElements[i];
				pSlide->m_arElements[i] = pSlide->m_arElements[i - 1];
				pSlide->m_arElements[i - 1] = pTemp;
			}

			// а заодно и прочитаем (если они есть)
			// и тектовые настройки (у последнего (теперь он будет первым) - их нет)
			pSlide->SetUpTextStyle(pSlide->m_arElements[i]);
		}
	}
	
	m_arNormalMasters.Add(dwCurID, pSlide);
}

void CPPTUserInfo::UpdateXMLSource()
{
	m_strXmlVideoSource	=	_T("");
	m_strXmlTransforms	=	_T("");
	m_strXmlOverlays	=	_T("");
	CString strVideoOverlay = _T("");
	CString strAudioOverlay = _T("");

	CAudioOverlay oAudioOverlay;
	CalculateTimes(oAudioOverlay);

	if ( m_arNormalSlides.GetCount () > 0 )
	{
		//CString strXmlVideoSource = _T("");
		
		double dDuration = 0.0;
		for (size_t nIndex = 0; nIndex < m_arNormalSlides.GetCount(); ++nIndex)
		{
			dDuration += m_arNormalSlides[nIndex].m_dDuration;
		}

		// strXmlVideoSource.Format(_T("<VideoSources><ColorSource Color='16777215' Duration='%lf'/></VideoSources>"),	dDuration);

		m_nWriteSlideTimeOffset	=	0.0;
		m_nWriteSlideTime		=	0.0;

		CString XmlSlideSource	=	_T("");
	
		/*double TimeTransition	=	1000.0;
		for ( size_t nIndex = 0; nIndex < m_arNormalSlides.GetCount() - 1; ++nIndex )
		{
			int KeyIndex = m_mapTransitions.FindKey ( (DWORD)nIndex );
			if ( -1 != KeyIndex )
			{
				if ( m_arNormalSlides [ nIndex ].m_dDuration < 2 * TimeTransition )
					m_arNormalSlides [ nIndex ].m_dDuration += 2 * TimeTransition;
			
				if ( m_arNormalSlides [ nIndex + 1 ].m_dDuration < 2 * TimeTransition )
					m_arNormalSlides [ nIndex + 1 ].m_dDuration += 2 * TimeTransition;
			}
		}*/

		for ( size_t nIndex = 0; nIndex < m_arNormalSlides.GetCount(); ++nIndex )
		{
			CString XmlCompose					= _T("");
			CString XmlAudioOverlayTransition	= _T("");

			bool bInsertCutBlack	=	false;
			bool bInsertCutNone		=	false;

			int KeyIndex = m_mapTransitions.FindKey ( (DWORD)nIndex );
			if ( -1 != KeyIndex )
			{
				CSlideShowSlideInfoAtom InfoAtom = m_mapTransitions.GetValueAt ( KeyIndex );

				/*
				if ( 0 == InfoAtom.m_nEffectType )
				{
					bInsertCutNone		=	true;
					if ( 1 == InfoAtom.m_nEffectDirection )
					{
						bInsertCutNone	=	false;
						bInsertCutBlack	=	true;
						//m_strXmlVideoSource	+=	_T("<ColorSource Color=\"0\" Duration=\"500\" />");	
					}

					//continue;
				}
				*/

				XmlCompose		=	GetXmlSlideTransition ( InfoAtom, m_arNormalSlides[nIndex].m_dStartTime, XmlAudioOverlayTransition, oAudioOverlay);
			}

			XmlSlideSource.Format ( _T("<ColorSource Color=\"16777215\" Duration=\"%lf\" widthmetric=\"%lf\" heightmetric=\"%lf\">"), 
				m_arNormalSlides [ nIndex ].m_dDuration, (double)m_arNormalSlides[nIndex].m_lWidth, (double)m_arNormalSlides[nIndex].m_lHeight );

			m_strXmlTransforms	=	_T("");
			//strVideoOverlay		=	_T("");
			//strAudioOverlay		=	_T("");

			m_nWriteSlideTime	=	m_arNormalSlides [ nIndex ].m_dDuration;
		
			double dScaleHor	=	(double)(m_arNormalSlides[nIndex].m_lWidth) / m_arNormalSlides[nIndex].m_lOriginalWidth;
			double dScaleVer	=	(double)(m_arNormalSlides[nIndex].m_lHeight) / m_arNormalSlides[nIndex].m_lOriginalHeight;

			// если на слайде есть анимации
			Animations::CSlideTimeLine* pTimeLine	=	m_mapAnimations.Lookup ( (DWORD)nIndex );

			strAudioOverlay += XmlAudioOverlayTransition;
			
			if ( NULL != pTimeLine )
			{
				WriteAnimationXml ( pTimeLine, &m_arNormalSlides[nIndex], dScaleHor, dScaleVer );
			}
			else
			{
				for ( int nElem = 0; nElem < m_arNormalSlides[nIndex].m_arElements.GetSize(); ++nElem )
				{
					CString strParam = m_arNormalSlides[nIndex].m_arElements[nElem]->ToXml();

					if (etVideo == m_arNormalSlides[nIndex].m_arElements[nElem]->m_etType)
					{
						strVideoOverlay += strParam;
					}
					else if (etAudio == m_arNormalSlides[nIndex].m_arElements[nElem]->m_etType)
					{
						// по идее уже не надо
						strAudioOverlay += strParam;
					}
					else
					{
						m_strXmlTransforms += strParam;
					}
				}
			}

			// m_nWriteSlideTimeOffset	+=	m_arNormalSlides[nIndex].m_dDuration;

		
			if ( -1 != KeyIndex && 0 == nIndex && XmlCompose.GetLength () > 1 )
			{
				CString strMem = _T("");
				strMem.Format(_T("<ColorSource Color=\"0\" Duration=\"1000\" widthmetric=\"%lf\" heightmetric=\"%lf\" />"),
					(double)m_arNormalSlides[nIndex].m_lWidth, (double)m_arNormalSlides[nIndex].m_lHeight );
				
				m_strXmlVideoSource	+=	strMem;	// 
			}
			
			m_strXmlVideoSource		+=	XmlCompose;
			
			XmlSlideSource			+=	_T("<VideoTransforms>")	+ m_strXmlTransforms	+	_T("</VideoTransforms>");
			//XmlSlideSource			+=	_T("<VideoOverlays>")	+ m_strXmlOverlays		+	_T("</VideoOverlays>");
			//m_strXmlOverlays        =   _T("<VideoOverlays>")	+ m_strXmlOverlays		+	_T("</VideoOverlays>");
			XmlSlideSource			+=	_T("</ColorSource>");
		
			m_strXmlVideoSource		+=	XmlSlideSource;
		}

		m_strXmlVideoSource			=	_T("<VideoSources>") + m_strXmlVideoSource + _T("</VideoSources>");
	}

	oAudioOverlay.Calculate();
	CString strAudioOverlay_ = oAudioOverlay.GetAudioOverlay();
	
	m_strXmlTransforms				=	_T("");
	m_strXmlOverlays        =   _T("<VideoOverlays>")	+ strVideoOverlay +	_T("</VideoOverlays>") + _T("<AudioOverlays>") + 
		_T("<AudioSources>") + strAudioOverlay_ +	_T("</AudioSources>") +	_T("</AudioOverlays>");

	m_strXmlVideoSource = _T("<SingleSource>") + m_strXmlVideoSource + _T("</SingleSource>");
	m_strXmlVideoSource = _T("<MultiSource>") + m_strXmlVideoSource + m_strXmlOverlays + _T("</MultiSource>");
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
}

void CPPTUserInfo::WriteAnimationXml ( Animations::CSlideTimeLine* pTimeLine, CSlide* pSlide, double nScaleW, double nScaleH )
{
	if (NULL == pSlide)
		return;	
	
	for ( int nElemInd = 0; nElemInd < pSlide->m_arElements.GetSize(); ++nElemInd )
	{
		IElement* pSlideElement = pSlide->m_arElements [ nElemInd ];
		if ( NULL != pSlideElement )
		{
			// анимации для видео
			if ( 0 == pSlideElement->m_etType )
			{
				// TODO : к видео так же должны быть применимы анимации ???
				m_strXmlOverlays	+=	pSlideElement->ToXml ();
				continue;
			}

			// анимации для картинок
			if ( 2 == pSlideElement->m_etType )
			{
				CImageElement* pImage	=	static_cast<CImageElement*> ( pSlideElement );
				if ( NULL != pImage )
				{
					AnimationsArray* pSources = pTimeLine->GetAnimation().Lookup ( pSlideElement->m_lID );
					if ( pSources )
					{
						m_strXmlTransforms	+=	GetAnimationImageXml ( static_cast<double> ( pSlide->m_lWidth ), static_cast<double> ( pSlide->m_lHeight ), pImage, pSources );
						continue;
					}
				}
			}

			// анимации для фигур
			if ( 3 == pSlideElement->m_etType ) 
			{
				CShapeElement* pShape	=	static_cast<CShapeElement*> ( pSlideElement );
				if ( NULL != pShape )
				{
					AnimationsArray* pSources = pTimeLine->GetAnimation().Lookup ( pSlideElement->m_lID );
					if ( pSources )
					{
						m_strXmlTransforms	+=	GetAnimationShapeXml ( static_cast<double> ( pSlide->m_lWidth ), static_cast<double> ( pSlide->m_lHeight ), pShape, pSources );
						continue;
					}
				}
			}

			m_strXmlTransforms	+=	pSlideElement->ToXml ();
		}
	}
}


CString CPPTUserInfo::GetXmlImageSource ( CImageElement* pImage )
{
	CString Source;	
	Source.Format (
		_T("left=\"%d\" top=\"%d\" right=\"%d\" bottom=\"%d\" filepath=\"%s\" ")
		_T("metric=\"0\" backcolor=\"-1\" scaletype=\"-1\" scalecolor=\"255\" "),
		pImage->m_rcBounds.left, pImage->m_rcBounds.top, pImage->m_rcBounds.right, pImage->m_rcBounds.bottom, CString ( CW2A ( pImage->m_strFileName ) ) );

	return Source; 
}

CString CPPTUserInfo::GetXmlAnimationSource ( double fX, double fY, AnimationsArray* pSource )
{
	CString XmlAnimationSource;

	// TODO : три эффекта максиум должно быть выполнятся на одном участке
	for ( UINT i = 0; i < pSource->GetCount(); ++i )
	{
		CString XmlEffect; 

		Animations::TimeLineElement& pESource = pSource->GetAt(i);

		// MOTION PATH EFFECT
		if ( 0x00000004 == pESource.m_nEffectType )
		{
			Animations::MotionPath path;
			if ( path.Create ( pESource.m_MotionPath ) )
			{
				XmlEffect.Format (	_T("<animation_effect>")
										_T("<effect_id>%d</effect_id>")
										_T("<effect_type>%d</effect_type>")
												_T("%s")							//	MOTION PATH
										_T("<time_delay>%f</time_delay>")
										_T("<time_duration>%f</time_duration>")
											_T("<plale_width>%f</plale_width>")
											_T("<plane_height>%f</plane_height>")
									_T("</animation_effect>"), 
									
									pESource.m_nEffectID,
									pESource.m_nEffectType,
									Animations::Serialize::CreateMotionPath ( path, fX, fY ),
									m_nWriteSlideTimeOffset + pESource.m_nTimeDelay,
									pESource.m_nDuration,
									fX, 
									fY );

									XmlAnimationSource	+=	XmlEffect;
			}
		}

		// ENTRANCE										EXIT
		if ( 0x00000001 == pESource.m_nEffectType || 0x00000002 == pESource.m_nEffectType )
		{
			XmlEffect.Format (	_T("<animation_effect>")
									_T("<effect_id>%d</effect_id>")
									_T("<effect_type>%d</effect_type>")
									_T("<effect_dir>%d</effect_dir>")
									_T("<time_delay>%f</time_delay>")
									_T("<time_duration>%f</time_duration>")
									_T("<plale_width>%f</plale_width>")
									_T("<plane_height>%f</plane_height>")
								_T("</animation_effect>"), 
									
								pESource.m_nEffectID,
								pESource.m_nEffectType,
								pESource.m_nEffectDir,
								m_nWriteSlideTimeOffset + pESource.m_nTimeDelay,
								pESource.m_nDuration,
								fX, 
								fY );

								XmlAnimationSource	+=	XmlEffect;
		}
	}	 

	return XmlAnimationSource;
}


CString CPPTUserInfo::GetAnimationShapeXml ( double fX, double fY, CShapeElement* pShape, AnimationsArray* pSource )
{
	CGeomShapeInfo infGeomShape;
	
	infGeomShape.m_dLeft	=	pShape->m_rcBounds.left;
	infGeomShape.m_dTop		=	pShape->m_rcBounds.top;
	infGeomShape.m_dWidth	=	pShape->m_rcBounds.right	-	pShape->m_rcBounds.left;
	infGeomShape.m_dHeight	=	pShape->m_rcBounds.bottom	-	pShape->m_rcBounds.top;

	infGeomShape.m_dLimoX	=	pShape->m_oShape.m_lLimoX;
	infGeomShape.m_dLimoY	=	pShape->m_oShape.m_lLimoY;

	CString Source	=	CString ( _T("") );

	for ( int i = 0; i < pShape->m_oShape.m_pShape->m_oPath.m_arParts.GetSize(); ++i )
	{
		Source	+=	CString ( _T("<ImagePaint-DrawGraphicPathAnimate>") );

		CString FillFormat;
		FillFormat.Format ( _T("<stroke>%s</stroke><fill>%s</fill><widthmm>%d</widthmm><heightmm>%d</heightmm>"), 
							NSAttributes::BoolToString ( pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].m_bStroke ), 
							NSAttributes::BoolToString ( pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].m_bFill ),
							pShape->m_oMetric.m_lMillimetresHor,
							pShape->m_oMetric.m_lMillimetresVer );

		Source	+=	FillFormat;

		if ( pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].m_bFill )
		{
			Source += pShape->m_oShape.m_oBrush.ToString2();
		}

		Source += pShape->m_oShape.m_oPen.ToString2 ();


		for ( int nIndex = 0; nIndex < pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].m_arSlices.GetSize(); ++nIndex )
		{
			Source += pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].m_arSlices [ nIndex ].ToXml ( infGeomShape, pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].width,
				pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].height, NSBaseShape::ppt);
		}

		CString TimeLine;
		TimeLine.Format ( _T("<timeline type = \"1\"  begin=\"%f\" end=\"%f\" fadein=\"0\" fadeout=\"0\" completeness=\"1.0\"/> "),
						m_nWriteSlideTimeOffset, m_nWriteSlideTimeOffset + m_nWriteSlideTime );

		Source	+=	GetXmlAnimationSource ( fX, fY, pSource );
		Source	+=	TimeLine;
		Source	+=	CString ( _T("</ImagePaint-DrawGraphicPathAnimate>") );
	}

	return Source;
}

CString CPPTUserInfo::GetAnimationImageXml ( double fX, double fY, CImageElement* pImage, AnimationsArray* pSource )
{
	CString Source	=	CString ( _T("") );
		Source.Format (	
				_T("<ImagePaint-DrawImageFromFileAnimate ")
				_T("%s")
				_T(" >")
				_T("%s")
				_T("<timeline type = \"1\"  begin=\"%f\" end=\"%f\" fadein=\"0\" fadeout=\"0\" completeness=\"1.0\"/> ")
				_T("</ImagePaint-DrawImageFromFileAnimate>"),
				GetXmlImageSource ( pImage ),
				GetXmlAnimationSource ( fX, fY, pSource ),
				m_nWriteSlideTimeOffset, m_nWriteSlideTimeOffset + m_nWriteSlideTime );

	return Source;
}

CString CPPTUserInfo::GetXmlSlideTransition ( CSlideShowSlideInfoAtom InfoAtom, double dStartTime, CString& strAudioTransition, CAudioOverlay& oAudioOverlay )
{
	CString Source	=	CString ( _T("") );

	int EffectID	=	1;

	switch ( InfoAtom.m_nEffectType )
	{
	case 0 : // Cut 
		{
			// Отрабатывается выше ( выкидывать слайд, или вставлять на несколько ms )
		}
		break;
	case 1 : // Random	//
		{
			// рандом вставить
			EffectID	=	1;							//	Fade Solid Smal
		}
		break;
	case  2 : // Blinds 
		{
			if ( 0x00 == InfoAtom.m_nEffectDirection )	//	Vertical	//	Slide Side Top To Bottom
				EffectID	=	1003;
			if ( 0x01 == InfoAtom.m_nEffectDirection )	//	Horizontal	//	Slide Side Left To Righ
				EffectID	=	1001;
		}
		break;
	case  3 : // Checker 
		{
			if ( 0x00 == InfoAtom.m_nEffectDirection )	//	Horizontal	//	WipeCheckerRightToLef
				EffectID	=	2901;
			if ( 0x01 == InfoAtom.m_nEffectDirection )	//	Vertical	//	WipeCheckerTopToBottom
				EffectID	=	2903;
		}
		break;
	case  4 : // Cover 
		{
			//0x00  Left 
#pragma message ("TODO : Cover - добавить такой эффект в ImageStudio")	
			//0x01  Up 
			//0x02  Right 
			//0x03  Down 
			//0x04  Left Up 
			//0x05  Right Up 
			//0x06  Left Down 
			//0x07  Right Down 
		}
		break;
	case  5 : // Dissolve 
		{
#pragma message ("TODO : Dissolve - добавить такой эффект в ImageStudio")
		
			EffectID	=	1;							//	Fade Solid Smal
		}
		break;
	case  6 : // Fade 
		{
			EffectID	=	4904;						//	Flash Dark
		}
		break;
	case  7 : // Uncover 
		{
#pragma message ("TODO : Uncover - добавить такой эффект в ImageStudio")	
			//0x00  Left 
			//0x01  Up 
			//0x02  Right 
			//0x03  Down 
			//0x04  Left Up 
			//0x05  Right Up 
			//0x06  Left Down 
			//0x07  Right Down 
		}
		break;
	case  8 : // Random Bars 
		{
			if ( 0x00 == InfoAtom.m_nEffectDirection )	//	Horizontal
				EffectID	=	5331;
			if ( 0x01 == InfoAtom.m_nEffectDirection )	//	Vertical
				EffectID	=	5332;
		}
		break;
	case  9 : // Strips 
		{
			if ( 0x04 == InfoAtom.m_nEffectDirection )	//	Left Up		//	Mosaic Right Bottom
				EffectID	=	110;
			if ( 0x05 == InfoAtom.m_nEffectDirection )	//	Right Up	//	Mosaic Left Bottom
				EffectID	=	111;
			if ( 0x06 == InfoAtom.m_nEffectDirection )	//	Left Down	//	Mosaic Right Top
				EffectID	=	109;
			if ( 0x07 == InfoAtom.m_nEffectDirection )	//	Right Down	//	Mosaic Left Top
				EffectID	=	108;
		}
		break;
	case  10: // Wipe 
		{
			if ( 0x00 == InfoAtom.m_nEffectDirection )	//	Left			//	Reveal, Left
				EffectID	=	5322;
			if ( 0x01 == InfoAtom.m_nEffectDirection )	//	Up				//	Reveal, Up
				EffectID	=	5323;
			if ( 0x02 == InfoAtom.m_nEffectDirection )	//	Right			//	Reveal, Right
				EffectID	=	5321;
			if ( 0x03 == InfoAtom.m_nEffectDirection )	//	Down			//	Reveal, Down
				EffectID	=	5320;
		}
		break;
	case  11: // Box In/Out 
		{
			if ( 0x00 == InfoAtom.m_nEffectDirection )	//	Wipe Center Box Out 
				EffectID	=	702;
			if ( 0x01 == InfoAtom.m_nEffectDirection )	//	Wipe Center Box In
				EffectID	=	701;
		}
		break;
	case  13 :// Split 
		{
			if ( 0x00 == InfoAtom.m_nEffectDirection )	//	Horizontally out	//	Split, Horizontal
				EffectID	=	5324;
#pragma message ("TODO : Split Horizontal In - добавить такой эффект в ImageStudio")	
			if ( 0x01 == InfoAtom.m_nEffectDirection )	//	Horizontally in		//	
				EffectID	=	5324;					
			if ( 0x02 == InfoAtom.m_nEffectDirection )	//	Vertically out		//	Split, Vertical
				EffectID	=	5325;
#pragma message ("TODO : Split Vertical In - добавить такой эффект в ImageStudio")
			if ( 0x03 == InfoAtom.m_nEffectDirection )	//	Vertically in		//	
				EffectID	=	5325;						
		}
		break;
	case  17 : // Diamond 
		{
			EffectID	=	704;						//	Wipe Center Diamond Out
		}
		break;
	case  18 : // Plus
		{
			EffectID	=	708;						//	Wipe Center Cross Out
		}
		break;
	case  19 : // Wedge 
		{
			EffectID	=	3006;						//	Clock Top Opposite
		}
		break;
	case 20 : // Push 
		{
			if ( 0x00 == InfoAtom.m_nEffectDirection )	//	Left			//	Push Side Right To Left
				EffectID	=	1402;
			if ( 0x01 == InfoAtom.m_nEffectDirection )	//	Up				//	Push Side Bottom To Top
				EffectID	=	1404;
			if ( 0x02 == InfoAtom.m_nEffectDirection )	//	Right			//	Push Side Left To Right
				EffectID	=	1401;
			if ( 0x03 == InfoAtom.m_nEffectDirection )	//	Down			//	Push Side Top To Bottom
				EffectID	=	1403;
		}
		break;
	case  21 : // Comb 
		{
			if ( 0x00 == InfoAtom.m_nEffectDirection )	//	Horizontal			//	MosaicStrips
				EffectID	=	103;
#pragma message ("TODO : MosaicStrips Vertical - добавить такой эффект в ImageStudio")
			if ( 0x01 == InfoAtom.m_nEffectDirection )	//	Vertical			//	MosaicStrips
				EffectID	=	103;
		}
		break;
	case  22 : // Newsflash 
		{
			EffectID	=	313;						//	Rotate Twice Out Clockwise
		}
		break;
	case  23 :// AlphaFade 
		{
			EffectID	=	1;							//	Fade Solid Smal
		}
		break;
	case 26 : // Wheel  
		{
			if ( 0x01 == InfoAtom.m_nEffectDirection )	//	ClockBottomReverse
				EffectID	=	3004;
			if ( 0x02 == InfoAtom.m_nEffectDirection )	//	Clock2AnglesClockwise
				EffectID	=	3013;
			if ( 0x03 == InfoAtom.m_nEffectDirection )	//	Clock4AnglesClockwise
				EffectID	=	3017;
			if ( 0x04 == InfoAtom.m_nEffectDirection )	//	Clock6AnglesClockwise
				EffectID	=	3019;
			if ( 0x08 == InfoAtom.m_nEffectDirection )	//	Clock8AnglesClockwise
				EffectID	=	3021;
		}
		break;
	case  27 : // Circle	
		{
			EffectID	=	706;						//	Wipe Center Circle Out
		}
		break;
	default :
		{
			return Source;
		}
		break;
	}

	double dTimeTransition = 500.0;
	if (0 == InfoAtom.m_nSpeed)
	{
		dTimeTransition = 750.0;
	}
	else if (2 == InfoAtom.m_nSpeed)
	{
		dTimeTransition = 250.0;
	}

	Source.Format (	_T("<VideoCompose Time = \"%lf\" effectid=\"%d\" />"), dTimeTransition, EffectID );	 

	if (InfoAtom.m_bSound)
	{
		int nIndexFound = m_mapSoundCollection.FindKey(InfoAtom.m_nSoundRef);
		if (-1 != nIndexFound)
		{
			CAudioElement oAudioElem;
			
			oAudioElem.m_dStartTime		= dStartTime - dTimeTransition;//dStartTime;
			oAudioElem.m_dEndTime		= oAudioOverlay.m_dAllDuration;//dStartTime + dTimeTransition; // по идее надо смотреть на loop и длительность аудио
			oAudioElem.m_strFileName	= m_mapSoundCollection.GetValueAt(nIndexFound);

			CAudioPart oPart(&oAudioElem);
			oPart.m_bStop = InfoAtom.m_bStopSound;
			oPart.m_bLoop = InfoAtom.m_bLoopSound;
			oPart.m_bIsTransition = true;

			oAudioOverlay.m_arParts.Add(oPart);

			strAudioTransition = oAudioElem.ToXml();
		}
	}
	
	return Source;
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

		CExFilesInfo* pInfo = m_mapExFiles.Lookup(0xFFFFFFFF);
		CString strTmpFolder = pInfo->m_strFilePath;

		for (int i = 0; i < nSize; ++i)
		{
			CSimpleArray<CRecordCString*>		oArrayStrings;
			CSimpleArray<CRecordSoundDataBlob*> oArrayData;

			oArraySounds[i]->GetRecordsByType(&oArrayStrings, false);
			oArraySounds[i]->GetRecordsByType(&oArrayData, false);

			if ((3 <= oArrayStrings.GetSize()) && (1 == oArrayData.GetSize()))
			{
				//CString _str = oArrayStrings[0]->m_strText;
				//CorrectXmlString(_str);

				//CString strFileName = strTmpFolder + _str + _T(".audio");
				//CString strFileNameSave = strTmpFolder + oArrayStrings[0]->m_strText + _T(".audio");

				//DWORD dwKey = (DWORD)XmlUtils::GetInteger(oArrayStrings[2]->m_strText);

				//oArrayData[0]->SaveToFile(strFileNameSave);
				//m_mapSoundCollection.Add(dwKey, strFileName);
				
				CString strFileName = strTmpFolder + oArrayStrings[2]->m_strText + _T(".audio");

				DWORD dwKey = (DWORD)XmlUtils::GetInteger(oArrayStrings[2]->m_strText);

				oArrayData[0]->SaveToFile(strFileName);
				
				m_mapSoundCollection.Add(dwKey, strFileName);
			}
		}
	}

	if (NULL == pExObjects)
		return;
		
	// читаем видео
	CSimpleArray<CRecordExVideoContainer*> oArray;
	pExObjects->GetRecordsByType(&oArray, true);

	for (int nIndex = 0; nIndex < oArray.GetSize(); ++nIndex)
	{
		LoadExternal(oArray[nIndex], CExFilesInfo::eftVideo);
	}

	oArray.RemoveAll();

	// читаем аудио
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
		LoadExternal(oArrayAudioMIDI[nIndex], CExFilesInfo::eftAudio);
	}
	for (int nIndex = 0; nIndex < nSize3; ++nIndex)
	{
		LoadExternal(oArrayWAVLink[nIndex], CExFilesInfo::eftAudio);
	}
	for (int nIndex = 0; nIndex < nSize1; ++nIndex)
	{
		DWORD dwKeyObj		= oArrayAudioCD[nIndex]->m_oMedia.m_nExObjID;

		CExFilesInfo* pInfo		= m_mapExFiles.Lookup(dwKeyObj);

		if (NULL != pInfo)
		{
			pInfo->m_dStartTime		= oArrayAudioCD[nIndex]->m_dStartTime;
			pInfo->m_dEndTime		= oArrayAudioCD[nIndex]->m_dEndTime;
			pInfo->m_bLoop			= oArrayAudioCD[nIndex]->m_oMedia.m_bLoop;
		}	
	}
	for (int nIndex = 0; nIndex < nSize4; ++nIndex)
	{
		DWORD dwKeySound	= oArrayAudioEmbedded[nIndex]->m_nSoundID;
		DWORD dwKeyObj		= oArrayAudioEmbedded[nIndex]->m_oMedia.m_nExObjID;

		int index = m_mapSoundCollection.FindKey(dwKeySound);
		if (index >= 0)
		{
			CExFilesInfo* pAudio	= new CExFilesInfo();
			pAudio->m_eType			= CExFilesInfo::eftAudio;
			pAudio->m_strFilePath	= m_mapSoundCollection.GetValueAt(index);
			pAudio->m_bLoop			= oArrayAudioEmbedded[nIndex]->m_oMedia.m_bLoop;

			m_mapExFiles.Add(dwKeyObj, pAudio);
		}
	}

	oArrayAudioCD.RemoveAll();
	oArrayAudioMIDI.RemoveAll();
	oArrayWAVLink.RemoveAll();
	oArrayAudioEmbedded.RemoveAll();

	// теперь запишем откуда файл открыт
	for (int i = 0; i < m_mapExFiles.GetSize(); ++i)
	{
		m_mapExFiles.GetValueAt(i)->m_strPresentationDirectory = m_strFileDirectory;
	}
}

void CPPTUserInfo::LoadExternal(CRecordsContainer* pExObject, CExFilesInfo::ExFilesType eType)
{
	CSimpleArray<CRecordExMediaAtom*> oArrayExMedia;
	CSimpleArray<CRecordCString*> oArrayCString;

	pExObject->GetRecordsByType(&oArrayExMedia, false);
	pExObject->GetRecordsByType(&oArrayCString, false);

	if ((1 == oArrayExMedia.GetSize()) && (1 == oArrayCString.GetSize()))
	{
		CExFilesInfo* pInfo		= new CExFilesInfo();
		pInfo->m_eType			= eType;
		pInfo->m_strFilePath	= oArrayCString[0]->m_strText;

		pInfo->m_bLoop			= oArrayExMedia[0]->m_bLoop;
		
		m_mapExFiles.Add(oArrayExMedia[0]->m_nExObjID, pInfo);
	}
	
	oArrayExMedia.RemoveAll();
	oArrayCString.RemoveAll();
}

double CPPTUserInfo::CalculateTimes(CAudioOverlay& oAudioOverlay)
{
	CaclulateSlideTimes();

	LONG lCount = (LONG)m_arNormalSlides.GetCount();

	double dAllDuration = 0;
	if (lCount > 0)
	{
		dAllDuration = m_arNormalSlides[lCount - 1].m_dEndTime;
	}

	oAudioOverlay.m_dAllDuration = dAllDuration;

	for (LONG i = 0; i < lCount; ++i)
	{
		CalculateSlideElements(i, oAudioOverlay);
	}

	return dAllDuration;
}

void CPPTUserInfo::CaclulateSlideTimes()
{
	double Transition1	= 0;
	double Duration		= 0;
	double Transition2	= 0;

	int nCount = (int)m_arNormalSlides.GetCount();
	for (int i = 0; i < nCount; ++i)
	{
		double dSlideDuration = 0;
		if (m_arNormalSlides[i].m_dReadDuration > 0)
		{
			dSlideDuration = m_arNormalSlides[i].m_dReadDuration;
		}
		if (0 == dSlideDuration)
		{
			// решено сделать так: если время слайда не выставлено - то берем максимальную
			// длительность видео, лежащего на нем. Если таких нет - то дефолтовая длительность,
			// впрочем она уже выставлена

			int nSize = m_arNormalSlides[i].m_arElements.GetSize();
			for (int j = 0; j < nSize; ++j)
			{
				if (etVideo == m_arNormalSlides[i].m_arElements[j]->m_etType)
				{
					dSlideDuration = max(dSlideDuration, ((CVideoElement*)m_arNormalSlides[i].m_arElements[j])->m_dVideoDuration);
				}
			}

			if (0 == dSlideDuration)
			{
				dSlideDuration = m_arNormalSlides[i].m_dDuration;
			}
		}

		m_arNormalSlides[i].m_dDuration = dSlideDuration;
		
		int index = m_mapTransitions.FindKey(i);
		if(index >= 0)
			Transition1 = m_mapTransitions.GetValueAt(index).GetTimeTransition();
		else
			Transition1 = 0;

		Duration = m_arNormalSlides[i].m_dDuration;

		index = m_mapTransitions.FindKey(i+1);
		if(index >= 0)
			Transition2 = m_mapTransitions.GetValueAt(index).GetTimeTransition();
		else
			Transition2 = 0;

		if (i == 0)
		{
			m_arNormalSlides[i].m_dStartTime = Transition1;
		}
		else
		{
			m_arNormalSlides[i].m_dStartTime = m_arNormalSlides[i-1].m_dStartTime + m_arNormalSlides[i-1].m_dDuration;
		}
		
		m_arNormalSlides[i].m_dDuration	= Transition1 + Duration + Transition2;
		m_arNormalSlides[i].m_dEndTime	= m_arNormalSlides[i].m_dStartTime + m_arNormalSlides[i].m_dDuration;
	}
}

void CPPTUserInfo::CalculateSlideElements(int nIndex, CAudioOverlay& oAudioOverlay)
{
	double dStartTime	= m_arNormalSlides[nIndex].m_dStartTime;
	double dEndTime		= m_arNormalSlides[nIndex].m_dEndTime;
	double dDuration	= m_arNormalSlides[nIndex].m_dDuration;
	
	CSimpleArray<IElement*>* pArray = &m_arNormalSlides[nIndex].m_arElements;

	int nCountElems = pArray->GetSize();
	for (int i = 0; i < nCountElems; ++i)
	{
		IElement* pElement = m_arNormalSlides[nIndex].m_arElements[i];

		switch (pElement->m_etType)
		{
		case etPicture:
		case etShape:
		case etVideo:
		case etText:
			{
				pElement->m_dStartTime	= 0;
				pElement->m_dEndTime	= dDuration;

				break;
			}
		case etAudio:
			{
				CAudioElement* pAudioElem = dynamic_cast<CAudioElement*>(pElement);

				if (NULL != pAudioElem)
				{
					// а другого и быть не может
					if (pAudioElem->m_bWithVideo)
					{
						pElement->m_dStartTime	= dStartTime;
						pElement->m_dEndTime	= dEndTime;
					}
					else if (pAudioElem->m_bLoop)
					{
						pElement->m_dStartTime	= dStartTime;
						pElement->m_dEndTime	= oAudioOverlay.m_dAllDuration/* - dStartTime*/;
					}
					else
					{
						pElement->m_dStartTime	= dStartTime;
						pElement->m_dEndTime	= min(dStartTime + pAudioElem->m_dAudioDuration, oAudioOverlay.m_dAllDuration/* - dStartTime*/);
					}

					CAudioPart oPart(pAudioElem);
					oAudioOverlay.m_arParts.Add(oPart);					
				}
				
				break;
			}
		default:
			{
				pElement->m_dStartTime	= 0;
				pElement->m_dEndTime	= dDuration;

				break;
			}
		};
	}
}
