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
    std::vector<CRecordTextMasterStyle9Atom*>       m_rgTextMasterStyleAtom;
    nullable<CRecordTextDefaults9Atom>             m_textDefaultsAtom;
    nullable<CRecordOutlineTextProps9Container>    m_outlineTextPropsContainer;
    nullable<CRecordBlipCollection9Container>      m_blipCollectionContainer;


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

public:
    std::vector<CRecordDocProgBinaryTagSubContainerOrAtom*> m_arrRgChildRec;
};

}
