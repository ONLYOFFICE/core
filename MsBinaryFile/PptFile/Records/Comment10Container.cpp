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
#include "Comment10Container.h"

using namespace PPT;

void CRecordComment10AuthorAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_sCommentAuthor = StreamUtils::ReadStringW(pStream, m_oHeader.RecLen);
}

void CRecordComment10TextAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_sCommentText = StreamUtils::ReadStringW(pStream, m_oHeader.RecLen);
}

void CRecordComment10AuthorInitialAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_sCommentAuthorInitials = StreamUtils::ReadStringW(pStream, m_oHeader.RecLen);
}

void CRecordComment10Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nIndex = StreamUtils::ReadLONG(pStream);
    m_oDatetime.ReadFromStream(pStream);
    m_oAnchor.ReadFromStream(pStream);
}

CRecordComment10Container::CRecordComment10Container() :
    m_pCommentAuthorAtom(nullptr),
    m_pCommentTextAtom(nullptr),
    m_pCommentAuthorInitialsAtom(nullptr),

    m_haveAuthorAtom(false),
    m_haveTextAtom(false),
    m_haveAuthorInitialAtom(false)
{

}

CRecordComment10Container::~CRecordComment10Container()
{
    RELEASEOBJECT(m_pCommentAuthorAtom)
    RELEASEOBJECT(m_pCommentTextAtom)
            RELEASEOBJECT(m_pCommentAuthorInitialsAtom)
}

void CRecordComment10Container::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    LONG lPos(0); StreamUtils::StreamPosition(lPos, pStream);

    _UINT32 lCurLen(0);
    SRecordHeader ReadHeader;

    while (lCurLen < m_oHeader.RecLen) {
        if ( ReadHeader.ReadFromStream(pStream) == false )
        {
            break;
        }

        lCurLen += 8 + ReadHeader.RecLen;

        if (ReadHeader.RecType == RT_CString) {
            switch (ReadHeader.RecInstance) {
            case 0:
                m_pCommentAuthorAtom = new CRecordComment10AuthorAtom();
                m_pCommentAuthorAtom->ReadFromStream(ReadHeader, pStream);
                m_haveAuthorAtom = true;
                break;
            case 1:
                m_pCommentTextAtom = new CRecordComment10TextAtom();
                m_pCommentTextAtom->ReadFromStream(ReadHeader, pStream);
                m_haveTextAtom = true;
                break;

            case 2:
                m_pCommentAuthorInitialsAtom = new CRecordComment10AuthorInitialAtom();
                m_pCommentAuthorInitialsAtom->ReadFromStream(ReadHeader, pStream);
                m_haveAuthorInitialAtom = true;
                break;
            default:
                break;
            }

        } else
        {
            m_oCommentAtom.ReadFromStream(ReadHeader, pStream);
        }
    }

    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}
