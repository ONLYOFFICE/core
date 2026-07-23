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

#include "AudioOverlay.h"
#include "Slide.h"

namespace PPT
{
class CDocument
{
public:
    long m_lSlideWidth;
    long m_lSlideHeight;

    long m_lNotesWidth;
    long m_lNotesHeight;

    std::vector<CThemePtr>	m_arThemes;
    std::vector<CSlide*>	m_arSlides;
    std::vector<CSlide*>	m_arNotes;

    CThemePtr				m_pNotesMaster;
    CThemePtr				m_pHandoutMaster;

    bool					m_bMacroEnabled;
    std::wstring			m_sVbaProjectFile;

    CDocument() : m_bMacroEnabled(true)
    {
        m_lSlideWidth	= 0;
        m_lSlideHeight	= 0;

        m_lNotesWidth	= 0;
        m_lNotesHeight	= 0;
    }

    virtual ~CDocument()
    {
        Clear();
    }

    void Clear()
    {
        m_arThemes.clear();
        try
        {
            ClearSlides();
            ClearNotes();
        }catch(...)
        {
        }
    }
    inline void ClearSlides()
    {
        for (size_t i = 0 ; i < m_arSlides.size(); i++)
        {
            RELEASEOBJECT(m_arSlides[i]);
        }
        m_arSlides.clear();
    }
    inline void ClearNotes()
    {
        for (size_t i = 0 ; i < m_arNotes.size(); i++)
        {
            RELEASEOBJECT(m_arNotes[i]);
        }
        m_arNotes.clear();
    }
    void CalculateSlideElements(int nIndex, CAudioOverlay& oAudioOverlay)
    {
        double dStartTime	= m_arSlides[nIndex]->m_dStartTime;
        double dEndTime		= m_arSlides[nIndex]->m_dEndTime;
        double dDuration	= m_arSlides[nIndex]->m_dDuration;

        CSlide* pSlide = m_arSlides[nIndex];

        size_t nCountElems = pSlide->m_arElements.size();
        for (size_t i = 0; i < nCountElems; ++i)
        {
            CElementPtr pElement = pSlide->m_arElements[i];

            switch (pElement->m_etType)
            {
            case etAudio:
            {
                CAudioElement* pAudioElem = dynamic_cast<CAudioElement*>(pElement.get());

                if (NULL != pAudioElem)
                {
                    // and there can be no other
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
                        pElement->m_dEndTime	= (std::min)(dStartTime + pAudioElem->m_dAudioDuration, oAudioOverlay.m_dAllDuration/* - dStartTime*/);
                    }

                    CAudioPart oPart(pAudioElem);
                    oAudioOverlay.m_arParts.push_back(oPart);
                }

                break;
            }
            default:
                //{
                //pElement->m_dStartTime	= 0;
                //pElement->m_dEndTime	= dDuration;

                break;
                //}
            };
        }
    }

    void ResetAutoText(CElementPtr pElement, vector_string const (&placeholdersReplaceString)[3])
    {
        CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElement.get());

        if (NULL == pShape) return;

        if (pElement->m_lPlaceholderType == PT_MasterSlideNumber)	pShape->SetUpTextPlaceholder(L"<#>");

        int ind = -1;
        if (pElement->m_lPlaceholderType == PT_MasterDate)	ind = 0;
        if (pElement->m_lPlaceholderType == PT_MasterFooter)	ind = 2;

