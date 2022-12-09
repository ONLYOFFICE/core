/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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
#pragma once
#include "ClassesAtom.h"

#include "../Records/Animations/_includer.h"
#include "../Enums/_includer.h"
#include "../Records/ExObjListContainer.h"
#include "../Records/CryptSession10Container.h"

#include "SlideInfo.h"

class CPPTDocumentInfo;

class CPPTUserInfo : public CDocument
{
public:
	CUserEdit										m_oUser;
	std::map<_UINT32, _UINT32>						m_mapOffsetInPIDs;
	CRecordDocument									m_oDocument;
	
	CEncryptionHeader								m_oEncryptionHeader;
	bool											m_bEncrypt;
	std::wstring									m_strPassword;
	CRYPT::ECMADecryptor*							m_pDecryptor;
	POLE::Storage*									m_pStorageDecrypt;
	std::vector<XLS::CFStreamPtr>					m_arStreamDecrypt; // на каждый Persist свой ... оО
	
	std::map<_UINT32, CRecordSlide*>				m_mapSlides;
	std::map<_UINT32, CRecordSlide*>				m_mapMasters;
	std::map<_UINT32, CRecordSlide*>				m_mapNotes;

	std::map<_UINT32, CRecordSlide*>				m_mapNotesMasters;
	std::map<_UINT32, CRecordSlide*>				m_mapHandoutMasters;

	std::vector<_UINT32>							m_arrSlidesOrder;
	std::vector<_UINT32>							m_arrMastersOrder;
	std::vector<_UINT32>							m_arrNotesOrder;

	// перевод id мастера в индекс темы/шаблона
	std::map<_UINT32, LONG>							m_mapMasterToTheme;

	// original id -> natural id
	std::map<_UINT32, _UINT32>							m_mapMasterOriginalIds;

	std::vector<CSlideInfo>							m_arSlideWrapper;
	std::vector<CSlideInfo>							m_arMasterWrapper;
	std::vector<CSlideInfo>							m_arNotesWrapper;

	CSlideInfo*										m_pNotesMasterWrapper;
	CSlideInfo*										m_pHandoutMasterWrapper;

	std::vector<CRecordBlipStoreContainer*>			m_arrBlipStore;
    std::vector<CFontProperty>						m_arrFonts;

    PPT::CTextStyles							m_oDefaultTextStyle;

	vector_string									m_PlaceholdersReplaceString[3]; //0-dates, 1 - headers, 2 - footers

	bool											m_bHasDate;
	bool											m_bHasSlideNumber;
	bool											m_bHasFooter;
	int												m_nFormatDate;

	CPPTDocumentInfo*								m_pDocumentInfo;
	CRecordVbaProjectStg*							m_VbaProjectStg;
	int												m_lIndexThisUser;

//	std::map <_UINT32, Animations::CSlideTimeLine*>	m_mapAnimations;

	double											m_nWriteSlideTimeOffset;
	double											m_nWriteSlideTime;

	std::map<_UINT32, CSlideShowSlideInfoAtom>		m_mapTransitions;

    std::vector<int>								m_arOffsetPictures;
    bool											m_bIsSetupEmpty;

    std::wstring									m_strTmpDirectory;

	// вся инфа о ex - файлах
	CExMedia										m_oExMedia;

	std::vector<CColor>								m_oSchemeColors;

	nullable<WORD>									m_wLanguage;	// язык пользователя (редактора)
	bool											m_bRtl;
	bool											m_bShowComments;

	int												m_current_level;
	std::vector<CElementPtr>*						m_current_elements;

//-----------------------------------------------------------------------------------------------------
	CPPTUserInfo();
	~CPPTUserInfo();

	void Clear();

    bool ReadFromStream(CRecordUserEditAtom* pUser, POLE::Stream* pStream);
	bool ReadDocumentPersists(POLE::Stream* pStream);
	void ReadExtenalObjects(std::wstring strFolderMem);

	void DecryptStream(POLE::Stream *pStream, int block);

