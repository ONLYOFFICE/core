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
#include "TimeBehaviorContainer.h"

using namespace PPT;

CRecordTimeBehaviorContainer::CRecordTimeBehaviorContainer() :
    m_pStringList(nullptr),
    m_pPropertyList(nullptr),

    m_haveStringList(false),
    m_havePropertyList(false)
{

}

CRecordTimeBehaviorContainer::~CRecordTimeBehaviorContainer()
{
    RELEASEOBJECT (m_pStringList);
    RELEASEOBJECT (m_pPropertyList);
}

void CRecordTimeBehaviorContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    LONG lPos(0); StreamUtils::StreamPosition(lPos, pStream);

    SRecordHeader ReadHeader;

    if ( ReadHeader.ReadFromStream(pStream) )
        m_oBehaviorAtom.ReadFromStream ( ReadHeader, pStream );

    UINT lCurLen	=	8 + ReadHeader.RecLen;

    while ( lCurLen < m_oHeader.RecLen )
    {
        if (ReadHeader.ReadFromStream(pStream) == false)
            break;

        lCurLen			+=	8 + ReadHeader.RecLen;

        RecordType nRecord	=	ReadHeader.RecType;

        switch (nRecord)
        {
        case RT_TimeVariantList:
        {
            m_pStringList = new CRecordTimeStringListContainer();
            m_pStringList->ReadFromStream(ReadHeader, pStream);
            m_haveStringList = true;
            break;
        }

        case RT_TimePropertyList:
        {
            m_pPropertyList = new CRecordTimePropertyList4TimeBehavior();
            m_pPropertyList->ReadFromStream(ReadHeader, pStream);
            m_havePropertyList = true;
            break;
        }

        case RT_TimeClientVisualElement:
        {
            m_oClientVisualElement.ReadFromStream( ReadHeader, pStream );
        }

        default:
            break ;
        }
    }
    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}

_UINT32 CRecordTimeBehaviorContainer::GetObjectID()
{
    return m_oClientVisualElement.m_oVisualShapeAtom.m_nObjectIdRef;
}
