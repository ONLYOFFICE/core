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
#include "../Reader/Records.h"


namespace PPT
{
class CUnknownRoundTrip : public CUnknownRecord
{
public:
    std::pair<boost::shared_array<unsigned char>, _INT32> data;


    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream) override;
    void ReadFromStream(SRecordHeader & oHeader, const XLS::CFStreamPtr &pStream) override;
};

class CUnknownRoundTripID : public CUnknownRecord
{
public:
    UINT m_dwID = -1;


    virtual std::wstring getStrID()const;
    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream) override;
};

// .zip
class RoundTripTheme12Atom : public CUnknownRoundTrip {};
class RoundTripContentMasterInfo12Atom : public CUnknownRoundTrip {};
class RoundTripOArtTextStyles12Atom : public CUnknownRoundTrip {};
class RoundTripNotesMasterTextStyles12Atom : public CUnknownRoundTrip {};
class RoundTripCustomTableStyles12Atom : public CUnknownRoundTrip {};
class RoundTripAnimationAtom12Atom : public CUnknownRoundTrip {};


// .xml
class RoundTripColorMapping12Atom : public CUnknownRecord
{
public:
    std::string m_colorMapping;

    RoundTripColorMapping12Atom(){}
    ~RoundTripColorMapping12Atom(){}

    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream) override;
    std::wstring getPClrMap()const;
};

// ID
class RoundTripContentMasterId12Atom : public CUnknownRecord
{
public:
    UINT m_mainMasterId;
    USHORT m_contentMasterInstanceId;

    RoundTripContentMasterId12Atom() :
        m_mainMasterId(-1), m_contentMasterInstanceId(-1) {}
    ~RoundTripContentMasterId12Atom(){}

    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream) override;
};
class RoundTripShapeId12Atom : public CUnknownRoundTripID {};
class RoundTripCompositeMasterId12Atom : public CUnknownRoundTripID {};
class RoundTripOriginalMainMasterId12Atom : public CUnknownRoundTripID{};
class RoundTripHFPlaceholder12Atom : public CUnknownRoundTripID
{
public:
    PlaceholderEnum m_nPlacementID = PT_None;


    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream) override;
};

class RoundTripHeaderFooterDefaults12Atom : public CUnknownRecord
{
public:
    bool m_fIncludeDate;
    bool m_fIncludeFooter;
    bool m_fIncludeHeader;
    bool m_fIncludeSlideNumber;


    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream) override;
};

class RoundTripDocFlags12Atom : public CUnknownRecord
{
public:
    bool m_fCompressPicturesOnSave;


    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream) override;

};

class RoundTripShapeCheckSumForCL12Atom : public CUnknownRecord
{
public:
    UINT m_shapeCheckSum;
    UINT m_textCheckSum;


    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream) override;
};

class RoundTripNewPlaceholderId12Atom : public CUnknownRecord
{
public:
    PlaceholderEnum m_newPlaceholderId = PT_None;


    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream) override;
};

class RoundTripAnimationHashAtom12Atom : public CUnknownRecord
{
public:
    UINT m_animationChecksum = -1;


    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream) override;
};

class RoundTripSlideSyncInfoAtom12 : public CUnknownRecord
{
public:
    UINT m_dateTimeModified;
    UINT m_dateTimeInserted;


    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream) override;
};

class RoundTripSlideSyncInfo12Container : public CRecordsContainer {};
}
