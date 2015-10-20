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
	CUserEdit										m_oUser;
	std::map<DWORD, DWORD>							m_mapOffsetInPIDs;
	CRecordDocument									m_oDocument;

	//todooo при переходе на C++11 использовать НУЖНЫЙ здесь unsorted_map - m_arr .. Order уберутся

	std::map<DWORD, CRecordSlide*>					m_mapSlides;
	std::map<DWORD, CRecordSlide*>					m_mapMasters;
	std::map<DWORD, CRecordSlide*>					m_mapNotes;

	std::vector<DWORD>								m_arrSlidesOrder;
	std::vector<DWORD>								m_arrMastersOrder;
	std::vector<DWORD>								m_arrNotesOrder;

	// перевод id мастера в индекс темы/шаблона
	std::map<DWORD, LONG>							m_mapMasterToTheme;

	// original id -> natural id
	std::map<DWORD, DWORD>							m_mapMasterOriginalIds;

	// это как бы ППТ-шная обертка над слайдом
	std::vector<CSlideInfo>							m_arSlideWrapper;
	std::vector<CSlideInfo>							m_arMasterWrapper;

	// эти параметры - одни на весь документ. 
	// чтобы поддержать нашу схему (пптх) - копируем их в темы
	std::vector<CRecordBlipStoreContainer*>			m_arrBlipStore;
	std::vector<CFont>								m_arrFonts;
	NSPresentationEditor::CTextStyles				m_oDefaultTextStyle;

	// чтобы загружать неизмененные элементы от других юзеров (предыдущих)
	CPPTDocumentInfo*								m_pDocumentInfo;
	int												m_lIndexThisUser;

	// Animations structures
	std::map <DWORD, Animations::CSlideTimeLine*>	m_mapAnimations;

	double											m_nWriteSlideTimeOffset;
	double											m_nWriteSlideTime;

	std::map<DWORD, CSlideShowSlideInfoAtom>		m_mapTransitions;

	// номера "пустых" картинок - в эти пути не будем сохранять
    std::vector<int>								m_arOffsetPictures;
    bool											m_bIsSetupEmpty;

	CString											m_strFileDirectory;

	// вся инфа о ex - файлах
	CExMedia										m_oExMedia;

	std::vector<CColor>								m_oSchemeColors;

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

	void LoadSlideFromPrevUsers	(DWORD dwSlideID);
	void LoadMasterFromPrevUsers(DWORD dwSlideID);
	void LoadNoteFromPrevUsers	(DWORD dwSlideID);

	void LoadExternal(CRecordExObjListContainer* pExObjects);

	void LoadExVideo(CRecordsContainer* pExObject);
	void LoadExAudio(CRecordsContainer* pExObject);

	void CreateDefaultStyle(NSPresentationEditor::CTextStyles& pStyle, NSPresentationEditor::CTheme* pTheme);
	void CorrectColorScheme(std::vector<CColor>& oScheme)
	{
		if (oScheme.size() < 1) return;

		std::vector<CColor> oArrayMem;
		oArrayMem.push_back(oScheme[0]);//0
		oArrayMem.push_back(oScheme[1]);//1
		oArrayMem.push_back(oScheme[2]);//2
		oArrayMem.push_back(oScheme[3]);//3
		oArrayMem.push_back(oScheme[0]);//4
		oArrayMem.push_back(oScheme[4]);//5 //accent1
		oArrayMem.push_back(oScheme[5]);//6 //accent2
		oArrayMem.push_back(oScheme[0]);//7 //accent3
		oArrayMem.push_back(oScheme[5]);//8 //accent4
		oArrayMem.push_back(oScheme[4]);//9 //accent5
		oArrayMem.push_back(oScheme[7]);//10 //accent6
		oArrayMem.push_back(oScheme[6]);//11 //hlink
		oArrayMem.push_back(oScheme[7]);//12 //folHlink
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

	
	std::wstring ConvertLayoutType(INT nGeom, BYTE* pPlaceholders)
	{
		switch (nGeom)
		{
		case 0x00:	//SL_TitleSlide
		case 0x02:	//SL_MasterTitle
			return _T("title");
		case 0x01:	// SL_TitleBody
			{
				int ind = 0;
				if (pPlaceholders[0] == 13 && pPlaceholders[1] != 0) ind++;
				NSOfficePPT::PlaceholderEnum phbody = (NSOfficePPT::PlaceholderEnum)pPlaceholders[ind];
				switch (phbody)
				{
				case NSOfficePPT::MasterTitle:
					return _T("title");
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
		case 0x07:	//SL_TitleOnly
			return _T("titleOnly");
		case 0x08:	//SL_TwoColumns
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
		case 0x09:	//SL_TwoRows
			{
				NSOfficePPT::PlaceholderEnum topType	= (NSOfficePPT::PlaceholderEnum)pPlaceholders[1];
                NSOfficePPT::PlaceholderEnum bottomType = (NSOfficePPT::PlaceholderEnum)pPlaceholders[2];

				if (topType == NSOfficePPT::Body && bottomType == NSOfficePPT::Object)
                {
                    return _T("txOverObj");
                }
                return _T("objOverTx");
			}
		case 0x0A:	//SL_ColumnTwoRows
			{
				NSOfficePPT::PlaceholderEnum leftType	= (NSOfficePPT::PlaceholderEnum)pPlaceholders[1];

				if (leftType == NSOfficePPT::Object)
                {
                    return _T("objAndTwoObj");
                }
                return _T("txAndTwoObj");
			}
		case 0x0B:	//SL_TwoRowsColumn
			{
				NSOfficePPT::PlaceholderEnum rightType = (NSOfficePPT::PlaceholderEnum)pPlaceholders[2];

				if (rightType == NSOfficePPT::Object)
                {
                    return _T("twoObjAndObj");
                }
                return _T("twoObjAndTx");
			}
		case 0x0D:	//SL_TwoColumnsRow
			return _T("twoObjOverTx");
		case 0x0E://SL_FourObjects
			return _T("fourObj");		
		case 0x0F:	//SL_BigObject
			return _T("objOnly");
		case 0x10:	//SL_Blank
			return _T("blank");
		case 0x11:	//SL_VerticalTitleBody
			return _T("vertTitleAndTx");
		case 0x12:	//SL_VerticalTwoRows
			return _T("vertTx");
		}
		return _T("blank");
	}

	void AddAnimation		(DWORD dwSlideID, double Width, double Height, IElement* pElement);
	void AddAudioTransition (DWORD dwSlideID, CTransition* pTransition, const std::wstring& strFilePath);

	int AddNewLayout(NSPresentationEditor::CTheme* pTheme, SSlideLayoutAtom* layoutRecord, std::vector<CTextFullSettings> & text, bool addShapes = true);
};
