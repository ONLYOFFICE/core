/*
 * (c) Copyright Ascensio System SIA 2010-2021
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


#include <boost/date_time/posix_time/conversion.hpp>

#include "PCDIDateTime.h"
#include "boost/date_time/posix_time/posix_time_types.hpp"
#include <boost/date_time/posix_time/time_parsers.hpp>

using namespace XLS;

namespace XLSB
{

    PCDIDateTime::PCDIDateTime()
    {
    }

    PCDIDateTime::PCDIDateTime(XLS::CFRecord& record)
    {
        load(record);
    }

    PCDIDateTime::~PCDIDateTime()
    {
    }

    BiffStructurePtr PCDIDateTime::clone()
    {
        return BiffStructurePtr(new PCDIDateTime(*this));
    }

    void PCDIDateTime::load(XLS::CFRecord& record)
    {
        record >> yr >> mon >> dom >> hr >> min >> sec;
    }

	void PCDIDateTime::save(XLS::CFRecord& record)
	{
		record << yr << mon << dom << hr << min << sec;
	}

    std::wstring PCDIDateTime::value()
    {
        if (mon < 1 || mon > 12) mon = 1;
        if (dom < 1 || dom > 31) dom = 1;

        if (yr < 1) yr = 1; //???

        std::wstringstream s;
        s << yr << L"-" << (mon < 10 ? L"0" : L"") << mon << L"-" <<  (dom < 10 ? L"0" : L"") << dom << L"T"
             << (hr < 10 ? L"0" : L"") << hr << L":" << (min < 10 ? L"0" : L"") << min << L":" << (sec < 10 ? L"0" : L"") << sec;

        return s.str();
    }

	void PCDIDateTime::fromString(const std::wstring& str)
    {
		std::string ts(str.begin(), str.end());
		boost::posix_time::ptime pt(boost::posix_time::time_from_string(ts));
		tm pt_tm = boost::posix_time::to_tm(pt);

		yr = pt_tm.tm_year;
		mon = pt_tm.tm_mon;
		dom = pt_tm.tm_mday;
		hr = pt_tm.tm_hour;
		min = pt_tm.tm_min;
		sec = pt_tm.tm_sec;
    }



} // namespace XLSB

