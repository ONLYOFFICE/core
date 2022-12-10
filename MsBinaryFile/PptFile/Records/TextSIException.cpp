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
