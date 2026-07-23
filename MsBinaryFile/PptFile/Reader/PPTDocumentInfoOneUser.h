/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "ClassesAtom.h"
#include "../Records/Animations/_includer.h"
#include "../Enums/_includer.h"
#include "../Records/ExObjListContainer.h"
#include "../Records/CryptSession10Container.h"
#include "../Records/DocumentRecords.h"
#include "../Records/SlideContainer.h"
#include "../Records/Drawing/BlipStoreContainer.h"
#include "../Records/Drawing/GroupShapeContainer.h"
#include "SlideInfo.h"


namespace PPT
{
class CPPTDocumentInfo;

class CPPTUserInfo : public CDocument
{
public:
    CPPTDocumentInfo*           m_pDocumentInfo = NULL;
    CUserEdit                   m_oUser;
    std::map<_UINT32, _UINT32>  m_mapOffsetInPIDs;
    CRecordDocument             m_oDocument;

    CEncryptionHeader               m_oEncryptionHeader;
    bool                            m_bEncrypt;
    std::wstring                    m_strPassword;
    CRYPT::Decryptor*               m_pDecryptor;
    POLE::Storage*                  m_pStorageDecrypt;
    std::vector<XLS::CFStreamPtr>   m_arStreamDecrypt; // each Persist has its own ... o_O

    std::map<_UINT32, CRecordSlide*>    m_mapSlides;
    std::map<_UINT32, CRecordSlide*>    m_mapMasters;
    std::map<_UINT32, CRecordSlide*>    m_mapNotes;

    std::map<_UINT32, CRecordSlide*>    m_mapNotesMasters;
    std::map<_UINT32, CRecordSlide*>    m_mapHandoutMasters;

    std::vector<_UINT32>    m_arrSlidesOrder;
    std::vector<_UINT32>    m_arrMastersOrder;
    std::vector<_UINT32>    m_arrNotesOrder;

    // translation of master id to theme/template index
    std::map<_UINT32, LONG> m_mapMasterToTheme;

    std::map<_UINT32, LONG> m_mapRealUsedMaster;

    // original id -> natural id
    std::map<_UINT32, _UINT32>  m_mapMasterOriginalIds;

    std::vector<CSlideInfo> m_arSlideWrapper;
    std::vector<CSlideInfo> m_arMasterWrapper;
    std::vector<CSlideInfo> m_arNotesWrapper;

    CSlideInfo* m_pNotesMasterWrapper;
    CSlideInfo* m_pHandoutMasterWrapper;

    std::vector<CRecordBlipStoreContainer*> m_arrBlipStore;
    std::vector<CFontProperty>              m_arrFonts;

    CTextStyles m_oDefaultTextStyle;

    vector_string   m_PlaceholdersReplaceString[3]; //0-dates, 1 - headers, 2 - footers

    bool    m_bHasDate;
    bool    m_bHasSlideNumber;
    bool    m_bHasFooter;
    int     m_nFormatDate;

    int m_lIndexThisUser;

    double  m_nWriteSlideTimeOffset;
    double  m_nWriteSlideTime;

    std::map<_UINT32, CSlideShowSlideInfoAtom>		m_mapTransitions;

    std::vector<int>    m_arOffsetPictures;
    bool                m_bIsSetupEmpty;

// all info about ex-files
    CExMedia                m_oExMedia;

    std::vector<CColor>     m_oSchemeColors;

    nullable<WORD>          m_wLanguage;	// user (editor) language
    bool                    m_bRtl;
    bool                    m_bShowComments;

    int                         m_current_level;
    std::vector<CElementPtr>*   m_current_elements;

//-----------------------------------------------------------------------------------------------------
    CPPTUserInfo();
    ~CPPTUserInfo();

    void Clear();

    bool ReadFromStream(CRecordUserEditAtom* pUser, POLE::Stream* pStream);
    bool ReadDocumentPersists(POLE::Stream* pStream);
    void ReadExtenalObjects();

    void DecryptStream(POLE::Stream *pStream, int block);

    void FromDocument();

    void NormalizeCoords(long lWidth, long lHeight);

    void LoadSlide(_UINT32 dwSlideID, CSlide* pSlide);
    void LoadNotes(_UINT32 dwNotesID, CSlide* pSlide);

    void TestSlide(_UINT32 dwSlideID);

    void LoadMasters();

    void LoadNoMainMaster	(_UINT32 dwMasterID);
    void LoadMainMaster		(_UINT32 dwMasterID, bool alwaysLoad);

    void LoadMaster(_typeMaster type, CRecordSlide* pMaster, CSlideInfo *& pMasterWrapper, CThemePtr & pTheme);

    void LoadSlideFromPrevUsers			(_UINT32 dwSlideID);
    void LoadMasterFromPrevUsers		(_UINT32 dwSlideID);
    void LoadNotesFromPrevUsers			(_UINT32 dwSlideID);
    void LoadNotesMasterFromPrevUsers	(_UINT32 dwSlideID);
    void LoadHandoutMasterFromPrevUsers	(_UINT32 dwSlideID);

    void LoadExternal(CRecordExObjListContainer* pExObjects);

    void LoadExVideo(CRecordsContainer* pExObject);
    void LoadExAudio(CRecordsContainer* pExObject);
    void LoadExOleObject(CRecordsContainer* pExObject);

    void LoadAutoNumbering(CRecordGroupShapeContainer* pGroupContainer, CTheme* pTheme);
    void LoadBulletBlip(CShapeElement* pShape);
    void LoadAutoNumBullet(CShapeElement* pShape, int slideID);
    void CreateDefaultStyle(CTextStyles& pStyle, CTheme* pTheme);
    void CorrectColorScheme(std::vector<CColor>& oScheme);

    void ConvertLayoutType(SSlideLayoutAtom & layoutRecord, std::wstring & type, std::wstring & name);

    void AddAnimation		(_UINT32 dwSlideID, double Width, double Height, CElementPtr pElement);
    void AddAudioTransition (_UINT32 dwSlideID, CTransition* pTransition, const std::wstring& strFilePath);

    int			AddNewLayout(CTheme* pTheme, CRecordSlide* pRecordSlide, bool addShapes, bool bMasterObjects);

    CElementPtr	AddNewLayoutPlaceholder		(CLayout *pLayout,	int placeholderType, int placeholderSizePreset = -1);

    CElementPtr	AddNewThemePlaceholder		(CTheme* pTheme,	int placeholderType, int placeholderSizePreset = -1);

    CElementPtr	AddThemeLayoutPlaceholder	(CLayout *pLayout,	int placeholderType, CTheme* pTheme, bool idx_only = false);
    CElementPtr	AddLayoutSlidePlaceholder	(CSlide *pSlide,	int placeholderType, CLayout *pLayout, bool idx_only = false);

    void LoadGroupShapeContainer(CRecordGroupShapeContainer* pGroup, std::vector<CElementPtr>* pParentElements,
                                 CTheme* pTheme, CLayout* pLayout, CSlideInfo* pThemeWrapper, CSlideInfo* pSlideWrapper, CSlide* pSlide = NULL);
};
}
