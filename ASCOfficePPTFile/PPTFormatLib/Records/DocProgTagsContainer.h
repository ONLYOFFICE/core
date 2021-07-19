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

#include "CString.h"
#include "TextMasterStyle9Atom.h"
#include "ProgStringTagContainer.h"
#include "TextDefaults9Atom.h"
#include "OutlineTextProps9Container.h"


#define ___PPT9  L"___PPT9"
#define ___PPT10 L"___PPT10"
#define ___PPT11 L"___PPT11"
#define ___PPT12 L"___PPT12"


namespace PPT_FORMAT
{

class CRecordPP9DocBinaryTagExtension : public CUnknownRecord
{
public:
    std::vector<CRecordTextMasterStyle9Atom*>       m_rgTextMasterStyleAtom;
     nullable<CRecordTextDefaults9Atom>             m_textDefaultsAtom;
     nullable<CRecordOutlineTextProps9Container>    m_outlineTextPropsContainer;
    // TODO
    CRecordPP9DocBinaryTagExtension()
    {

    }

    ~CRecordPP9DocBinaryTagExtension()
    {
        for (auto pEl : m_rgTextMasterStyleAtom)
        {
            RELEASEOBJECT(pEl)
        }
    }

    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
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
            switch (ReadHeader.RecType)
            {
            case RT_TextMasterStyle9Atom:
            {
                auto pStyle = new CRecordTextMasterStyle9Atom;
                pStyle->ReadFromStream(ReadHeader, pStream);
                m_rgTextMasterStyleAtom.push_back(pStyle);
                break;
            }
            case RT_TextDefaults9Atom:
            {
                m_textDefaultsAtom = new CRecordTextDefaults9Atom;
                m_textDefaultsAtom->ReadFromStream(ReadHeader, pStream);
                break;
            }
            case RT_OutlineTextProps9:
            {
                m_outlineTextPropsContainer = new CRecordOutlineTextProps9Container;
                m_outlineTextPropsContainer->ReadFromStream(ReadHeader, pStream);
                break;
            }
            default:
                StreamUtils::StreamSkip(ReadHeader.RecLen, pStream);
                break;
            }

        }

        StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
    }
};


class CRecordPP10DocBinaryTagExtension : public CUnknownRecord
{
public:

    CRecordPP10DocBinaryTagExtension ()
    {
    }

    virtual ~CRecordPP10DocBinaryTagExtension ()
    {

    }

    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
    {
        m_oHeader			=	oHeader;

        LONG lPos			=	0;
        StreamUtils::StreamPosition ( lPos, pStream );

        StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
    }

public:

};

class CRecordPP11DocBinaryTagExtension : public CUnknownRecord
{
public:
    CRecordPP11DocBinaryTagExtension()
    {

    }

    ~CRecordPP11DocBinaryTagExtension()
    {

    }

    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
    {
        m_oHeader			=	oHeader;
        LONG lPos			=	0;
        StreamUtils::StreamPosition ( lPos, pStream );

        StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
    }

public:

};


class CRecordPP12DocBinaryTagExtension : public CUnknownRecord
{
public:

    CRecordPP12DocBinaryTagExtension()
    {

    }

    ~CRecordPP12DocBinaryTagExtension()
    {
    }

    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
    {
        m_oHeader			=	oHeader;
        LONG lPos			=	0;
        StreamUtils::StreamPosition ( lPos, pStream );

        StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
    }
};


class CRecordDocProgBinaryTagSubContainerOrAtom : public CUnknownRecord
{
public:
    CRecordDocProgBinaryTagSubContainerOrAtom() :
        m_pTagName(nullptr), m_pTagContainer(nullptr)
    {}
    ~CRecordDocProgBinaryTagSubContainerOrAtom()
    {
        RELEASEOBJECT(m_pTagName);
        RELEASEOBJECT(m_pTagContainer);
    }
    void ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream) override
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

            if (m_pTagName->m_strText == ___PPT9) {
                m_pTagContainer = new CRecordPP9DocBinaryTagExtension();
                dynamic_cast<CRecordPP9DocBinaryTagExtension*>
                        (m_pTagContainer)->ReadFromStream(childHeader, pStream);
            } else if (m_pTagName->m_strText == ___PPT10) {
                m_pTagContainer = new CRecordPP10DocBinaryTagExtension();
                dynamic_cast<CRecordPP10DocBinaryTagExtension*>
                        (m_pTagContainer)->ReadFromStream(childHeader, pStream);
            } else if (m_pTagName->m_strText == ___PPT11) {
                m_pTagContainer = new CRecordPP11DocBinaryTagExtension();
                dynamic_cast<CRecordPP11DocBinaryTagExtension*>
                        (m_pTagContainer)->ReadFromStream(childHeader, pStream);
            } else if (m_pTagName->m_strText == ___PPT12) {
            m_pTagContainer = new CRecordPP12DocBinaryTagExtension();
            dynamic_cast<CRecordPP12DocBinaryTagExtension*>
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

public:
    CRecordCString* m_pTagName;     // OPTIONAL
    IRecord*        m_pTagContainer;
};

class CRecordDocProgTagsContainer : public CUnknownRecord
{
public:

    CRecordDocProgTagsContainer ()
    {

    }

    ~CRecordDocProgTagsContainer()
    {
        for (auto pEl : m_arrRgChildRec)
            RELEASEOBJECT(pEl)
    }


    virtual void ReadFromStream (SRecordHeader & oHeader, POLE::Stream* pStream)
    {
        m_oHeader	=	oHeader;

        LONG lPos = 0;
        StreamUtils::StreamPosition ( lPos, pStream );

        LONG lCurLen(0);
        SRecordHeader ReadHeader;

        while (lCurLen < m_oHeader.RecLen)
        {
            if ( ReadHeader.ReadFromStream(pStream) == false)
                break;
            lCurLen +=	8 + ReadHeader.RecLen;
            CRecordDocProgBinaryTagSubContainerOrAtom* pRecord =
                    new CRecordDocProgBinaryTagSubContainerOrAtom();
            pRecord->ReadFromStream(ReadHeader, pStream);
            m_arrRgChildRec.push_back(pRecord);
        }

        StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
    }

    CRecordPP9DocBinaryTagExtension* getPP19DocBinaryTagExtension()
    {
        for (auto* rec : m_arrRgChildRec)
            if (rec->m_pTagName->m_strText == ___PPT9)
                return (CRecordPP9DocBinaryTagExtension*)rec->m_pTagContainer;

        return nullptr;
    }

public:
    std::vector<CRecordDocProgBinaryTagSubContainerOrAtom*> m_arrRgChildRec;
};

}
