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
#ifndef _BUILD_APPLICATIONFONTS_H_
#define _BUILD_APPLICATIONFONTS_H_

#define UNKNOWN_CHARSET 3 // для случаев, когда задано значение DEFAULT_CHARSET, но 
                          // на самом деле charset не учитывается

#include <vector>
#include <list>
#include "FontManager.h"

namespace NSFonts
{
    class CLibrary_private
    {
    public:
        FT_Library m_library;
    };
}

class CFontRange
{
public:
    std::wstring Name;
    int Start;
    int End;
};

class CFontList : public NSFonts::IFontList
{
private:
    std::vector<NSFonts::CFontInfo*>	m_pList;
    std::wstring                        m_sDirectory;
    // вспомогогательные данные для подбора "похожих шрифтов"
    std::map<std::wstring, int> m_mapNamesToIndex;
    std::vector<std::vector<std::wstring> > m_listLikes;

    CFontRange*             m_pRanges;
    int                     m_nRangesCount;

    std::list<CFontRange>   m_listRanges; // последние использованные (найденные)

public:
	CFontList()
	{
        // 0
        m_mapNamesToIndex.insert(std::pair<std::wstring, int>(L"Cambria Math", 0));
        m_mapNamesToIndex.insert(std::pair<std::wstring, int>(L"Asana Math", 0));
        m_mapNamesToIndex.insert(std::pair<std::wstring, int>(L"XITS Math", 0));
        m_mapNamesToIndex.insert(std::pair<std::wstring, int>(L"Latin Modern", 0));

        std::vector<std::wstring> ar0;
        ar0.push_back(L"Cambria Math");
        ar0.push_back(L"Asana Math");
        ar0.push_back(L"XITS Math");
        ar0.push_back(L"Latin Modern");
        m_listLikes.push_back(ar0);

        // 1
        m_mapNamesToIndex.insert(std::pair<std::wstring, int>(L"Symbol", 1));
        std::vector<std::wstring> ar1;
        ar1.push_back(L"OpenSymbol");
        m_listLikes.push_back(ar1);

        m_pRanges = NULL;
        m_nRangesCount = 0;
	}
	~CFontList()
	{
        for ( std::vector<NSFonts::CFontInfo*>::iterator iter = m_pList.begin(); iter != m_pList.end(); iter++ )
		{
            NSFonts::CFontInfo* pTemp = *iter;
			RELEASEOBJECT(pTemp);
		}
        m_pList.clear();

        RELEASEARRAYOBJECTS(m_pRanges);
	}

    virtual std::vector<NSFonts::CFontInfo*>* GetFonts() { return &m_pList; }

private:
	int GetCharsetPenalty(ULONG ulCandRanges[6], unsigned char unReqCharset);
	int GetSigPenalty(ULONG ulCandRanges[6], ULONG ulReqRanges[6], double dRangeWeight = 1, double dRangeWeightSuferflouous = 0);
    int GetFixedPitchPenalty(INT bCandFixed, INT bReqFixed);
    int GetFaceNamePenalty(std::wstring sCandName, std::wstring sReqName, std::vector<std::wstring>* pArrayLikes = NULL);
	int GetFamilyUnlikelyPenalty(SHORT nCandFamilyClass, SHORT nReqFamilyClass);
	int GetFamilyUnlikelyPenalty(int nCandFamilyClass, std::wstring sReqFamilyClass);
	int GetWidthPenalty(USHORT usCandWidth, USHORT usReqWidth);
	int GetWeightPenalty(USHORT usCandWeight, USHORT usReqWeight);
    int GetItalicPenalty(INT bCandItalic, INT bReqItalic);
    int GetBoldPenalty(INT bCandBold, INT bReqBold);
    int GetFontFormatPenalty(EFontFormat eCandFormat, EFontFormat eReqFormat);
	int GetPanosePenalty(BYTE *pCandPanose, BYTE *pReqPanose);
	int GetAvgWidthPenalty(SHORT shCandWidth, SHORT shReqWidth);
	int GetAscentPenalty(SHORT shCandAscent, SHORT shReqAscent);
	int GetDescentPenalty(SHORT shCandDescent, SHORT shReqDescent);
	int GetLineGapPenalty(SHORT shCandLineGap, SHORT shReqLineGap);
	int GetXHeightPenalty(SHORT shCandXHeight, SHORT shReqXHeight);
	int GetCapHeightPenalty(SHORT shCandCapHeight, SHORT shReqCapHeight);

public:
    static EFontFormat GetFontFormat(FT_Face pFace);
    virtual void ToBuffer(BYTE** pDstData, LONG* pLen, std::wstring strDirectory = L"", bool bIsOnlyFileName = false);

public:
	void LoadFromArrayFiles (std::vector<std::wstring>& arrFiles, int nFlag = 0);
	void LoadFromFolder (const std::wstring& strDirectory);
	bool CheckLoadFromFolderBin	(const std::wstring& strDirectory);
    void Add (NSFonts::CFontInfo* pInfo);
    NSFonts::CFontInfo* GetByParams (NSFonts::CFontSelectFormat& oSelect, bool bIsDictionaryUse = true);
    std::vector<NSFonts::CFontInfo*> GetAllByName (const std::wstring& strFontName);

    std::wstring GetFontBySymbol(int symbol);
};

class CApplicationFonts : public NSFonts::IApplicationFonts
{
private:
	CApplicationFontStreams	m_oStreams;
	CFontsCache				m_oCache;
    CFontList				m_oList;

public:
	CApplicationFonts();
	~CApplicationFonts();

public:
    NSFonts::IFontsCache*				GetCache();
    NSFonts::IFontList*					GetList();
    NSFonts::IApplicationFontStreams*	GetStreams();

    void InitializeFromFolder(std::wstring strFolder, bool bIsCheckSelection = true);
    void Initialize(bool bIsCheckSelection = true);
      
	std::vector<std::wstring> GetSetupFontFiles();
	void InitializeFromArrayFiles(std::vector<std::wstring>& files, int nFlag = 0);

#if defined(_WIN32) || defined (_WIN64)
	void InitFromReg();
#endif
    
#ifdef _IOS
    std::vector<std::wstring> GetSetupFontFiles_ios();
#endif

    NSFonts::IFontManager* GenerateFontManager();

    std::wstring GetFontBySymbol(int symbol);
};

#endif
