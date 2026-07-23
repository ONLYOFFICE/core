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
#include "SlideInfo.h"

using namespace PPT;

CElementInfo::CElementInfo()
{
    m_lOffsetTextStyle	= -1;
    m_lOffsetTextProp	= -1;
    m_lPersistIndex		= -1;
    m_pStream			= NULL;

    m_lMasterTextType			= -1;
    m_lMasterPlaceholderType	= -1;
}

CElementInfo::CElementInfo(const CElementInfo &oSrc)
{
    *this = oSrc;
}

CElementInfo &CElementInfo::operator=(const CElementInfo &oSrc)
{
    m_lOffsetTextStyle	= oSrc.m_lOffsetTextStyle;
    m_lOffsetTextProp	= oSrc.m_lOffsetTextProp;
    m_lPersistIndex		= oSrc.m_lPersistIndex;
    m_pStream			= oSrc.m_pStream;

    m_lMasterTextType			= oSrc.m_lMasterTextType;
    m_lMasterPlaceholderType	= oSrc.m_lMasterPlaceholderType;

    return *this;
}

CElementInfo::~CElementInfo()
{
}

CSlideInfo::CSlideInfo()
{
}

CSlideInfo::~CSlideInfo()
{
}

CSlideInfo::CSlideInfo(const CSlideInfo &oSrc)
{
    *this = oSrc;
}

CSlideInfo &CSlideInfo::operator=(const CSlideInfo &oSrc)
{
    m_arTextPlaceHolders = oSrc.m_arTextPlaceHolders;

    m_parEmptyPictures	= oSrc.m_parEmptyPictures;
    m_mapFilePictures	= oSrc.m_mapFilePictures;

    for (int i = 0; i < 9; ++i)
    {
        m_pStyles[i] = oSrc.m_pStyles[i];
    }

    m_mapElements = oSrc.m_mapElements;

    return *this;
}

std::wstring CSlideInfo::GetFileNamePicture(_UINT32 lIndex)
{
    std::map<int, std::wstring>::iterator pic = m_mapFilePictures->find(lIndex);

    if (pic != m_mapFilePictures->end())
    {
        return pic->second;
    }
    return L"";
}

int CSlideInfo::GetIndexPicture(int lIndex)
{
    if (NULL == m_parEmptyPictures)
        return -1;
    if ( m_parEmptyPictures->size() < 1 )
        return -1;

    lIndex -= 1;

    if (lIndex < 0) lIndex = 0;

    if(lIndex >= 0 && lIndex < (int)m_parEmptyPictures->size())
    {
        return m_parEmptyPictures->at(lIndex);
    }
    else
    {
        return m_parEmptyPictures->at(0);//???
    }
}
