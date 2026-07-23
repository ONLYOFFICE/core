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
#include "TextSIException.h"

using namespace PPT;

SSmartTags::SSmartTags(){}

SSmartTags::SSmartTags(const SSmartTags &other)
{
    m_count = other.m_count;
    m_rgSmartTagIndex = other.m_rgSmartTagIndex;
}

void SSmartTags::ReadFromStream(POLE::Stream *pStream)
{
    m_count = StreamUtils::ReadDWORD(pStream);
    for (_UINT32 i = 0; i < m_count; i++)
    {
        _UINT32 SmartTagIndex = StreamUtils::ReadDWORD(pStream);
        m_rgSmartTagIndex.push_back(SmartTagIndex);
    }
}

STextSIException::STextSIException()
{

}

STextSIException::~STextSIException()
{

}

void STextSIException::ReadFromStream(POLE::Stream *pStream)
{
    _UINT16 flags = StreamUtils::ReadWORD(pStream);
    StreamUtils::StreamSkip(2, pStream);

    m_spell     = 0x1 & flags;
    m_lang      = 0x2 & flags;
    m_altLang   = 0x4 & flags;


    m_fPp10ext  = 0x20 & flags;
    m_fBidi     = 0x40 & flags;


    m_smartTag  = 0x200 & flags;

    if (m_spell)
        m_spellInfo = StreamUtils::ReadWORD(pStream);
    if (m_lang)
        m_altLang   = StreamUtils::ReadWORD(pStream);
    if (m_altLang)
        m_altLid    = StreamUtils::ReadWORD(pStream);
    if (m_fBidi)
        m_bidi      = StreamUtils::ReadWORD(pStream);

    if (m_fPp10ext)
    {
        m_pp10runid     = 0x0F & StreamUtils::ReadBYTE(pStream);
        StreamUtils::StreamSkip(2, pStream);
        m_grammarError  = 0x80 & StreamUtils::ReadBYTE(pStream);
    }

    if (m_smartTag)
    {
        StreamUtils::StreamSkip(StreamUtils::ReadDWORD(pStream) * 4, pStream);
        //            m_smartTags = new SSmartTags;
        //            m_smartTags->ReadFromStream(pStream);
    }
}
