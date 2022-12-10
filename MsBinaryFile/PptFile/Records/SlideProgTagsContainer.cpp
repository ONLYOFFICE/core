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
#include "SlideProgTagsContainer.h"

using namespace PPT;

CRecordPP9SlideBinaryTagExtension::CRecordPP9SlideBinaryTagExtension()
{

}

CRecordPP9SlideBinaryTagExtension::~CRecordPP9SlideBinaryTagExtension()
{
    for (auto pEl : m_rgTextMasterStyleAtom)
    {
        RELEASEOBJECT(pEl)
    }
}

void CRecordPP9SlideBinaryTagExtension::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;
    LONG lPos			=	0;
    StreamUtils::StreamPosition ( lPos, pStream );

    UINT lCurLen		=	0;

    SRecordHeader ReadHeader;

    while ( lCurLen < m_oHeader.RecLen )
    {
        if ( ReadHeader.ReadFromStream(pStream) == false)
            break;

        lCurLen += 8 + ReadHeader.RecLen;
        auto pStyle = new CRecordTextMasterStyle9Atom;
        pStyle->ReadFromStream(ReadHeader, pStream);
        m_rgTextMasterStyleAtom.push_back(pStyle);
    }

    StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
}

CRecordPP10SlideBinaryTagExtension::CRecordPP10SlideBinaryTagExtension() :
    m_pLinkedSlideAtom(nullptr),
    m_pSlideFlagsAtom(nullptr),
    m_pSlideTimeAtom(nullptr),
    m_pHashCode10Atom(nullptr),
    m_pExtTimeNodeContainer(nullptr),
    m_pBuildListContainer(nullptr),

    m_haveLinkedSlide(false),
    m_haveSlideFlags(false),
    m_haveSlideTime(false),
    m_haveHashCode(false),
    m_haveExtTime(false),
    m_haveBuildList(false)
{
}

CRecordPP10SlideBinaryTagExtension::~CRecordPP10SlideBinaryTagExtension()
{
    RELEASEOBJECT (m_pLinkedSlideAtom)
            RELEASEOBJECT (m_pSlideFlagsAtom)
            RELEASEOBJECT (m_pSlideTimeAtom)
            RELEASEOBJECT (m_pHashCode10Atom)
            RELEASEOBJECT (m_pExtTimeNodeContainer)
            RELEASEOBJECT (m_pBuildListContainer)

            for (auto pEl : m_arrRgTextMasterStyleAtom)
            RELEASEOBJECT(pEl)
            for (auto pEl : m_arrRgComment10Container)
            RELEASEOBJECT(pEl)
            for (auto pEl : m_arrRgLinkedShape10Atom)
            RELEASEOBJECT(pEl)
}

void PPT::CRecordPP10SlideBinaryTagExtension::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    LONG lPos			=	0;
    StreamUtils::StreamPosition ( lPos, pStream );

    UINT lCurLen		=	0;

    SRecordHeader ReadHeader;

    while ( lCurLen < m_oHeader.RecLen )
    {
        if ( ReadHeader.ReadFromStream(pStream) == false)
            break;

        lCurLen += 8 + ReadHeader.RecLen;

        switch (ReadHeader.RecType) {
        case RT_TextMasterStyle10Atom:
        {
            CRecordTextMasterStyleAtom* pRecord =
                    new CRecordTextMasterStyleAtom();
            pRecord->ReadFromStream(ReadHeader, pStream);
            m_arrRgTextMasterStyleAtom.push_back(pRecord);
            break;
        }

        case RT_Comment10:
        {
            CRecordComment10Container* pRecord =
                    new CRecordComment10Container();
            pRecord->ReadFromStream(ReadHeader, pStream);
            m_arrRgComment10Container.push_back(pRecord);
            break;
        }

        case RT_LinkedSlide10Atom :
        {
            m_pLinkedSlideAtom = new CRecordLinkedSlide10Atom();
            m_pLinkedSlideAtom->ReadFromStream(ReadHeader, pStream);
            m_haveLinkedSlide = true;
            break;
        }

        case RT_LinkedShape10Atom :
        {
            CRecordLinkedShape10Atom* pRecord =
                    new CRecordLinkedShape10Atom();
            pRecord->ReadFromStream(ReadHeader, pStream);
            m_arrRgLinkedShape10Atom.push_back(pRecord);
            break;
        }

        case RT_SlideFlags10Atom:
        {
            m_pSlideFlagsAtom = new CRecordSlideFlags10Atom();
            m_pSlideFlagsAtom->ReadFromStream(ReadHeader, pStream);
            m_haveSlideFlags = true;
            break;
        }

        case RT_SlideTime10Atom:
        {
            m_pSlideTimeAtom = new CRecordSlideTime10Atom();
            m_pSlideTimeAtom->ReadFromStream(ReadHeader, pStream);
            m_haveSlideTime = true;
            break;
        }

        case RT_HashCodeAtom:
        {
            m_pHashCode10Atom = new CRecordHashCode10Atom();
            m_pHashCode10Atom->ReadFromStream(ReadHeader, pStream);
            m_haveHashCode = true;
            break;
        }

        case RT_TimeExtTimeNodeContainer:
        {
            m_pExtTimeNodeContainer = new CRecordExtTimeNodeContainer();
            m_pExtTimeNodeContainer->ReadFromStream(ReadHeader, pStream);
            m_haveExtTime = true;
            break;
        }

        case RT_BuildList:
        {
            m_pBuildListContainer = new CRecordBuildListContainer();
            m_pBuildListContainer->ReadFromStream(ReadHeader, pStream);
            m_haveBuildList = true;
            break;
        }

        default:
            break;
        }
    }

    StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
}

