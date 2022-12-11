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

        pRecord->ReadFromStream  ( ReadHeader, pStream );
        lCurLen += 8 + ReadHeader.RecLen;

        m_arrElements.push_back ( pRecord );
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
