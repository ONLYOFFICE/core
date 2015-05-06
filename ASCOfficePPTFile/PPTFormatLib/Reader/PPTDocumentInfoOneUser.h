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
	std::map<DWORD, DWORD>				m_mapOffsetInPIDs;
	CRecordDocument						m_oDocument;

	//todooo при переходе на C++11 использовать НУЖНЫЙ здесь unsorted_map - m_arr .. Order уберутся
	
	std::map<DWORD, CRecordSlide*>		m_mapSlides;
	std::map<DWORD, CRecordSlide*>		m_mapMasters;
	std::map<DWORD, CRecordSlide*>		m_mapNotes;

	std::vector<DWORD>					m_arrSlidesOrder;
	std::vector<DWORD>					m_arrMastersOrder;
	std::vector<DWORD>					m_arrNotesOrder;

	// перевод id мастера в индекс темы/шаблона
	std::map<DWORD, LONG>				m_mapMasterToTheme;
	std::map<DWORD, LONG>				m_mapMasterToLayout;

	// original id -> natural id
	std::map<DWORD, DWORD>				m_mapMasterOriginalIds;

	// это как бы ППТ-шная обертка над слайдом
	std::vector<CSlideInfo>						m_arSlideWrapper;
	std::vector<CSlideInfo>						m_arMasterWrapper;

	// эти параметры - одни на весь документ. 
	// чтобы поддержать нашу схему (пптх) - копируем их в темы
	std::vector<CFont>					m_arrFonts;
	NSPresentationEditor::CTextStyles	m_oDefaultTextStyle;

	// чтобы загружать неизмененные элементы от других юзеров (предыдущих)
	CPPTDocumentInfo*					m_pDocumentInfo;
	size_t								m_lIndexThisUser;

	// Animations structures
	std::map <DWORD, Animations::CSlideTimeLine*>	m_mapAnimations;

	double								m_nWriteSlideTimeOffset;
	double								m_nWriteSlideTime;

	std::map<DWORD, CSlideShowSlideInfoAtom> m_mapTransitions;

	// номера "пустых" картинок - в эти пути не будем сохранять
    std::vector<bool>					m_arEmptyPictures;
    bool								m_bIsSetupEmpty;

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

    bool ReadFromStream(CRecordUserEditAtom* pUser, POLE::Stream* pStream, CStringW strFolderMem);
	void FromDocument();

	void NormalizeCoords(long lWidth, long lHeight);

	void LoadSlide(DWORD dwSlideID, CSlide* pSlide);
	
	void LoadMasters(const LONG& lOriginWidth, const LONG& lOriginHeight);
	
	void LoadNoMainMaster	(DWORD dwMasterID, const LONG& lOriginWidth, const LONG& lOriginHeight);
	void LoadMainMaster		(DWORD dwMasterID, const LONG& lOriginWidth, const LONG& lOriginHeight);

	void UpdateXMLInfo();

	void LoadSlideFromPrevUsers	(DWORD dwSlideID);
	void LoadMasterFromPrevUsers(DWORD dwSlideID);
	void LoadNoteFromPrevUsers	(DWORD dwSlideID);

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
			
            pCF->FontBold		= false;
            pCF->FontItalic		= false;
            pCF->FontUnderline	= false;
            pCF->FontStrikeout	= false;

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
		oArrayMem.push_back(oScheme[0]);//0
		oArrayMem.push_back(oScheme[1]);//1
		oArrayMem.push_back(oScheme[2]);//2
		oArrayMem.push_back(oScheme[3]);//3
		oArrayMem.push_back(oScheme[0]);//4
		oArrayMem.push_back(oScheme[5]);//5 //accent1
		oArrayMem.push_back(oScheme[6]);//6 //accent2
		oArrayMem.push_back(oScheme[7]);//7 //accent3
		oArrayMem.push_back(oScheme[5]);//8 //accent4
		oArrayMem.push_back(oScheme[6]);//9 //accent5
		oArrayMem.push_back(oScheme[7]);//10 //accent6
		oArrayMem.push_back(oScheme[5]);//11 //hlink
		oArrayMem.push_back(oScheme[6]);//12 //folHlink
		oArrayMem.push_back(oScheme[0]);//13 //lt1
		oArrayMem.push_back(oScheme[1]);//14 //dk1
		oArrayMem.push_back(oScheme[2]);//15 //lt2
		oArrayMem.push_back(oScheme[3]);//16 //dk2

//0x00 //Background color
//0x01 //Text color
//0x02 //Shadow color
//0x03 //Title text color
//0x04 //Fill color
//0x05 //Accent 1 color
//0x06 //Accent 2 color
//0x07 //Accent 3 color

		oScheme  = oArrayMem;
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

	void AddAnimation (DWORD dwSlideID, double Width, double Height, IElement* pElement);
	void AddAudioTransition (DWORD dwSlideID, CTransition* pTransition, const CString& strFilePath);
};
