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
