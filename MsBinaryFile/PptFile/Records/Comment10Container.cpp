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
