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
#include "TimeNodeAtom.h"

using namespace PPT;

void CRecordTimeNodeAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    //	LONG lPos = 0;	StreamUtils::StreamPosition(lPos, pStream);

    StreamUtils::StreamSkip ( sizeof ( _UINT32 ), pStream );

    //	0x00000000  Does not restart.
    //	0x00000001  Can restart at any time.
    //	0x00000002  Can restart when the corresponding time node is not active.
    //	0x00000003  Same as 0x00000000.
    m_dwRestart				=	StreamUtils::ReadDWORD ( pStream );
    //	0x00000000 specifies that this time node is a parallel time node,
    //	which allows all of its child nodes to start at the same time.
    m_dwType				=	(TimeNodeTypeEnum)StreamUtils::ReadDWORD ( pStream );

    //	0x00000000  The properties remain at their ending values while the parent time node is still running or holding. After which, the properties reset to  their original values.
    //	0x00000001  The properties reset to their original values after the time node becomes inactive.
    //	0x00000002  The properties remain at their ending values while the parent time node is still running or holding, or until another sibling time node is started under a sequential time node as  specified in the type field. After which, the  properties reset to their original values.
    //	0x00000003  Same as 0x00000000.
    //	0x00000004  Same as 0x00000001.
    m_dwFill				=	StreamUtils::ReadDWORD ( pStream );

    StreamUtils::StreamSkip ( sizeof ( _UINT32 ), pStream );
    StreamUtils::StreamSkip ( sizeof ( _UINT32 ),	pStream );

    // duration: 0xFFFFFFFF specifies that the duration of the time node is infinite,
    // and that its actual duration is determined by the durations of its child nodes.
    m_nDuration				=	StreamUtils::ReadLONG ( pStream );

    _UINT32 Value				=	StreamUtils::ReadDWORD ( pStream );

    m_fFillProperty			=	( 0x01 == ( 0x01 & ((BYTE)Value) ) );
    m_fRestartProperty		=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );

    m_fGroupingTypeProperty	=	( 0x08 == ( 0x08 & ((BYTE)Value) ) );
    m_fDurationProperty		=	( 0x10 == ( 0x10 & ((BYTE)Value) ) );

    // StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
}
