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
class CRecordTimeSequenceDataAtom : public CUnknownRecord
{
public:
    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
    {
        m_oHeader = oHeader;

        // LONG lPos = 0;		StreamUtils::StreamPosition ( lPos, pStream );

        m_nConcurrency					=	StreamUtils::ReadDWORD ( pStream );
        m_nNextAction					=	StreamUtils::ReadDWORD ( pStream );
        m_nPreviousAction				=	StreamUtils::ReadDWORD ( pStream );

        StreamUtils::StreamSkip ( sizeof ( _UINT32 ),	pStream );

        _UINT32 Value						=	StreamUtils::ReadDWORD ( pStream );

        m_fConcurrencyPropertyUsed		=	( 0x01 == ( 0x01 & ((BYTE)Value) ) );
        m_fNextActionPropertyUsed		=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
        m_fPreviousActionPropertyUsed	=	( 0x04 == ( 0x04 & ((BYTE)Value) ) );

        // StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
    }

public:

    _UINT32		m_nConcurrency;
    _UINT32		m_nNextAction;
    _UINT32		m_nPreviousAction;

    bool		m_fConcurrencyPropertyUsed;
    bool		m_fNextActionPropertyUsed;
    bool		m_fPreviousActionPropertyUsed;
};
}
