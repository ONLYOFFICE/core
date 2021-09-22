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

namespace PPT_FORMAT
{
class CUnknownRoundTrip : public IRecord
{
public:
    std::pair<boost::shared_array<unsigned char>, _INT32> data;

    CUnknownRoundTrip()
    {
    }

    ~CUnknownRoundTrip()
    {
    }

    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
    {
        m_oHeader = oHeader;

        data = std::make_pair(boost::shared_array<unsigned char>(new unsigned char[m_oHeader.RecLen]), m_oHeader.RecLen);
        pStream->read(data.first.get(), data.second);
    }

    void ReadFromStream(SRecordHeader & oHeader, const CFStreamPtr &pStream)
    {
        m_oHeader = oHeader;

        data = std::make_pair(boost::shared_array<unsigned char>(new unsigned char[m_oHeader.RecLen]), m_oHeader.RecLen);
        pStream->read(data.first.get(), data.second);
    }
};

class RoundTripTheme12Atom : public CUnknownRoundTrip {};
class RoundTripColorMapping12Atom : public CUnknownRoundTrip {};
class RoundTripContentMasterInfo12Atom : public CUnknownRoundTrip {};
class RoundTripShapeId12Atom : public CUnknownRoundTrip {};
class RoundTripContentMasterId12Atom : public CUnknownRoundTrip {};
class RoundTripOArtTextStyles12Atom : public CUnknownRoundTrip {};
class RoundTripHeaderFooterDefaults12Atom : public CUnknownRoundTrip {};
class RoundTripDocFlags12Atom : public CUnknownRoundTrip {};
class RoundTripShapeCheckSumForCL12Atom : public CUnknownRoundTrip {};
class RoundTripNotesMasterTextStyles12Atom : public CUnknownRoundTrip {};
class RoundTripCustomTableStyles12Atom : public CUnknownRoundTrip {};
class RoundTripNewPlaceholderId12Atom : public CUnknownRoundTrip {};
class RoundTripAnimationAtom12Atom : public CUnknownRoundTrip {};
class RoundTripAnimationHashAtom12Atom : public CUnknownRoundTrip {};
class RoundTripSlideSyncInfo12 : public CUnknownRoundTrip {};
class RoundTripSlideSyncInfoAtom12 : public CUnknownRoundTrip {};

class RoundTripCompositeMasterId12Atom : public CUnknownRecord
{
public:
    UINT m_dwID;

    RoundTripCompositeMasterId12Atom(): m_dwID(-1){}
    ~RoundTripCompositeMasterId12Atom(){}

    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
    {
        m_oHeader = oHeader;
        m_dwID = StreamUtils::ReadDWORD(pStream);
    }
};

class RoundTripOriginalMainMasterId12Atom : public CUnknownRecord
{
public:
    UINT m_dwID;

    RoundTripOriginalMainMasterId12Atom(): m_dwID(-1){}
    ~RoundTripOriginalMainMasterId12Atom(){}

    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
    {
        m_oHeader = oHeader;
        m_dwID = StreamUtils::ReadDWORD(pStream);
    }
};

class RoundTripHFPlaceholder12Atom : public CUnknownRecord
{
public:
    PlaceholderEnum m_nPlacementID;

    RoundTripHFPlaceholder12Atom(): m_nPlacementID(PT_None){}
    ~RoundTripHFPlaceholder12Atom(){}

    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
    {
        m_oHeader = oHeader;
        m_nPlacementID = (PlaceholderEnum)StreamUtils::ReadBYTE(pStream);
    }
};
}
