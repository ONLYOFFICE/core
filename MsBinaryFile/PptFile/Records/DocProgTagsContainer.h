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

#include "CString.h"
#include "TextMasterStyle9Atom.h"
#include "ProgStringTagContainer.h"
#include "TextDefaults9Atom.h"
#include "OutlineTextProps9Container.h"
#include "BlipCollection9Container.h"


#define ___PPT9  L"___PPT9"
#define ___PPT10 L"___PPT10"
#define ___PPT11 L"___PPT11"
#define ___PPT12 L"___PPT12"


namespace PPT
{

class CRecordPP9DocBinaryTagExtension : public CRecordsContainer
{
public:
    std::vector<CRecordTextMasterStyle9Atom*>   m_rgTextMasterStyleAtom;
    nullable<CRecordTextDefaults9Atom>          m_textDefaultsAtom;
    nullable<CRecordOutlineTextProps9Container> m_outlineTextPropsContainer;
    nullable<CRecordBlipCollection9Container>   m_blipCollectionContainer;

    ~CRecordPP9DocBinaryTagExtension();
    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream ) override;
};


class CRecordPP10DocBinaryTagExtension : public CUnknownRecord
{
public:
    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )override;
};

class CRecordPP11DocBinaryTagExtension : public CUnknownRecord
{
public:
    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream );
};


class CRecordPP12DocBinaryTagExtension : public CUnknownRecord
{
public:
    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream ) override;
};


class CRecordDocProgBinaryTagSubContainerOrAtom : public CUnknownRecord
{
public:
    CRecordDocProgBinaryTagSubContainerOrAtom();
    ~CRecordDocProgBinaryTagSubContainerOrAtom();

    void ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream) override;

public:
    CRecordCString* m_pTagName;     // OPTIONAL
    IRecord*        m_pTagContainer;
};

class CRecordDocProgTagsContainer : public CUnknownRecord
{
public:
    CRecordDocProgTagsContainer ();
    ~CRecordDocProgTagsContainer();

    virtual void ReadFromStream (SRecordHeader & oHeader, POLE::Stream* pStream)override;

    CRecordPP9DocBinaryTagExtension* getPP9DocBinaryTagExtension();
    IRecord* getDocBinaryTagExtension(const std::wstring& extVersion);

    std::vector<CRecordDocProgBinaryTagSubContainerOrAtom*> m_arrRgChildRec;
};

}
