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

#include <fstream>
#include "stream.h"


namespace CFCPP
{
class FStreamWrapper : public IStream, public std::fstream
{
public:
    FStreamWrapper(std::string filename, std::ios_base::openmode openmode) :
        std::fstream(filename, openmode) {}
	
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
	FStreamWrapper(std::wstring filename, std::ios_base::openmode openmode) :
		std::fstream(filename, openmode) {}
#endif
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
	inline bool isError() override {
		return (std::fstream::bad() || std::fstream::fail());
	}
};

}
