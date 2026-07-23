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
#include "TimePropertyList4TimeNodeContainer.h"

using namespace PPT;

CRecordTimePropertyList4TimeNodeContainer::CRecordTimePropertyList4TimeNodeContainer()
{
    m_bEmtyNode					=	false;
}
CRecordTimePropertyList4TimeNodeContainer::~CRecordTimePropertyList4TimeNodeContainer()
{
    ClearNodes ();
}

void CRecordTimePropertyList4TimeNodeContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    LONG lPos = 0;
    StreamUtils::StreamPosition ( lPos, pStream );

    UINT lCurLen		=	0;

    SRecordHeader ReadHeader;

    if ( 0 == m_oHeader.RecLen )
        m_bEmtyNode = true;

    while ( lCurLen < m_oHeader.RecLen )
    {
        if (ReadHeader.ReadFromStream(pStream) == false)
            break;

        CRecordTimeVariant* pRecord = NULL;

        TimePropertyID4TimeNode VariableType = ( TimePropertyID4TimeNode ) ReadHeader.RecInstance;

        switch ( VariableType )
        {
        case TL_TPID_Display:			pRecord = new CRecordTimeDisplayType ();		break;
        case TL_TPID_MasterPos:			pRecord = new CRecordTimeMasterRelType ();		break;
        case TL_TPID_SubType:			pRecord = new CRecordTimeSubType ();			break;
        case TL_TPID_EffectID:			pRecord = new CRecordTimeEffectID ();			break;
        case TL_TPID_EffectDir:			pRecord = new CRecordTimeEffectDir ();			break;
        case TL_TPID_EffectType:		pRecord = new CRecordTimeEffectType ();         break;
        case TL_TPID_AfterEffect:		pRecord = new CRecordTimeAfterEffect ();		break;
        case TL_TPID_SlideCount:		pRecord = new CRecordTimeSlideCount ();         break;
        case TL_TPID_TimeFilter:		pRecord = new CRecordTimeNodeTimeFilter ();     break;
        case TL_TPID_EventFilter:		pRecord = new CRecordTimeEventFilter ();		break;
        case TL_TPID_HideWhenStopped:	pRecord = new CRecordTimeHideWhenStopped ();	break;
        case TL_TPID_GroupID:			pRecord = new CRecordTimeGroupID ();			break;
        case TL_TPID_EffectNodeType:	pRecord = new CRecordTimeEffectNodeType ();     break;
        case TL_TPID_PlaceholderNode:	pRecord = new CRecordTimePlaceholderNode ();	break;
        case TL_TPID_MediaVolume:		pRecord = new CRecordTimeMediaVolume ();		break;
        case TL_TPID_MediaMute:			pRecord = new CRecordTimeMediaMute ();			break;
        case TL_TPID_ZoomToFullScreen:	pRecord = new CRecordTimeZoomToFullScreen ();	break;
        default :
            break;
        }
        if (pRecord)
        {
            pRecord->ReadFromStream(ReadHeader, pStream);
            m_arrElements.push_back(pRecord);
        }
        lCurLen += 8 + ReadHeader.RecLen;
    }

    StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
}

bool CRecordTimePropertyList4TimeNodeContainer::IsEmpty()
{
    return m_bEmtyNode;
}

void CRecordTimePropertyList4TimeNodeContainer::ClearNodes()
{
    for ( size_t i = 0; i < m_arrElements.size(); ++i )
    {
        RELEASEOBJECT ( m_arrElements[i] );
    }
    m_arrElements.clear ();
}