CRecordPP12SlideBinaryTagExtension::CRecordPP12SlideBinaryTagExtension()
{

}

CRecordPP12SlideBinaryTagExtension::~CRecordPP12SlideBinaryTagExtension()
{

}

CRecordSlideProgBinaryTagSubContainerOrAtom::CRecordSlideProgBinaryTagSubContainerOrAtom() :
    m_pTagName(nullptr), m_pTagContainer(nullptr)
{}

CRecordSlideProgBinaryTagSubContainerOrAtom::~CRecordSlideProgBinaryTagSubContainerOrAtom()
{
    RELEASEOBJECT(m_pTagName);
    RELEASEOBJECT(m_pTagContainer);
}

void CRecordSlideProgBinaryTagSubContainerOrAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    LONG lPos = 0; StreamUtils::StreamPosition(lPos, pStream);

    SRecordHeader ReadHeader;
    ReadHeader.ReadFromStream(pStream);

    if (m_oHeader.RecType == RT_ProgBinaryTag)
    {
        m_pTagName = new CRecordCString();
        m_pTagName->ReadFromStream(ReadHeader, pStream);

        SRecordHeader childHeader;
        if (!childHeader.ReadFromStream(pStream))
            return;

        if (m_pTagName->m_strText == TN_PPT9) {
            m_pTagContainer = new CRecordPP9SlideBinaryTagExtension();
            dynamic_cast<CRecordPP9SlideBinaryTagExtension*>
                    (m_pTagContainer)->ReadFromStream(childHeader, pStream);
        } else if (m_pTagName->m_strText == TN_PPT10) {
            m_pTagContainer = new CRecordPP10SlideBinaryTagExtension();
            dynamic_cast<CRecordPP10SlideBinaryTagExtension*>
                    (m_pTagContainer)->ReadFromStream(childHeader, pStream);
        } else if (m_pTagName->m_strText == TN_PPT12) {
            m_pTagContainer = new CRecordPP12SlideBinaryTagExtension();
            dynamic_cast<CRecordPP12SlideBinaryTagExtension*>
                    (m_pTagContainer)->ReadFromStream(childHeader, pStream);
        }

    } else if (m_oHeader.RecType == RT_ProgStringTag)
    {
        m_pTagContainer = new CRecordProgStringTagContainer();
        dynamic_cast<CRecordProgStringTagContainer*>
                (m_pTagContainer)->ReadFromStream(ReadHeader, pStream);
    }
    LONG lSeek = lPos + m_oHeader.RecLen;
    StreamUtils::StreamSeek(lSeek, pStream);
}

CRecordSlideProgTagsContainer::CRecordSlideProgTagsContainer()
{

}

CRecordSlideProgTagsContainer::~CRecordSlideProgTagsContainer()
{
    for (auto pEl : m_arrRgChildRec)
        RELEASEOBJECT(pEl)
}

void PPT::CRecordSlideProgTagsContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader	=	oHeader;

    LONG lPos = 0;
    StreamUtils::StreamPosition ( lPos, pStream );

    LONG lCurLen(0);
    SRecordHeader ReadHeader;

    while (lCurLen < (LONG)m_oHeader.RecLen)
    {
        if ( ReadHeader.ReadFromStream(pStream) == false)
            break;
        lCurLen +=	8 + ReadHeader.RecLen;
        CRecordSlideProgBinaryTagSubContainerOrAtom* pRecord =
                new CRecordSlideProgBinaryTagSubContainerOrAtom();
        pRecord->ReadFromStream(ReadHeader, pStream);
        m_arrRgChildRec.push_back(pRecord);
    }

    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}

CRecordPP10SlideBinaryTagExtension *CRecordSlideProgTagsContainer::getPP10SlideBinaryTagExtension()
{
    for (auto* rec : m_arrRgChildRec)
        if (rec->m_pTagName && rec->m_pTagName->m_strText == TN_PPT10)
            return (CRecordPP10SlideBinaryTagExtension*)rec->m_pTagContainer;

    return nullptr;
}
