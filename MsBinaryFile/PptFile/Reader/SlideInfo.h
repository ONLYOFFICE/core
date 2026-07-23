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
#include "../Drawing/Document.h"
#include "../Records/TextFullSettings.h"

namespace PPT
{
class CElementInfo
{
public:
    int		m_lOffsetTextStyle;
    int		m_lOffsetTextProp;
    POLE::Stream*	m_pStream;

    int		m_lPersistIndex;

    int		m_lMasterTextType;
    int		m_lMasterPlaceholderType;

public:
    CElementInfo();
    CElementInfo(const CElementInfo& oSrc);
    CElementInfo& operator=(const CElementInfo& oSrc);
    ~CElementInfo();
};

class CSlideInfo
{
public:
    std::vector<CTextFullSettings>						m_arTextPlaceHolders;
    std::map<LONG, CElementInfo>						m_mapElements;
    std::vector<int>*									m_parEmptyPictures;
    std::map<int, std::wstring>*						m_mapFilePictures;
    nullable_base<PPT::CTextStyles>	m_pStyles[9];

    CSlideInfo();
    ~CSlideInfo();

    CSlideInfo(const CSlideInfo& oSrc);
    CSlideInfo& operator=(const CSlideInfo& oSrc);

    std::wstring GetFileNamePicture(_UINT32 lIndex);

    int GetIndexPicture(int lIndex);
};
}
