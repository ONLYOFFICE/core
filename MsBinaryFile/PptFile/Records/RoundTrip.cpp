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
#include "RoundTrip.h"

using namespace PPT;

//static int nRTCounter = 1;
void CUnknownRoundTrip::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    data = std::make_pair(boost::shared_array<unsigned char>(new unsigned char[m_oHeader.RecLen]), m_oHeader.RecLen);
    pStream->read(data.first.get(), data.second);

//        std::string filename = std::to_string(nRTCounter++) + "_" + GetRecordName(m_oHeader.RecType) + ".zip";
//        std::ofstream file("RoundTrips/" + filename, std::ios::out);
//        file.write((char*)data.first.get(), data.second);
    //        file.close();
}

void CUnknownRoundTrip::ReadFromStream(SRecordHeader &oHeader, const XLS::CFStreamPtr &pStream)
{
    m_oHeader = oHeader;

    data = std::make_pair(boost::shared_array<unsigned char>(new unsigned char[m_oHeader.RecLen]), m_oHeader.RecLen);
    pStream->read(data.first.get(), data.second);
}

std::wstring CUnknownRoundTripID::getStrID() const
{
    return std::to_wstring(m_dwID);
}

void CUnknownRoundTripID::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    m_dwID = StreamUtils::ReadDWORD(pStream);

//        std::string filename = std::to_string(nRTCounter++) + "_" + GetRecordName(m_oHeader.RecType) + "_ID_" + std::to_string(m_dwID) + ".txt";
//        std::ofstream file("RoundTrips/" + filename, std::ios::out);
//        file << m_dwID;
//        file.close();
}

void RoundTripColorMapping12Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    m_colorMapping = StreamUtils::ReadStringA(pStream, m_oHeader.RecLen);

//        std::string filename = std::to_string(nRTCounter++) + "_" + GetRecordName(m_oHeader.RecType) + ".xml";
//        std::ofstream file("RoundTrips/" + filename, std::ios::out);
//        file << m_colorMapping;
    //        file.close();
}

std::wstring RoundTripColorMapping12Atom::getPClrMap() const
{
    auto iter = m_colorMapping.find("<a:clrMap");
    if (iter == (UINT)-1)
        return std::wstring(L"");

    auto subStr = m_colorMapping.substr(iter);
    subStr[1] = 'p';

    return std::wstring(subStr.begin(), subStr.end());
}

void RoundTripContentMasterId12Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_mainMasterId = StreamUtils::ReadDWORD(pStream);
    m_contentMasterInstanceId = StreamUtils::ReadWORD(pStream);

    StreamUtils::StreamSkip(2, pStream);

//        std::string filename = std::to_string(nRTCounter++) + "_" + GetRecordName(m_oHeader.RecType) +
//                "_" + std::to_string(m_mainMasterId) + "_" + std::to_string(m_contentMasterInstanceId) + ".txt";
//        std::ofstream file("RoundTrips/" + filename, std::ios::out);
//        file << m_mainMasterId << "\t" << m_contentMasterInstanceId;
//        file.close();
}

void RoundTripHFPlaceholder12Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    m_nPlacementID = (PlaceholderEnum)StreamUtils::ReadBYTE(pStream);
}

void RoundTripHeaderFooterDefaults12Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    BYTE flags = StreamUtils::ReadBYTE(pStream);

    m_fIncludeDate          = flags & 0x01;
    m_fIncludeFooter        = flags & 0x02;
    m_fIncludeHeader        = flags & 0x04;
    m_fIncludeSlideNumber   = flags & 0x08;
}

void RoundTripDocFlags12Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_fCompressPicturesOnSave = StreamUtils::ReadBYTE(pStream) & 0x01;
}

void RoundTripShapeCheckSumForCL12Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_shapeCheckSum = StreamUtils::ReadDWORD(pStream);
    m_textCheckSum  = StreamUtils::ReadDWORD(pStream);
}

void RoundTripNewPlaceholderId12Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    m_newPlaceholderId = (PlaceholderEnum)StreamUtils::ReadBYTE(pStream);
}

void RoundTripAnimationHashAtom12Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    m_animationChecksum = StreamUtils::ReadDWORD(pStream);

}

void RoundTripSlideSyncInfoAtom12::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_dateTimeModified = StreamUtils::ReadDWORD(pStream);
    m_dateTimeInserted  = StreamUtils::ReadDWORD(pStream);
}
