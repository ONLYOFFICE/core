#pragma once
#include "ClassesAtom.h"

#include "../Records/Animations/AnimationTypes.h"
#include "../Records/ExObjListContainer.h"

#include "SlideInfo.h"

class CPPTDocumentInfo;
using namespace NSPresentationEditor;

class CPPTUserInfo : public CDocument
{
public:
	CUserEdit							m_oUser;
	CSimpleMap<DWORD, DWORD>			m_mapOffsetInPIDs;
	CRecordDocument						m_oDocument;

	CSimpleMap<DWORD, CRecordSlide*>	m_mapSlides;
	CSimpleMap<DWORD, CRecordSlide*>	m_mapMasters;
	CSimpleMap<DWORD, CRecordSlide*>	m_mapNotes;

	// перевод id мастера в индекс темы/шаблона
	CAtlMap<DWORD, LONG>				m_mapMasterToTheme;
	CAtlMap<DWORD, LONG>				m_mapMasterToLayout;

	// original id -> natural id
	CAtlMap<DWORD, DWORD>				m_mapMasterOriginalIds;

	// это как бы ППТ-шная обертка над слайдом
	CAtlArray<CSlideInfo>				m_arSlideWrapper;
	CAtlArray<CSlideInfo>				m_arMasterWrapper;

	// эти параметры - одни на весь документ. 
	// чтобы поддержать нашу схему (пптх) - копируем их в темы
	CSimpleArray<CFont>					m_arrFonts;
	NSPresentationEditor::CTextStyles	m_oDefaultTextStyle;

	// чтобы загружать неизмененные элементы от других юзеров (предыдущих)
	CPPTDocumentInfo*					m_pDocumentInfo;
	size_t								m_lIndexThisUser;

	// Animations structures
	CSimpleMap <DWORD, Animations::CSlideTimeLine*>	m_mapAnimations;

	double								m_nWriteSlideTimeOffset;
	double								m_nWriteSlideTime;

	CSimpleMap<DWORD, CSlideShowSlideInfoAtom> m_mapTransitions;

	// номера "пустых" картинок - в эти пути не будем сохранять
	CAtlArray<BOOL>						m_arEmptyPictures;
	BOOL								m_bIsSetupEmpty;

	CString								m_strFileDirectory;

	// вся инфа о ex - файлах
	CExMedia							m_oExMedia;

	// DEBUG information
	CString								m_strXmlInfo;

	std::vector<CColor>					m_oSchemeColors;

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

	void UpdateXMLInfo();

	void LoadSlideFromPrevUsers(int nSlideIndex);
	void LoadMasterFromPrevUsers(int nSlideIndex);
	void LoadNoteFromPrevUsers(int nSlideIndex);

	void LoadExternal(CRecordExObjListContainer* pExObjects);

	void LoadExVideo(CRecordsContainer* pExObject);
	void LoadExAudio(CRecordsContainer* pExObject);

	void CreateDefaultStyle(NSPresentationEditor::CTextStyles& pStyle, NSPresentationEditor::CTheme* pTheme)
	{
		for (int i = 0; i < 10; ++i)
		{
			if (!pStyle.m_pLevels[i].is_init())
				pStyle.m_pLevels[i] = new NSPresentationEditor::CTextStyleLevel();

			NSPresentationEditor::CTextPFRun* pPF = &pStyle.m_pLevels[i]->m_oPFRun;
			NSPresentationEditor::CTextCFRun* pCF = &pStyle.m_pLevels[i]->m_oCFRun;

			pPF->textAlignment	= (WORD)0;
			pPF->leftMargin		= (LONG)0;
			pPF->indent			= (LONG)0;
			pPF->defaultTabSize	= (LONG)0;
			pPF->fontAlign		= (WORD)0;
			pPF->wrapFlags		= (WORD)0x02;
			pPF->textDirection	= (WORD)0;
			pPF->lineSpacing	= (LONG)100;
			pPF->spaceBefore	= (LONG)0;
			pPF->spaceAfter		= (LONG)0;
			
			pCF->FontBold		= FALSE;
			pCF->FontItalic		= FALSE;
			pCF->FontUnderline	= FALSE;
			pCF->FontStrikeout	= FALSE;

			pCF->Size			= 18;
			pCF->BaseLineOffset = (double)0;
			pCF->Cap			= (WORD)0;

			pCF->FontProperties = new NSPresentationEditor::CFontProperties();
			pCF->FontProperties->SetFont(pTheme->m_arFonts[0]);

			pCF->Color			= new NSPresentationEditor::CColor();
		}
	}

