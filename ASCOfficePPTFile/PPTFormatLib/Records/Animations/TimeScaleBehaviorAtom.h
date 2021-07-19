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
#pragma once


#include "../Reader/Records.h"


namespace PPT_FORMAT
{
class CRecordTimeScaleBehaviorAtom : public CUnknownRecord
{
public:
    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
    {
        m_oHeader			=	oHeader;

        _UINT32 src						=	StreamUtils::ReadDWORD ( pStream );

        m_fByPropertyUsed				=	( 0x01 == ( 0x01 & ((BYTE)src) ) );
        m_fFromPropertyUsed				=	( 0x02 == ( 0x02 & ((BYTE)src) ) );
        m_fToPropertyUsed				=	( 0x04 == ( 0x04 & ((BYTE)src) ) );
        m_fZoomContentsUsed				=	( 0x08 == ( 0x08 & ((BYTE)src) ) );

        m_XBy							=	StreamUtils::ReadFLOAT ( pStream );
        m_YBy							=	StreamUtils::ReadFLOAT ( pStream );
        m_XFrom							=	StreamUtils::ReadFLOAT ( pStream );
        m_YFrom							=	StreamUtils::ReadFLOAT ( pStream );
        m_XTo							=	StreamUtils::ReadFLOAT ( pStream );
        m_YTo							=	StreamUtils::ReadFLOAT ( pStream );

        src								=	StreamUtils::ReadDWORD ( pStream );
        m_fZoomContents					=	( 0x01 == ( 0x01 & ((BYTE)src) ) );
    }

public:

    bool	m_fByPropertyUsed;
    bool	m_fFromPropertyUsed;
    bool	m_fToPropertyUsed;
    bool	m_fZoomContentsUsed;
    float	m_XBy;
    float	m_YBy;
    float	m_XFrom;
    float	m_YFrom;
    float	m_XTo;
    float	m_YTo;
    bool	m_fZoomContents;
};

}
