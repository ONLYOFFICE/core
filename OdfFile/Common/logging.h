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

#include <iosfwd>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <locale>

namespace cpdoccore
{
    template <class Ostream>
    class logging
    {
    public:
        logging(Ostream &stream, bool bTime) : ostream_(stream), bTime_(bTime)
        {}

        template <class T>
        Ostream & operator << (const T & t)
        {
            if (bTime_)
            {
#if defined(_DEBUG) || defined(_LOGOUT_ALWAYS) 
                std::chrono::system_clock::time_point now{ std::chrono::system_clock::now() };
                std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                std::tm tm = *std::localtime(&now_c);

                auto duration_since_epoch = now.time_since_epoch();
                auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch) % 1000;

                ostream_ << std::put_time(&tm, L"%H:%M:%S") << L"." << milliseconds.count() << L"\t";
#endif
            }
            ostream_ << t;
            return ostream_;
        }

    private:
        Ostream & ostream_;
        bool bTime_ = true;
    };

    extern logging<std::wostream>     logging_cout;
    extern logging<std::wstringstream>logging_log;
   
    extern logging<std::wostream>     logging_cout_continue;
    extern logging<std::wstringstream>logging_log_continue;
}

#if defined( _DEBUG) || defined(_LOGOUT_ALWAYS) 
    #define _CP_LOG cpdoccore::logging_cout
#else
    #define _CP_LOG cpdoccore::logging_log
#endif

#if defined( _DEBUG) || defined(_LOGOUT_ALWAYS) 
#define _CP_LOG_CONTINUE cpdoccore::logging_cout_continue
#else
#define _CP_LOG_CONTINUE cpdoccore::logging_log_continue
#endif