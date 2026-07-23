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
#include "codecvt.h"
#include <algorithm>

using namespace std;


ucs2_conversion::result
ucs2_conversion::do_in(mbstate_t&,
					   const char*  from, const char* from_end, const char*& from_next,
					   wchar_t* to, wchar_t* to_limit, wchar_t*& to_next) const
{
	const int max_input = (from_end - from) & ~1;
	const int max_output = (to_limit - to);
	int count = std::min(max_input / 2, max_output); // TODO try to optimize by replacing division with shift

	from_next = from;
	to_next = to;

	for (;count--; from_next += 2, ++to_next)
	{
		unsigned char c1 = *from_next;
		unsigned char c2 = *(from_next + 1);
		*to_next = c1 | c2 << 8;
	}

	if (to_next == to && from_next == from_end - 1)
		return partial;
	return ok;
}


ucs2_conversion::result
ucs2_conversion::do_out(mbstate_t&,
						const wchar_t* from, const wchar_t* from_end, const wchar_t*& from_next,
						char* to, char* to_limit, char*& to_next) const
{
	const int max_input = (from_end - from);
	const int max_output = (to_limit - to) & ~1;
	int count = std::min(max_input, max_output / 2);	// TODO try to optimize by replacing division with shift

	from_next = from;
	to_next = to;

	for (;count--; ++from_next, to_next += 2)
	{
		*(to_next + 0) = *from_next & 0xFF;
		*(to_next + 1) = *from_next >> 8 & 0xFF;
	}
	return ok;
}



ube_conversion::result
ube_conversion::do_in(mbstate_t&,
					  const char*  from, const char* from_end, const char*& from_next,
					  wchar_t* to, wchar_t* to_limit, wchar_t*& to_next) const
{
	const int max_input = (from_end - from) & ~1;
	const int max_output = (to_limit - to);
	int count = std::min(max_input / 2, max_output); // TODO try to optimize by replacing division with shift

	from_next = from;
	to_next = to;

	for (;count--; from_next += 2, ++to_next)
	{
		unsigned char c1 = *from_next;
		unsigned char c2 = *(from_next + 1);
		*to_next = c2 | c1 << 8;
	}

	if (to_next == to && from_next == from_end - 1)
		return partial;
	return ok;
}

ube_conversion::result
ube_conversion::do_out(mbstate_t&,
					   const wchar_t* from, const wchar_t* from_end, const wchar_t*& from_next,
					   char* to, char* to_limit, char*& to_next) const
{
	const int max_input = (from_end - from);
	const int max_output = (to_limit - to) & ~1;
	int count = std::min(max_input, max_output / 2);	// TODO try to optimize by replacing division with shift

	from_next = from;
	to_next = to;

	for (;count--; ++from_next, to_next += 2)
	{
		*(to_next + 1) = *from_next & 0xFF;
		*(to_next + 0) = *from_next >> 8 & 0xFF;
	}
	return ok;
}


utf8_conversion::result
utf8_conversion::do_in(mbstate_t&,
					   const char*  from, const char* from_end, const char*& from_next,
					   wchar_t* to, wchar_t* to_limit, wchar_t*& to_next) const
{
	from_next = from;
	to_next = to;

	for(; to_next < to_limit && from_next < from_end; ++to_next)
	{
		if (static_cast<unsigned char>(*from_next) < 0x80)
		{
			*to_next = static_cast<unsigned char>(*from_next++);
		}
		else
		{
			const int zero_bit_pos = most_signifant_bit_position(~*from_next);
			int extra_bytes  = 7 - zero_bit_pos;

			if (from_end - from_next < extra_bytes + 1)
				return partial;

			*to_next = static_cast<unsigned char>(*from_next++) & ((1 << zero_bit_pos - 1) - 1);
			for (;extra_bytes--; ++from_next)
			{
				*to_next = *to_next << 6  |  static_cast<unsigned char>(*from_next) & 63;
			}
		}
	}
	return ok;
}


// TODO can be optimized, assuming UTF-8 has maximum 4 bytes per character.
// And then replacing division and multiplication with shifts
utf8_conversion::result
utf8_conversion::do_out(mbstate_t&,
						const wchar_t* from, const wchar_t* from_end, const wchar_t*& from_next,
						char* to, char* to_limit, char*& to_next) const
{
	from_next = from;
	to_next = to;

	for (;from_next < from_end; ++from_next)
	{
		const unsigned symbol = *from_next;

		if (symbol < 0x7F)
		{
			if (to_next < to_limit)
				*to_next++ = static_cast<unsigned char>(symbol);
			else
				return ok;
		}
		else
		{
			const size_t msb_pos = most_signifant_bit_position(symbol);
			int extra_bytes = msb_pos / 6;

			if (to_limit - to_next >= extra_bytes + 1)
			{
				*to_next = static_cast<unsigned char>(0xFF80 >> extra_bytes);
				*to_next++ |= take_6_bits(symbol, extra_bytes * 6);

				for(;extra_bytes--;)
					*to_next++ = 0x80 | take_6_bits(symbol, extra_bytes * 6);
			}
			else
			{
				return ok;
			}
		}
	}
	return ok;
}


const unsigned char 
utf8_conversion::take_6_bits(const int unsigned value, const size_t right_position) const
{
	return (value >> right_position) & 63;
}


const size_t 
utf8_conversion::most_signifant_bit_position(const unsigned int value) const
{
	size_t result = 0;
	size_t half = 16;

	for(; half > 0; half >>= 1)
	{
		if (1u << (result + half) <= value )
			result += half;
	}
	return result + 1;
}
