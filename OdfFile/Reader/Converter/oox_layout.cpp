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

#include "oox_layout.h"
#include <boost/functional.hpp>

#include <iomanip>
#include <locale>
#include <sstream>

#include <xml/simple_xml_writer.h>
#include "../Format/style_text_properties.h"

namespace cpdoccore {
namespace oox {

namespace
{
	void normalize(_CP_OPT(double) & val, bool bSize)
	{
		if (!val) return;

		if (false == (*val >= 0. && *val <= 1.))
		{
			if (*val > 1.)		val = 1.;
			else if (*val < 0.)	val = 0.;
			else				val.reset();	//NaN
		}
		if (bSize && val && *val <= 0.)
			val.reset();
	}
	std::wstring serialize_value(double val)
	{
		std::wostringstream strm;
		strm.imbue(std::locale::classic());
		strm << std::fixed << std::setprecision(6) << val;

		return strm.str();
	}
	void serialize_value(std::wostream & _Wostream, const wchar_t * name, const _CP_OPT(double) & val)
	{
		if (!val) return;

		CP_XML_WRITER(_Wostream)
		{
			CP_XML_NODE(name)
			{
				CP_XML_ATTR(L"val", serialize_value(*val));
			}
		}
	}
}

void oox_layout::set_rect(const _CP_OPT(double) & rel_x, const _CP_OPT(double) & rel_y,
                          const _CP_OPT(double) & rel_w, const _CP_OPT(double) & rel_h)
{
	x = rel_x;
	y = rel_y;
	w = rel_w;
	h = rel_h;

	normalize(x, false);
	normalize(y, false);
	normalize(w, true);
	normalize(h, true);

	//the box must stay inside the chart area
	if (x && w && *x + *w > 1.) w = 1. - *x;
	if (y && h && *y + *h > 1.) h = 1. - *y;

	normalize(w, true);
	normalize(h, true);
}

void oox_layout::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:layout")
        {
			if (false == empty())
			{
				CP_XML_NODE(L"c:manualLayout")
				{
					if (target)
					{
						CP_XML_NODE(L"c:layoutTarget")
						{
							CP_XML_ATTR(L"val", *target);
						}
					}
					CP_XML_NODE(L"c:xMode")
					{
						CP_XML_ATTR(L"val", L"edge");
					}
					CP_XML_NODE(L"c:yMode")
					{
						CP_XML_ATTR(L"val", L"edge");
					}
					serialize_value(CP_XML_STREAM(), L"c:x", x);
					serialize_value(CP_XML_STREAM(), L"c:y", y);
					serialize_value(CP_XML_STREAM(), L"c:w", w);
					serialize_value(CP_XML_STREAM(), L"c:h", h);
				}
			}
        }
    }

}

}
}
