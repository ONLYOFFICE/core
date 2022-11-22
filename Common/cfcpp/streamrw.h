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

#include <fstream>
#include <array>
#include <vector>
#include "Stream/stream.h"


namespace CFCPP
{
class StreamRW
{
public:
    StreamRW(Stream stream);


    _INT64 Seek(_INT64 offset);
    _INT64 Tell();

    template <class T>
    T Read()
    {
        T value;
        char* asByteArr = reinterpret_cast<char*>(&value);
        stream->read(asByteArr, sizeof (T));
        return value;
    }

    template<class T>
    void Write(T value)
    {
        char* asByteArr = reinterpret_cast<char*>(&value);
        stream->write(asByteArr, sizeof (T));
    }

    void ReadArray(char* data, _INT32 lenght);
    void ReadArray(BYTE* data, _INT32 lenght);
    void WriteArray(const BYTE *arr, _INT32 lenght);
    void WriteArray(const char *arr, _INT32 lenght);

    inline void Close(){return;}

private:
    Stream stream;
};

}
