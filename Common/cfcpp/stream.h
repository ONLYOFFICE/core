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

#include <sstream>
#include <fstream>
#include <memory>
#include <algorithm>
#include <iosfwd>

#include "../DocxFormat/Source/Base/Types_32.h"
#include "../../DesktopEditor/common/Types.h"

namespace CFCPP
{
    class IStream
    {
    public:
        virtual _INT64 tell() = 0;
        virtual _INT64 seek(_INT64 offset, std::ios_base::seekdir mode = std::ios::beg) = 0;
        virtual _INT64 read(char* buffer, _INT64 len) = 0;
        virtual void write (const char* buffer, _INT64 len) = 0;
        virtual void flush() = 0;
        virtual void close() = 0;
    };

    using Stream = std::shared_ptr<IStream>;


    class FStreamWrapper : public IStream, public std::fstream
    {
    public:
        FStreamWrapper(std::string filename, std::ios_base::openmode openmode) :
            std::fstream(filename, openmode) {}

        inline _INT64 tell() override {
            return std::fstream::tellg();
        }
        inline _INT64 seek(_INT64 offset, std::ios_base::seekdir mode = std::ios::beg) override {
            std::fstream::seekp(offset, mode);
            std::fstream::seekg(offset, mode);
            return tell();
        }
        inline _INT64 read(char* buffer, _INT64 len) override {
            std::fstream::read(buffer, len);
            return tell();
        }
        inline void write (const char* buffer, _INT64 len) override {
            std::fstream::write(buffer, len);
        }
        inline void flush() override {
            std::fstream::flush();
        }
        inline void close() override {
            std::fstream::close();
        }
    };

    std::string CorrectUnixPath(const std::string original);

    Stream OpenFileStream(std::wstring filename, bool bRewrite = false, bool trunc = false);
    Stream OpenFileStream(std::string filename, bool bRewrite = false, bool trunc = false);

    bool IsOpen(const Stream& st);
    _INT64 Length(const Stream& st);
    _INT64 FileLenght(std::wstring filename);

    ULONG64 FileFNVHash(std::wstring filename, _INT64 len = -1, _INT64 offset = 0);

}
