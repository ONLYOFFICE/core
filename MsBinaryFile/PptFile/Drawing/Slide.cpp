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
