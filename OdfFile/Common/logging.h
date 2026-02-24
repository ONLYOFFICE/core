/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include <iosfwd>
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