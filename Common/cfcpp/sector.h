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

#include <memory>
#include <vector>
#include <mutex>
#include "Stream/stream.h"


namespace CFCPP
{

enum SectorType
{
    Normal,
    Mini,
    FAT,
    DIFAT,
    RangeLockSector,
    Directory
};

class Sector
{
public:
    Sector(_INT32 size, const Stream stream);
    Sector(_INT32 size, const std::vector<BYTE> &data);
    Sector(_INT32 size);

    bool IsStreamed();
    void ZeroData();
    void InitFATData();
    void ReleaseData();

    virtual void Dispose(bool disposing=false);
    std::vector<BYTE> &GetData();

public:
    static _INT32 MINISECTOR_SIZE;
    const static _INT32 FREESECT = 0xFFFFFFFF;
    const static _INT32 ENDOFCHAIN = 0xFFFFFFFE;
    const static _INT32 FATSECT = 0xFFFFFFFD;
    const static _INT32 DIFSECT = 0xFFFFFFFC;

    _INT32 getSize() const;

    SectorType type = Normal;
    bool dirtyFlag = false;
    _INT32 id = -1;

private:
    _INT32 size = 0;
    Stream stream;
    std::vector<BYTE> data;
    std::mutex lockObject;
    bool _disposed = false;
};

}
