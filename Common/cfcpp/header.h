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

#include "sector.h"


namespace CFCPP
{
class Header
{
public:
    Header();
    Header(_UINT16 version);
    void Write(Stream& stream) const;
    void Read(Stream& stream);

private:
    void CheckVersion()const;
    void CheckSignature()const;

public:
    BYTE headerSignature[8] = {0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1};
    BYTE clsid[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    _UINT16 minorVersion = 0x003E;
    _UINT16 majorVersion = 0x0003;
    _UINT16 byteOrder = 0xFFFE;
    _UINT16 sectorShift = 9;
    _UINT16 miniSectorShift = 6;
    BYTE unUsed[6] = {0,0,0,0,0,0};
    _INT32 directorySectorsNumber = 0;
    _INT32 fatSectorsNumber = 0;
    _INT32 firstDirectorySectorID = Sector::ENDOFCHAIN;
    _UINT32 unUsed2 = 0;
    _UINT32 minSizeStandardStream = 4096;
    _INT32 firstMiniFATSectorID = 0xFFFFFFFE;
    _UINT32 miniFATSectorsNumber = 0;
    _INT32 firstDIFATSectorID = Sector::ENDOFCHAIN;
    _UINT32 difatSectorsNumber = 0;
    _INT32 difat[109];
};

}
