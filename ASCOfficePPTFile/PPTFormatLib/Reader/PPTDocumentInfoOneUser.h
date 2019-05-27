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

#include "../Records/Animations/AnimationTypes.h"
#include "../Records/ExObjListContainer.h"
#include "../Records/CryptSession10Container.h"

#include "SlideInfo.h"

class CPPTDocumentInfo;
using namespace PPT_FORMAT;

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
	std::vector<CFStreamPtr>						m_arStreamDecrypt; // на каждый Persist свой ... оО
	
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

	PPT_FORMAT::CTextStyles							m_oDefaultTextStyle;

	vector_string									m_PlaceholdersReplaceString[3]; //0-dates, 1 - headers, 2 - footers

	bool											m_bHasDate;
	bool											m_bHasSlideNumber;
	bool											m_bHasFooter;
	int												m_nFormatDate;

	CPPTDocumentInfo*								m_pDocumentInfo;
	CRecordVbaProjectStg*							m_VbaProjectStg;
	int												m_lIndexThisUser;

	std::map <_UINT32, Animations::CSlideTimeLine*>	m_mapAnimations;

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

	void CreateDefaultStyle(PPT_FORMAT::CTextStyles& pStyle, PPT_FORMAT::CTheme* pTheme);
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
	
	std::wstring ConvertLayoutType(INT nGeom, BYTE* pPlaceholders)
	{
		switch (nGeom)
		{
		case SL_TitleSlide:
		case SL_MasterTitle:				return L"title";
		case SL_TitleBody:
			{
				int ind = 0;
				if (pPlaceholders[0] == 13 && pPlaceholders[1] != 0) ind++;
				ePlaceholderType phbody = (ePlaceholderType)pPlaceholders[ind];
				switch (phbody)
				{
					case PT_MasterTitle:	return L"title";
					case PT_Table:			return L"tbl";
					case PT_OrgChart:		return L"dgm";
					case PT_Graph:			return L"chart";
				default:
					break;
				}
				return L"obj";
			}
		case SL_TitleOnly:					return L"titleOnly";
		case SL_TwoColumns:
			{
				ePlaceholderType leftType  = (ePlaceholderType)pPlaceholders[1];
                ePlaceholderType rightType = (ePlaceholderType)pPlaceholders[2];

				if (leftType == PT_Body && rightType == PT_Object)
                {
                    return L"txAndObj";
                }
				else if (leftType == PT_Object && rightType == PT_Body)
                {
                    return L"objAndTx";
                }
				else if (leftType == PT_Body && rightType == PT_ClipArt)
                {
                    return L"txAndClipArt";
                }
				else if (leftType == PT_ClipArt && rightType == PT_Body)
                {
                    return L"clipArtAndTx";
                }
				else if (leftType == PT_Body && rightType == PT_Graph)
                {
                    return L"txAndChart";
                }
				else if (leftType == PT_Graph && rightType == PT_Body)
                {
                    return L"chartAndTx";
                }
				else if (leftType == PT_Body && rightType == PT_Media)
                {
                    return L"txAndMedia";
                }
				else if (leftType == PT_Media && rightType == PT_Body)
                {
                    return L"mediaAndTx";
                }
                return L"twoObj";
			}
		case SL_TwoRows:
			{
				ePlaceholderType topType	= (ePlaceholderType)pPlaceholders[1];
                ePlaceholderType bottomType = (ePlaceholderType)pPlaceholders[2];

				if (topType == PT_Body && bottomType == PT_Object)
                {
                    return L"txOverObj";
                }
                return L"objOverTx";
			}
		case SL_ColumnTwoRows:
			{
				ePlaceholderType leftType = (ePlaceholderType)pPlaceholders[1];

				if (leftType == PT_Object)
                {
                    return L"objAndTwoObj";
                }
                return L"txAndTwoObj";
			}
		case SL_TwoRowsColumn:
			{
				ePlaceholderType rightType = (ePlaceholderType)pPlaceholders[2];

				if (rightType == PT_Object)
                {
					return L"twoObjAndObj";
                }
                return L"twoObjAndTx";
			}
		case SL_TwoColumnsRow:		return L"twoObjOverTx";
		case SL_FourObjects:		return L"fourObj";		
		case SL_BigObject:			return L"objOnly";
		case SL_Blank:				return L"blank";
		case SL_VerticalTitleBody:	return L"vertTitleAndTx";
		case SL_VerticalTwoRows:	return L"vertTx";
		}
		return L"blank";
	}
	
	void AddAnimation		(_UINT32 dwSlideID, double Width, double Height, CElementPtr pElement);
	void AddAudioTransition (_UINT32 dwSlideID, CTransition* pTransition, const std::wstring& strFilePath);

	int			AddNewLayout(PPT_FORMAT::CTheme* pTheme, CRecordSlide* pRecordSlide, bool addShapes, bool bMasterObjects);
	
	CElementPtr	AddNewLayoutPlaceholder		(CLayout *pLayout,	int placeholderType, int placeholderSizePreset = -1);

	CElementPtr	AddNewThemePlaceholder		(CTheme* pTheme,	int placeholderType, int placeholderSizePreset = -1);
	
	CElementPtr	AddThemeLayoutPlaceholder	(CLayout *pLayout,	int placeholderType, CTheme* pTheme, bool idx_only = false);
	CElementPtr	AddLayoutSlidePlaceholder	(CSlide *pSlide,	int placeholderType, CLayout *pLayout, bool idx_only = false);

	void LoadGroupShapeContainer(CRecordGroupShapeContainer* pGroup, std::vector<CElementPtr>* pParentElements, 
		CTheme* pTheme, CLayout* pLayout, CSlideInfo* pThemeWrapper, CSlideInfo* pSlideWrapper, CSlide* pSlide = NULL);
};
