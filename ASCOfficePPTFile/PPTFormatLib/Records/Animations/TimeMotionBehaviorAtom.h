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
class CRecordTimeMotionBehaviorAtom : public CUnknownRecord
{
public:
    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
    {
        m_oHeader = oHeader;

        _UINT32 Value						=	StreamUtils::ReadDWORD ( pStream );

        m_bByPropertyUsed				=	( 0x01 == ( 0x01 & ((BYTE)Value) ) );
        m_bFromPropertyUsed				=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
        m_bToPropertyUsed				=	( 0x04 == ( 0x04 & ((BYTE)Value) ) );
        m_bOriginPropertyUsed			=	( 0x08 == ( 0x08 & ((BYTE)Value) ) );
        m_bPathPropertyUsed				=	( 0x10 == ( 0x10 & ((BYTE)Value) ) );

        m_bEditRotationPropertyUsed		=	( 0x40 == ( 0x40 & ((BYTE)Value) ) );
        m_bPointsTypesPropertyUsed		=	( 0x80 == ( 0x80 & ((BYTE)Value) ) );

        m_nXBY							=	StreamUtils::ReadFLOAT ( pStream );
        m_nYBY							=	StreamUtils::ReadFLOAT ( pStream );
        m_nXFROM						=	StreamUtils::ReadFLOAT ( pStream );
        m_nYFROM						=	StreamUtils::ReadFLOAT ( pStream );
        m_nXTO							=	StreamUtils::ReadFLOAT ( pStream );
        m_nYTO							=	StreamUtils::ReadFLOAT ( pStream );
        m_nBehaviorOrigin				=	StreamUtils::ReadDWORD ( pStream );
    }

public:

    bool	m_bByPropertyUsed;
    bool	m_bFromPropertyUsed;
    bool	m_bToPropertyUsed;
    bool	m_bOriginPropertyUsed;
    bool	m_bPathPropertyUsed;
    bool	m_bEditRotationPropertyUsed;
    bool	m_bPointsTypesPropertyUsed;

    FLOAT	m_nXBY;
    FLOAT	m_nYBY;
    FLOAT	m_nXFROM;
    FLOAT	m_nYFROM;
    FLOAT	m_nXTO;
    FLOAT	m_nYTO;
    _UINT32	m_nBehaviorOrigin;
};

}
