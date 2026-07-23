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
#include "UserEditAtom.h"

using namespace PPT;

CRecordUserEditAtom::CRecordUserEditAtom()
{
    m_nEncryptSessionPersistIdRef = 0;
}

CRecordUserEditAtom::~CRecordUserEditAtom()
{
}

void CRecordUserEditAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nLastSlideIdRef = StreamUtils::ReadDWORD(pStream);

    m_nVersion		= StreamUtils::ReadWORD(pStream);
    m_nMinorVersion = StreamUtils::ReadBYTE(pStream);
    m_nMajorVersion = StreamUtils::ReadBYTE(pStream);

    m_nOffsetLastEdit				= StreamUtils::ReadDWORD(pStream);
    m_nOffsetPersistDirectory		= StreamUtils::ReadDWORD(pStream);
    m_nOffsetDocPersistIdRef		= StreamUtils::ReadDWORD(pStream);

    m_nPersistIdSeed				= StreamUtils::ReadDWORD(pStream);
    m_nLastView						= StreamUtils::ReadWORD(pStream);

    StreamUtils::StreamSkip(2, pStream);

    if (m_oHeader.RecLen > 28)
    {
        m_nEncryptSessionPersistIdRef	= StreamUtils::ReadDWORD(pStream);
    }
}

CRecordUserEditAtom &CRecordUserEditAtom::operator =(const CRecordUserEditAtom &oSrc)
{
    m_nLastSlideIdRef				= oSrc.m_nLastSlideIdRef;

    m_nVersion						= oSrc.m_nVersion;
    m_nMinorVersion					= oSrc.m_nMinorVersion;
    m_nMajorVersion					= oSrc.m_nMajorVersion;

    m_nOffsetLastEdit				= oSrc.m_nOffsetLastEdit;
    m_nOffsetPersistDirectory		= oSrc.m_nOffsetPersistDirectory;
    m_nOffsetDocPersistIdRef		= oSrc.m_nOffsetDocPersistIdRef;
    m_nPersistIdSeed				= oSrc.m_nPersistIdSeed;
    m_nLastView						= oSrc.m_nLastView;
    m_nEncryptSessionPersistIdRef	= oSrc.m_nEncryptSessionPersistIdRef;
    return (*this);
}