	void FromDocument();

	void NormalizeCoords(long lWidth, long lHeight);

	void LoadSlide(_UINT32 dwSlideID, CSlide* pSlide);
	void LoadNotes(_UINT32 dwNotesID, CSlide* pSlide);
	
	void LoadMasters();
	
	void LoadNoMainMaster	(_UINT32 dwMasterID);
	void LoadMainMaster		(_UINT32 dwMasterID);
	
    void LoadMaster(_typeMaster type, CRecordSlide* pMaster, CSlideInfo *& pMasterWrapper, CThemePtr & pTheme);

	void LoadSlideFromPrevUsers			(_UINT32 dwSlideID);
	void LoadMasterFromPrevUsers		(_UINT32 dwSlideID);
	void LoadNotesFromPrevUsers			(_UINT32 dwSlideID);
	void LoadNotesMasterFromPrevUsers	(_UINT32 dwSlideID);
	void LoadHandoutMasterFromPrevUsers	(_UINT32 dwSlideID);

	void LoadExternal(CRecordExObjListContainer* pExObjects);

	void LoadExVideo(CRecordsContainer* pExObject);
	void LoadExAudio(CRecordsContainer* pExObject);

    void LoadAutoNumbering(CRecordGroupShapeContainer* pGroupContainer, PPT::CTheme* pTheme);
    void LoadBulletBlip(CShapeElement* pShape);
    void LoadAutoNumBullet(CShapeElement* pShape, int slideID);
    void CreateDefaultStyle(PPT::CTextStyles& pStyle, PPT::CTheme* pTheme);
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
		oArrayMem.push_back(CColor(0xAAAAAA));//(oScheme[0]);//7 //accent3
		oArrayMem.push_back(CColor(0xDCDCDC));//(oScheme[5]);//8 //accent4
		oArrayMem.push_back(CColor(0xDBF1FA));//(oScheme[4]);//9 //accent5
		oArrayMem.push_back(CColor(0x4B9DCA));//(oScheme[7]);//10 //accent6
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
	
