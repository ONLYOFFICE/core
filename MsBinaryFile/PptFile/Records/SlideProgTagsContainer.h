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
#pragma once

#include "Animations/BuildListContainer.h"
#include "Animations/HashCode10Atom.h"

#include "TextMasterStyleAtom.h"
#include "HeadersFootersAtom.h"
#include "CString.h"

#include "LinkedSlide10Atom.h"
#include "LinkedShape10Atom.h"
#include "SlideFlags10Atom.h"
#include "Comment10Container.h"
#include "SlideTime10Atom.h"
#include "ProgStringTagContainer.h"

#include "TextMasterStyle9Atom.h"

#define TN_PPT9 L"___PPT9"
#define TN_PPT10 L"___PPT10"
#define TN_PPT12 L"___PPT12"


namespace PPT
{
class CRecordPP9SlideBinaryTagExtension : public CUnknownRecord
{
public:
    std::vector<CRecordTextMasterStyle9Atom*> m_rgTextMasterStyleAtom;

    CRecordPP9SlideBinaryTagExtension();

    ~CRecordPP9SlideBinaryTagExtension();

    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream );
};


class CRecordPP10SlideBinaryTagExtension : public CUnknownRecord
{
public:

    CRecordPP10SlideBinaryTagExtension ();

    virtual ~CRecordPP10SlideBinaryTagExtension ();

    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream );

public:
    std::vector<CRecordTextMasterStyleAtom*>    m_arrRgTextMasterStyleAtom;
    std::vector<CRecordComment10Container*>     m_arrRgComment10Container;

    CRecordLinkedSlide10Atom*                   m_pLinkedSlideAtom;         //	OPTIONAL
    std::vector<CRecordLinkedShape10Atom*>      m_arrRgLinkedShape10Atom;   //	OPTIONAL
    CRecordSlideFlags10Atom*                    m_pSlideFlagsAtom;          //	OPTIONAL
    CRecordSlideTime10Atom*                     m_pSlideTimeAtom;           //	OPTIONAL
    CRecordHashCode10Atom*                      m_pHashCode10Atom;          //	OPTIONAL
    CRecordExtTimeNodeContainer*                m_pExtTimeNodeContainer;	//	OPTIONAL
    CRecordBuildListContainer*                  m_pBuildListContainer;		//	OPTIONAL

    bool m_haveLinkedSlide;
    bool m_haveSlideFlags;
    bool m_haveSlideTime;
    bool m_haveHashCode;
    bool m_haveExtTime;
    bool m_haveBuildList;
};

class CRecordPP12SlideBinaryTagExtension : public CUnknownRecord
{
public:
    CRecordPP12SlideBinaryTagExtension();

    ~CRecordPP12SlideBinaryTagExtension();

    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
    {
        m_oHeader			=	oHeader;
        SRecordHeader ReadHeader;

        ReadHeader.ReadFromStream(pStream);
        m_oRoundTripHeaderFooterDefaultsAtom.ReadFromStream(m_oHeader, pStream);
    }

public:
    CRecordRoundTripHeaderFooterDefaults12Atom m_oRoundTripHeaderFooterDefaultsAtom;


};


class CRecordSlideProgBinaryTagSubContainerOrAtom : public CUnknownRecord
{
public:
    CRecordSlideProgBinaryTagSubContainerOrAtom();
    ~CRecordSlideProgBinaryTagSubContainerOrAtom();
    void ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream) override;

public:
    CRecordCString* m_pTagName;     // OPTIONAL
    IRecord*        m_pTagContainer;
};

class CRecordSlideProgTagsContainer : public CUnknownRecord
{
public:

    CRecordSlideProgTagsContainer ();

    ~CRecordSlideProgTagsContainer();


    virtual void ReadFromStream (SRecordHeader & oHeader, POLE::Stream* pStream) override;

    CRecordPP10SlideBinaryTagExtension* getPP10SlideBinaryTagExtension();

public:
    std::vector<CRecordSlideProgBinaryTagSubContainerOrAtom*> m_arrRgChildRec;
};

}