        if (pElement->m_lPlaceholderUserStr >= 0 && ind >= 0)
        {
            if (pElement->m_lPlaceholderUserStr < (int)placeholdersReplaceString[ind].size())
                pShape->SetUpTextPlaceholder( placeholdersReplaceString[ind][pElement->m_lPlaceholderUserStr] );
            else pShape->SetUpTextPlaceholder(L"");
        }
        else
        {
            if (pElement->m_lPlaceholderType == PT_MasterDate && pElement->m_nFormatDate == 1)
            {
                std::wstring current_date = L"11.11.2015";
                pShape->SetUpTextPlaceholder(current_date);
            }
        }
    }

    void CalculateEditor(bool bIsPlaceholderSetUp = false)
    {
        // we configure auto-replacements and fields here to avoid confusion

        size_t nCountThemes = m_arThemes.size();
        for (size_t i = 0; i < nCountThemes; ++i)
        {
            CTheme* pTheme = m_arThemes[i].get();
            pTheme->CalculateStyles();

            size_t nCountElems = pTheme->m_arElements.size();
            for (size_t nIndexEl = 0; nIndexEl < nCountElems; ++nIndexEl)
            {
                CElementPtr pElement = pTheme->m_arElements[nIndexEl];

                if (pElement->m_lPlaceholderType > 0)
                {
                    ResetAutoText(pElement, pTheme->m_PlaceholdersReplaceString);
                }
                pElement->m_pTheme = pTheme;
                pElement->m_pLayout = NULL;
            }

            size_t nCountLayouts = pTheme->m_arLayouts.size();
            for (size_t nIndexL = 0; nIndexL < nCountLayouts; ++nIndexL)
            {
                CLayout* pLayout = pTheme->m_arLayouts[nIndexL].get();

                size_t nCountLayoutElements = pLayout->m_arElements.size();
                for (size_t nIndexLayoutEl = 0; nIndexLayoutEl < nCountLayoutElements; ++nIndexLayoutEl)
                {
                    CElementPtr pElement = pLayout->m_arElements[nIndexLayoutEl];

                    if (pElement->m_lPlaceholderType > 0)
                    {
                        ResetAutoText(pElement, pLayout->m_PlaceholdersReplaceString);
                    }

                    pElement->m_pTheme = pTheme;
                    pElement->m_pLayout = NULL;

                    CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElement.get());
                    if (!pLayout->m_bUseThemeColorScheme && NULL != pShape)
                    {
                        int lPhType = pElement->m_lPlaceholderType;

                        int lIndex = 0;

                        if (isTitlePlaceholder(lPhType))		lIndex = 1;
                        else if (isBodyPlaceholder(lPhType))	lIndex = 2;
                        else if (-1 != lPhType)					lIndex = 3;

                        CTextStyles* pThemeStyles = &pTheme->m_pStyles[lIndex];
                        for (int nIndexLevel = 0; nIndexLevel < 10; ++nIndexLevel)
                        {
                            if (!pThemeStyles->m_pLevels[nIndexLevel].is_init())
                                continue;
                            if (!pThemeStyles->m_pLevels[nIndexLevel]->m_oCFRun.Color.is_init())
                                continue;
                            if (pThemeStyles->m_pLevels[nIndexLevel]->m_oCFRun.Color->m_lSchemeIndex == -1)
                                continue;

                            if (pShape->m_pShape->m_oText.m_oStyles.m_pLevels[0].is_init())
                            {
                                if (pShape->m_pShape->m_oText.m_oStyles.m_pLevels[0]->m_oCFRun.Color.is_init())
                                {

                                    if (pShape->m_pShape->m_oText.m_oStyles.m_pLevels[0]->m_oCFRun.Color->m_lSchemeIndex != -1)
                                        continue;

                                    LONG lIndexSchemeT = pThemeStyles->m_pLevels[nIndexLevel]->m_oCFRun.Color->m_lSchemeIndex;

                                    pShape->m_pShape->m_oText.m_oStyles.m_pLevels[0]->m_oCFRun.Color->m_lSchemeIndex = -1;
                                    pShape->m_pShape->m_oText.m_oStyles.m_pLevels[0]->m_oCFRun.Color->R = pLayout->m_arColorScheme[lIndexSchemeT].R;
                                    pShape->m_pShape->m_oText.m_oStyles.m_pLevels[0]->m_oCFRun.Color->G = pLayout->m_arColorScheme[lIndexSchemeT].G;
                                    pShape->m_pShape->m_oText.m_oStyles.m_pLevels[0]->m_oCFRun.Color->B = pLayout->m_arColorScheme[lIndexSchemeT].B;

                                    bIsPlaceholderSetUp = true;
                                }
                            }
                        }
                    }
                }
            }
        }

        size_t nCountSlides = m_arSlides.size();
        for (size_t i = 0; i < nCountSlides; ++i)
        {
            CSlide* pSlide = m_arSlides[i];

            CTheme* pTheme = NULL;
            if ((0 <= pSlide->m_lThemeID) && (pSlide->m_lThemeID < (LONG)nCountThemes))
                pTheme = m_arThemes[pSlide->m_lThemeID].get();

            CLayout* pLayout = NULL;
            if (NULL != pTheme)
            {
                if ((0 <= pSlide->m_lLayoutID) && (pSlide->m_lLayoutID < (LONG)pTheme->m_arLayouts.size()))
                    pLayout = pTheme->m_arLayouts[pSlide->m_lLayoutID].get();
            }

            size_t nCountElems = pSlide->m_arElements.size();
            for (size_t nIndexEl = 0; nIndexEl < nCountElems; ++nIndexEl)
            {
                CElementPtr pElement = pSlide->m_arElements[nIndexEl];

                if (pElement->m_lPlaceholderType > 0)
                {
                    ResetAutoText(pElement, pSlide->m_PlaceholdersReplaceString);
                }
                pElement->m_pTheme = pTheme;
                pElement->m_pLayout = pLayout;

            }

            if (NULL != pLayout && bIsPlaceholderSetUp)
                pSlide->SetUpPlaceholderStyles(pLayout);
        }
    }
};
}