	void ConvertLayoutType(SSlideLayoutAtom & layoutRecord, std::wstring & type, std::wstring & name)
	{
		name = L"Blank Slide";
		type = L"blank";

		switch (layoutRecord.m_nGeom)
		{
		case SL_TitleSlide:
		case SL_MasterTitle:
		{
			name = L"Title Slide";
			type = L"title";
		}break;
		case SL_TitleBody:
			{
				name = L"Title and Object Slide";
				type = L"obj";

				int ind = 0;
				if (layoutRecord.m_pPlaceHolderID[0] == 13 && layoutRecord.m_pPlaceHolderID[1] != 0) ind++;
                PlaceholderEnum phbody = (PlaceholderEnum)layoutRecord.m_pPlaceHolderID[ind];
				switch (phbody)
				{
					case PT_MasterTitle:	type = L"title";	name = L"Title and Object Slide";		break;
					case PT_Table:			type = L"tbl";		name = L"Title and Table Slide";		break;
					case PT_OrgChart:		type = L"dgm";		name = L"Title and Diagramma Slide";	break;
					case PT_Graph:			type = L"chart";	name = L"Title and Chart Slide";		break;
					default:
						break;
				}
			}break;
		case SL_TitleOnly:
		{
			name = L"Title Only Slide";
			type = L"titleOnly";
		}break;
		case SL_TwoColumns:
		{
            PlaceholderEnum leftType  = (PlaceholderEnum)layoutRecord.m_pPlaceHolderID[1];
            PlaceholderEnum rightType = (PlaceholderEnum)layoutRecord.m_pPlaceHolderID[2];

			name = L"Two Objects Slide";
			type = L"twoObj";

			if (leftType == PT_Body && rightType == PT_Object)
            {
				name = L"Text And Object Slide";
				type = L"txAndObj";
            }
			else if (leftType == PT_Object && rightType == PT_Body)
            {
				name = L"Object And Text Slide";
				type = L"objAndTx";
            }
			else if (leftType == PT_Body && rightType == PT_ClipArt)
            {
				name = L"Text And ClipArt Slide";
				type = L"txAndClipArt";
            }
			else if (leftType == PT_ClipArt && rightType == PT_Body)
            {
				name = L"ClipArt And Text Slide";
				type = L"clipArtAndTx";
            }
			else if (leftType == PT_Body && rightType == PT_Graph)
            {
				name = L"Text And Chart Slide";
				type = L"txAndChart";
            }
			else if (leftType == PT_Graph && rightType == PT_Body)
            {
				name = L"Chart And Text Slide";
				type = L"chartAndTx";
            }
			else if (leftType == PT_Body && rightType == PT_Media)
            {
				name = L"Text And Media Slide";
				type = L"txAndMedia";
            }
			else if (leftType == PT_Media && rightType == PT_Body)
            {
				name = L"Media And Text Slide";
				type = L"mediaAndTx";
            }
		}break;
		case SL_TwoRows:
		{
            PlaceholderEnum topType	= (PlaceholderEnum)layoutRecord.m_pPlaceHolderID[1];
            PlaceholderEnum bottomType = (PlaceholderEnum)layoutRecord.m_pPlaceHolderID[2];

			if (topType == PT_Body && bottomType == PT_Object)
            {
				name = L"Text Over Object Slide";
				type = L"txOverObj";
            }
			else
			{
				name = L"Object Over Text Slide";
				type = L"objOverTx";
			}
		}break;
		case SL_ColumnTwoRows:
		{
            PlaceholderEnum leftType = (PlaceholderEnum)layoutRecord.m_pPlaceHolderID[1];

			if (leftType == PT_Object)
            {
				type = L"objAndTwoObj";
            }
			else
			{
				type = L"txAndTwoObj";
			}
		}break;
		case SL_TwoRowsColumn:
		{
            PlaceholderEnum rightType = (PlaceholderEnum)layoutRecord.m_pPlaceHolderID[2];

			if (rightType == PT_Object)
            {
				type = L"twoObjAndObj";
            }
			else
			{
				type = L"twoObjAndTx";
			}
		}break;
		case SL_TwoColumnsRow:
		{
			type = L"twoObjOverTx";
		}break;
		case SL_FourObjects:
		{
			type = L"fourObj";		
		}break;
		case SL_BigObject:
		{
			name = L"Object Only Slide";
			type = L"objOnly";		
		}break;
		case SL_VerticalTitleBody:
		{
			type = L"vertTitleAndTx";		
		}break;
		case SL_VerticalTwoRows:
		{
			type = L"vertTx";		
		}break;
		case SL_Blank:
		default:
			break;
		}
	}
	
	void AddAnimation		(_UINT32 dwSlideID, double Width, double Height, CElementPtr pElement);
	void AddAudioTransition (_UINT32 dwSlideID, CTransition* pTransition, const std::wstring& strFilePath);

    int			AddNewLayout(PPT::CTheme* pTheme, CRecordSlide* pRecordSlide, bool addShapes, bool bMasterObjects);
	
	CElementPtr	AddNewLayoutPlaceholder		(CLayout *pLayout,	int placeholderType, int placeholderSizePreset = -1);

	CElementPtr	AddNewThemePlaceholder		(CTheme* pTheme,	int placeholderType, int placeholderSizePreset = -1);
	
	CElementPtr	AddThemeLayoutPlaceholder	(CLayout *pLayout,	int placeholderType, CTheme* pTheme, bool idx_only = false);
	CElementPtr	AddLayoutSlidePlaceholder	(CSlide *pSlide,	int placeholderType, CLayout *pLayout, bool idx_only = false);

	void LoadGroupShapeContainer(CRecordGroupShapeContainer* pGroup, std::vector<CElementPtr>* pParentElements, 
		CTheme* pTheme, CLayout* pLayout, CSlideInfo* pThemeWrapper, CSlideInfo* pSlideWrapper, CSlide* pSlide = NULL);
};
