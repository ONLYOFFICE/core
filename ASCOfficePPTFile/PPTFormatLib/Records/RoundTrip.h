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

#include <fstream>
static UINT nRTCounter = 1;

namespace PPT_FORMAT
{
class CUnknownRoundTrip : public CUnknownRecord
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

        std::string filename = std::to_string(nRTCounter++) + "_" + GetRecordName(m_oHeader.RecType) + ".zip";
        std::ofstream file("RoundTrips/" + filename, std::ios::out);
        file.write((char*)data.first.get(), data.second);
        file.close();
    }

    void ReadFromStream(SRecordHeader & oHeader, const CFStreamPtr &pStream)
    {
        m_oHeader = oHeader;

        data = std::make_pair(boost::shared_array<unsigned char>(new unsigned char[m_oHeader.RecLen]), m_oHeader.RecLen);
        pStream->read(data.first.get(), data.second);
    }
};

class CUnknownRoundTripID : public CUnknownRecord
{
public:
    UINT m_dwID;

    CUnknownRoundTripID(): m_dwID(-1){}
    ~CUnknownRoundTripID(){}

    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
    {
        m_oHeader = oHeader;
        m_dwID = StreamUtils::ReadDWORD(pStream);

        std::string filename = std::to_string(nRTCounter++) + "_" + GetRecordName(m_oHeader.RecType) + "_ID_" + std::to_string(m_dwID) + ".txt";
        std::ofstream file("RoundTrips/" + filename, std::ios::out);
        file << m_dwID;
        file.close();
    }
};

// .zip
class RoundTripTheme12Atom : public CUnknownRoundTrip {};
class RoundTripContentMasterInfo12Atom : public CUnknownRoundTrip {};
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

// .xml
class RoundTripColorMapping12Atom : public CUnknownRecord
{
public:
    std::string m_colorMapping;

    RoundTripColorMapping12Atom(){}
    ~RoundTripColorMapping12Atom(){}

    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
    {
        m_oHeader = oHeader;
        m_colorMapping = StreamUtils::ReadStringA(pStream, m_oHeader.RecLen);

        std::string filename = std::to_string(nRTCounter++) + "_" + GetRecordName(m_oHeader.RecType) + ".xml";
        std::ofstream file("RoundTrips/" + filename, std::ios::out);
        file << m_colorMapping;
        file.close();
    }
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

    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
    {
        m_oHeader = oHeader;

        m_mainMasterId = StreamUtils::ReadDWORD(pStream);
        m_contentMasterInstanceId = StreamUtils::ReadWORD(pStream);

        StreamUtils::StreamSkip(2, pStream);

        std::string filename = std::to_string(nRTCounter++) + "_" + GetRecordName(m_oHeader.RecType) +
                "_" + std::to_string(m_mainMasterId) + "_" + std::to_string(m_contentMasterInstanceId) + ".txt";
        std::ofstream file("RoundTrips/" + filename, std::ios::out);
        file << m_mainMasterId << "\t" << m_contentMasterInstanceId;
        file.close();
    }
};
class RoundTripShapeId12Atom : public CUnknownRoundTripID {};
class RoundTripCompositeMasterId12Atom : public CUnknownRoundTripID {};
class RoundTripOriginalMainMasterId12Atom : public CUnknownRoundTripID{};
class RoundTripHFPlaceholder12Atom : public CUnknownRoundTripID
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
