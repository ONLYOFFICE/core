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
#include "SSSlideInfoAtom.h"

using namespace PPT;

CSlideShowSlideInfoAtom::CSlideShowSlideInfoAtom()
{
    m_nSlideTime	= -1;
    m_nSoundRef		= 0xFFFFFFFE;

    m_nEffectDirection	= 0;
    m_nEffectType		= 0;

    m_bManualAdvance	= false;
    m_bHidden			= false;
    m_bSound			= false;
    m_bLoopSound		= false;
    m_bStopSound		= false;
    m_bAutoAdvance		= false;
    m_bCursorVisible	= false;

    m_nSpeed			= 1;
}

CSlideShowSlideInfoAtom::~CSlideShowSlideInfoAtom()
{
}

void CSlideShowSlideInfoAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nSlideTime		=	(INT)StreamUtils::ReadDWORD ( pStream );
    m_nSoundRef		=	StreamUtils::ReadDWORD ( pStream );

    m_nEffectDirection	=	StreamUtils::ReadBYTE ( pStream );
    m_nEffectType		=	StreamUtils::ReadBYTE ( pStream );

    BYTE bValue			=	StreamUtils::ReadBYTE ( pStream );

    m_bManualAdvance	=	(0x01 == (0x01 & bValue));
    m_bHidden		=	(0x04 == (0x04 & bValue));
    m_bSound		=	(0x10 == (0x10 & bValue));
    m_bLoopSound		=	(0x40 == (0x40 & bValue));

    bValue			=	StreamUtils::ReadBYTE ( pStream );

    m_bStopSound		=	(0x01 == (0x01 & bValue));
    m_bAutoAdvance		=	(0x04 == (0x04 & bValue));
    m_bCursorVisible	=	(0x10 == (0x10 & bValue));

    //_UINT32 dwValue		=	StreamUtils::ReadDWORD ( pStream );
    //m_nSpeed			=	( 0x01 == ( 0x01 & ((BYTE)dwValue) ) );
    m_nSpeed			=	StreamUtils::ReadBYTE( pStream );
    StreamUtils::StreamSkip(3, pStream);
}

double CSlideShowSlideInfoAtom::GetTimeTransition()
{
    double dTime = 500.0;
    if (0 == m_nSpeed)
    {
        dTime = 750.0;
    }
    else if (2 == m_nSpeed)
    {
        dTime = 250.0;
    }

    return dTime;
}

double CSlideShowSlideInfoAtom::GetTimeSlide()
{
    return (double)m_nSlideTime;
}
