/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include <string>
#include <boost/cstdint.hpp>

#include "xlsx_cell_format.h"

namespace cpdoccore {

namespace oox {

std::wstring getColAddress(size_t col);
std::wstring getRowAddress(size_t row);

size_t getColAddressInv(const std::wstring & a);
size_t getRowAdderssInv(const std::wstring & a);

std::wstring getCellAddress(size_t col, size_t row);
void getCellAddressInv(const std::wstring & a, size_t & col, size_t & row);

bool parseBoolVal(const std::wstring & str);


std::wstring cellType2Str(XlsxCellType::type type);



bool parseDate(const std::wstring & Date, int & Year, int & Month, int & Day);
bool parseTime(const std::wstring & Time, int & Hours, int & Minutes, double & seconds);
boost::int64_t convertDate(int Year, int Month, int Day);
double convertTime(int hours, int minutes, double sec);


}
}

