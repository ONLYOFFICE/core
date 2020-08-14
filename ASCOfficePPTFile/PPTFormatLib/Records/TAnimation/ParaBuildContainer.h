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


#include "../IRecord.h"


struct ParaBuildContainer : public CUnknownRecord
{
public:

    ParaBuildContainer()
    {

    }

    virtual ~ParaBuildContainer()
    {
        for ( size_t i = 0; i < rgParaBuildLevel.size(); ++i )
            RELEASEOBJECT (rgParaBuildLevel[i]);
    }

    virtual void ReadFromStream ( SRecordHeader & thisHeader, POLE::Stream* pStream )
    {
        m_oHeader			=	thisHeader;

        SRecordHeader oHeader;
        UINT res = 0;

        if (oHeader.ReadFromStream(pStream))
            buildAtom.ReadFromStream ( oHeader, pStream );

        if (oHeader.ReadFromStream(pStream))
            paraBuildAtom.ReadFromStream ( oHeader, pStream );

        UINT lCurLen	=	0;

        while ( lCurLen < m_oHeader.RecLen )
        {
            SRecordHeader ReadHeader;
            if (ReadHeader.ReadFromStream(pStream) == false)
                break;

            lCurLen			+=	8 + ReadHeader.RecLen;

            WORD nRecord	=	ReadHeader.RecType;

            if ( RT_LevelInfoAtom	== nRecord )
            {
                ParaBuildLevel* pLevel = new ParaBuildLevel ();
                if (pLevel)
                {
                    pLevel->levelInfoAtom.ReadFromStream  ( ReadHeader, pStream );

                    if (ReadHeader.ReadFromStream(pStream) )
                    {
                        pLevel->timeNode	=	CAnimationFactory::BuildAnimationObject ( ReadHeader.RecType );
                        if (pLevel->timeNode)
                        {
                            pLevel->timeNode->ReadFromStream  ( ReadHeader, pStream );
                            rgParaBuildLevel.push_back ( pLevel );

                            continue;
                        }
                    }
                }
            }
            StreamUtils::StreamSkip ( ReadHeader.RecLen, pStream );
        }
    }
public:
    BuildAtom		buildAtom;
    ParaBuildAtom	paraBuildAtom;

    std::vector <ParaBuildLevel*>	rgParaBuildLevel;
};
