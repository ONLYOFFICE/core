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
#include "header.h"
#include "cfexception.h"
#include "streamrw.h"


using namespace CFCPP;

Header::Header() :
    Header(3)
{

}

Header::Header(_UINT16 version)
{
    switch (version)
    {
    case 3:
        majorVersion = 3;
        sectorShift = 0x0009;
        break;

    case 4:
        majorVersion = 4;
        sectorShift = 0x000C;
        break;

    default:
        throw CFException("Invalid Compound File Format version");


    }

    for (_INT32 i = 0; i < 109; i++)
    {
        difat[i] = Sector::FREESECT;
    }
}

void Header::Write(CFCPP::Stream &stream) const
{
    StreamRW rw(stream);
    rw.WriteArray(headerSignature, sizeof(headerSignature));
    rw.WriteArray(clsid, sizeof(clsid));
    rw.Write(minorVersion);
    rw.Write(majorVersion);
    rw.Write(byteOrder);
    rw.Write(sectorShift);
    rw.Write(miniSectorShift);
    rw.WriteArray(unUsed, sizeof(unUsed));
    rw.Write(directorySectorsNumber);
    rw.Write(fatSectorsNumber);
    rw.Write(firstDirectorySectorID);
    rw.Write(unUsed2);
    rw.Write(minSizeStandardStream);
    rw.Write(firstMiniFATSectorID);
    rw.Write(miniFATSectorsNumber);
    rw.Write(firstDIFATSectorID);
    rw.Write(difatSectorsNumber);

    for (_INT32 i : difat)
    {
        rw.Write(i);
    }

    if (majorVersion == 4)
    {
        std::vector<BYTE> zeroHead(3584,0);
        rw.WriteArray(zeroHead.data(), zeroHead.size());
    }
}

void Header::Read(CFCPP::Stream &stream)
{
    StreamRW rw(stream);

    rw.ReadArray(headerSignature, sizeof(headerSignature));
    CheckSignature();
    rw.ReadArray(clsid, sizeof(clsid));
    minorVersion = rw.Read<decltype(minorVersion)>();
    majorVersion = rw.Read<decltype(majorVersion)>();
    CheckVersion();
    byteOrder = rw.Read<decltype(majorVersion)>();
    sectorShift = rw.Read<decltype(majorVersion)>();
    miniSectorShift = rw.Read<decltype(majorVersion)>();
    rw.ReadArray(unUsed, sizeof (unUsed));
    directorySectorsNumber = rw.Read<decltype(directorySectorsNumber)>();
    fatSectorsNumber = rw.Read<decltype(fatSectorsNumber)>();
    firstDirectorySectorID = rw.Read<decltype(firstDirectorySectorID)>();
    unUsed2 = rw.Read<decltype(unUsed2)>();
    minSizeStandardStream = rw.Read<decltype(minSizeStandardStream)>();
    firstMiniFATSectorID = rw.Read<decltype(firstMiniFATSectorID)>();
    miniFATSectorsNumber = rw.Read<decltype(miniFATSectorsNumber)>();
    firstDIFATSectorID = rw.Read<decltype(firstDIFATSectorID)>();
    difatSectorsNumber = rw.Read<decltype(difatSectorsNumber)>();

    for (_INT32 i = 0; i < 109; i++)
    {
        difat[i] = rw.Read<_INT32>();
    }
}

void Header::CheckVersion() const
{
    if (majorVersion != 3 && majorVersion != 4)
        throw CFFileFormatException("Unsupported Binary File Format version: OpenMcdf only supports Compound Files with major version equal to 3 or 4 ");
}

void Header::CheckSignature() const
{
    std::array<BYTE,8> OLE_CFS_SIGNATURE{ 0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1 };
    for (size_t i = 0; i < sizeof(headerSignature); i++)
    {
        if (headerSignature[i] != OLE_CFS_SIGNATURE[i])
            throw CFFileFormatException("Invalid OLE structured storage file");
    }
}
