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
    return _T("");
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
