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
#include "Slide.h"

using namespace PPT;

CSlide::CSlide() : m_oSlideShow()
{
    Clear();
}

CSlide::~CSlide()
{
    Clear();
}

void CSlide::Clear()
{
    m_arColorScheme.clear();
    m_arElements.clear();

    m_lThemeID			= -1;
    m_lLayoutID			= -1;
    m_lSlideID			= -1;
    m_lNotesID			= -1;

    m_dStartTime		= 0.0;
    m_dEndTime			= 0.0;
    m_dDuration			= 30000.0;

    m_bHidden = false;
    m_bShowMasterShapes = true;
    m_strComment.clear();
    m_sName.clear();

    for (int i = 0 ; i < 3 ; i++)
        m_PlaceholdersReplaceString[i].clear();
}

CSlide::CSlide(const CSlide &oSrc)
{
    Clear();

    size_t nCount = oSrc.m_arElements.size();
    for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
    {
        m_arElements.push_back(oSrc.m_arElements[nIndex]->CreateDublicate());
    }

    m_arColorScheme		= oSrc.m_arColorScheme;

    m_oSlideShow		= oSrc.m_oSlideShow;

    m_lThemeID			= oSrc.m_lThemeID;
    m_lLayoutID			= oSrc.m_lLayoutID;
    m_lNotesID			= oSrc.m_lNotesID;
    m_lSlideID			= oSrc.m_lSlideID;

    m_dStartTime		= oSrc.m_dStartTime;
    m_dEndTime			= oSrc.m_dEndTime;
    m_dDuration			= oSrc.m_dDuration;

    m_bIsBackground		= oSrc.m_bIsBackground;
    m_oBackground		= oSrc.m_oBackground;

    m_bHidden = oSrc.m_bHidden;
    m_bShowMasterShapes = oSrc.m_bShowMasterShapes;

    for (int i = 0 ; i < 3 ; i++) m_PlaceholdersReplaceString[i] = oSrc.m_PlaceholdersReplaceString[i];

    m_strComment		= oSrc.m_strComment;
    m_sName				= oSrc.m_sName;
}

void CSlide::SetUpPlaceholderStyles(CLayout *pLayout)
{
    size_t nCountElements = m_arElements.size();
    for (size_t nEl = 0; nEl < nCountElements; ++nEl)
    {
        if (-1 != m_arElements[nEl]->m_lPlaceholderType && etShape == m_arElements[nEl]->m_etType)
        {
            CShapeElement* pSlideElement = dynamic_cast<CShapeElement*>(m_arElements[nEl].get());

            if (NULL != pSlideElement)
            {
                LONG lCountThisType = pLayout->GetCountPlaceholderWithType(pSlideElement->m_lPlaceholderType);

                size_t nCountLayout = pLayout->m_arElements.size();
                for (size_t i = 0; i < nCountLayout; ++i)
                {
                    if (1 == lCountThisType)
                    {
                        if ((pLayout->m_arElements[i]->m_lPlaceholderType	== pSlideElement->m_lPlaceholderType) &&
                                (pLayout->m_arElements[i]->m_etType				== etShape))
                        {
                            CShapeElement* pLayoutElement = dynamic_cast<CShapeElement*>(pLayout->m_arElements[i].get());
                            if (NULL != pLayoutElement)
                            {
                                pSlideElement->m_pShape->m_oText.m_oLayoutStyles = pLayoutElement->m_pShape->m_oText.m_oStyles;
                            }
                        }
                    }
                    else
                    {
                        if ((pLayout->m_arElements[i]->m_lPlaceholderType	== pSlideElement->m_lPlaceholderType) &&
                                (pLayout->m_arElements[i]->m_lPlaceholderID		== pSlideElement->m_lPlaceholderID) &&
                                (pLayout->m_arElements[i]->m_etType				== etShape))
                        {
                            CShapeElement* pLayoutElement = dynamic_cast<CShapeElement*>(pLayout->m_arElements[i].get());
                            if (NULL != pLayoutElement)
                            {
                                pSlideElement->m_pShape->m_oText.m_oLayoutStyles = pLayoutElement->m_pShape->m_oText.m_oStyles;
                            }
                        }
                    }
                }
            }
        }
    }
}

ODRAW::CColor PPT::CSlide::GetColor(const LONG &lIndexScheme)
{
    if (lIndexScheme < (LONG)m_arColorScheme.size())
    {
        return m_arColorScheme[lIndexScheme];
    }
    return ODRAW::CColor();
}