	void CorrectColorScheme(std::vector<CColor>& oScheme)
	{
		std::vector<CColor> oArrayMem;
		oArrayMem.push_back(CColor());
		oArrayMem.push_back(oScheme[1]);
		oArrayMem.push_back(oScheme[2]);
		oArrayMem.push_back(oScheme[3]);
		oArrayMem.push_back(oScheme[0]);
		oArrayMem.push_back(oScheme[4]);
		oArrayMem.push_back(oScheme[5]);
		oArrayMem.push_back(oScheme[5]);//
		oArrayMem.push_back(oScheme[5]);//
		oArrayMem.push_back(oScheme[5]);//
		oArrayMem.push_back(oScheme[5]);//
		oArrayMem.push_back(oScheme[6]);
		oArrayMem.push_back(oScheme[7]);
		oArrayMem.push_back(oScheme[1]);
		oArrayMem.push_back(oScheme[2]);
		oArrayMem.push_back(oScheme[3]);
		oArrayMem.push_back(oScheme[0]);

		oScheme = oArrayMem;
	}

	CString ConvertLayoutType(INT nGeom, BYTE* pPlaceholders)
	{
		switch (nGeom)
		{
		case 0x00:
		case 0x02:
			return _T("title");
		case 0x01:
			{
				NSOfficePPT::PlaceholderEnum phbody = (NSOfficePPT::PlaceholderEnum)pPlaceholders[0];
				switch (phbody)
				{
				case NSOfficePPT::Table:
					return _T("tbl");
				case NSOfficePPT::OrganizationChart:
					return _T("dgm");
				case NSOfficePPT::Graph:
					return _T("chart");
				default:
					break;
				}
				return _T("obj");
			}
		case 0x07:
			return _T("titleOnly");
		case 0x08:
			{
				NSOfficePPT::PlaceholderEnum leftType  = (NSOfficePPT::PlaceholderEnum)pPlaceholders[1];
                NSOfficePPT::PlaceholderEnum rightType = (NSOfficePPT::PlaceholderEnum)pPlaceholders[2];

				if (leftType == NSOfficePPT::Body && rightType == NSOfficePPT::Object)
                {
                    return _T("txAndObj");
                }
				else if (leftType == NSOfficePPT::Object && rightType == NSOfficePPT::Body)
                {
                    return _T("objAndTx");
                }
				else if (leftType == NSOfficePPT::Body && rightType == NSOfficePPT::ClipArt)
                {
                    return _T("txAndClipArt");
                }
				else if (leftType == NSOfficePPT::ClipArt && rightType == NSOfficePPT::Body)
                {
                    return _T("clipArtAndTx");
                }
				else if (leftType == NSOfficePPT::Body && rightType == NSOfficePPT::Graph)
                {
                    return _T("txAndChart");
                }
				else if (leftType == NSOfficePPT::Graph && rightType == NSOfficePPT::Body)
                {
                    return _T("chartAndTx");
                }
				else if (leftType == NSOfficePPT::Body && rightType == NSOfficePPT::MediaClip)
                {
                    return _T("txAndMedia");
                }
				else if (leftType == NSOfficePPT::MediaClip && rightType == NSOfficePPT::Body)
                {
                    return _T("mediaAndTx");
                }
                return _T("twoObj");
			}
		case 0x09:
			{
				NSOfficePPT::PlaceholderEnum topType	= (NSOfficePPT::PlaceholderEnum)pPlaceholders[1];
                NSOfficePPT::PlaceholderEnum bottomType = (NSOfficePPT::PlaceholderEnum)pPlaceholders[2];

				if (topType == NSOfficePPT::Body && bottomType == NSOfficePPT::Object)
                {
                    return _T("txOverObj");
                }
                return _T("objOverTx");
			}
		case 0x0A:
			{
				NSOfficePPT::PlaceholderEnum leftType	= (NSOfficePPT::PlaceholderEnum)pPlaceholders[1];

				if (leftType == NSOfficePPT::Object)
                {
                    return _T("objAndTwoObj");
                }
                return _T("txAndTwoObj");
			}
		case 0x0B:
			{
				NSOfficePPT::PlaceholderEnum rightType = (NSOfficePPT::PlaceholderEnum)pPlaceholders[2];

				if (rightType == NSOfficePPT::Object)
                {
                    return _T("twoObjAndObj");
                }
                return _T("twoObjAndTx");
			}
		case 0x0D:
			return _T("twoObjOverTx");
		case 0x0F:
			return _T("objOnly");
		case 0x10:
			return _T("blank");
		case 0x0E:
			return _T("fourObj");
		case 0x11:
			return _T("vertTitleAndTx");
		case 0x12:
			return _T("vertTx");
		}
		return _T("obj");
	}

	void AddAnimation (int SlideIndex, double Width, double Height, IElement* pElement);
	void AddAudioTransition (long nSlide, CTransition* pTransition, const CString& strFilePath);
};