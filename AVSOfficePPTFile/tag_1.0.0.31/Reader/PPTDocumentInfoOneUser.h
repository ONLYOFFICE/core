#pragma once
#include "ClassesAtom.h"
#include "Slide.h"
//#include "Document.h"

#include "../Records/Animations/AnimationTypes.h"
#include "../Records/ExObjListContainer.h"

#include "AudioOverlay.h"

class CPPTDocumentInfo;

typedef CAtlArray<Animations::TimeLineElement> AnimationsArray;

class CPPTUserInfo
{
public:
	CUserEdit m_oUser;
	CSimpleMap<DWORD, DWORD> m_mapOffsetInPIDs;
	CRecordDocument m_oDocument;

	CSimpleMap<DWORD, CRecordSlide*> m_mapSlides;
	CSimpleMap<DWORD, CRecordSlide*> m_mapMasters;
	CSimpleMap<DWORD, CRecordSlide*> m_mapNotes;

	CAtlArray<CSlide> m_arNormalSlides;
	CSimpleMap<DWORD, CSlide*> m_arNormalMasters;

	// главная информация для картинок и видео...
	CSimpleMap<DWORD, CExFilesInfo*> m_mapExFiles;

	// все шрифты документа...
	CSimpleArray<CFont_> m_arrFonts;

	CString m_strXmlVideoSource;
	CString m_strXmlTransforms;
	CString m_strXmlOverlays;
	
	// чтобы по-быстрому сохранить ппт файл в xml
	CString m_strXmlInfo;

	// чтобы загружать неизмененные элементы от других юзеров (предыдущих)
	CPPTDocumentInfo* m_pDocumentInfo;
	size_t m_lIndexThisUser;

	// здесь, как и в каждом слайде - настройки текста
	CSimpleMap<DWORD, CTextMasterStyle*> m_mapTextStyles;
	
	// Animations structures
	CSimpleMap <DWORD, Animations::CSlideTimeLine*>	m_mapAnimations;

	double	m_nWriteSlideTimeOffset;
	double	m_nWriteSlideTime;

	CSimpleMap <DWORD, CSlideShowSlideInfoAtom> m_mapTransitions;

	CSimpleMap <DWORD, CString> m_mapSoundCollection;

	// номера "пустых" картинок - в эти пути не будем сохранять
	CAtlArray<BOOL> m_arEmptyPictures;
	BOOL m_bIsSetupEmpty;

	CString m_strFileDirectory;

public:

	CPPTUserInfo();
	~CPPTUserInfo();

	void Clear();

	BOOL ReadFromStream(CRecordUserEditAtom* pUser, IStream* pStream, CStringW strFolderMem);
	void FromDocument();

	void NormalizeCoords(long lWidth, long lHeight);

	void LoadSlide(int nIndex);
	void LoadMasters(const LONG& lOriginWidth, const LONG& lOriginHeight);
	void LoadNoMainMaster(int nIndex, const LONG& lOriginWidth, const LONG& lOriginHeight);
	void LoadMainMaster(int nIndex, const LONG& lOriginWidth, const LONG& lOriginHeight);

	void UpdateXMLSource();
	void UpdateXMLInfo();
	void WriteAnimationXml ( Animations::CSlideTimeLine* pTimeLine, CSlide* pSlide, double nScaleW, double nScaleH );
	
	CString GetXmlImageSource		( CImageElement* pImage );
	CString GetXmlShapeSource		( CShapeElement* pShape );
	CString GetAnimationShapeXml	( double fX, double fY, CShapeElement* pShape, AnimationsArray* pSource );
	CString GetAnimationImageXml	( double fX, double fY, CImageElement* pImage, AnimationsArray* pSource );
	CString GetXmlAnimationSource	( double fX, double fY, AnimationsArray* pSource );
	CString GetXmlSlideTransition	( CSlideShowSlideInfoAtom InfoAtom , double dStartTime, CString& strAudioOverlay, CAudioOverlay& oAudioOverlay );

	void LoadSlideFromPrevUsers(int nSlideIndex);
	void LoadMasterFromPrevUsers(int nSlideIndex);
	void LoadNoteFromPrevUsers(int nSlideIndex);

	void LoadExternal(CRecordExObjListContainer* pExObjects);
	void LoadExternal(CRecordsContainer* pExObject, CExFilesInfo::ExFilesType eType);

	double CalculateTimes(CAudioOverlay& oAudioOverlay);
	void CaclulateSlideTimes();
	void CalculateSlideElements(int nIndex, CAudioOverlay& oAudioOverlay);
};