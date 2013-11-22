/* iptc-utils.c
 *
 * Copyright © 2005 David Moore <dcm@acm.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details. 
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "config.h"
#include "iptc-utils.h"

typedef signed short IptcSShort;

static IptcSShort
iptc_get_sshort (const unsigned char *buf, IptcByteOrder order)
{
	if (!buf) return 0;
        switch (order) {
        case IPTC_BYTE_ORDER_MOTOROLA:
                return ((buf[0] << 8) | buf[1]);
        case IPTC_BYTE_ORDER_INTEL:
                return ((buf[1] << 8) | buf[0]);
        }

	/* Won't be reached */
	return (0);
}

IptcShort
iptc_get_short (const unsigned char *buf, IptcByteOrder order)
{
	return (iptc_get_sshort (buf, order) & 0xffff);
}

void
iptc_set_short (unsigned char *b, IptcByteOrder order, IptcShort value)
{
	if (!b) return;
	switch (order) {
	case IPTC_BYTE_ORDER_MOTOROLA:
		b[0] = (unsigned char) (value >> 8);
		b[1] = (unsigned char) value;
		break;
	case IPTC_BYTE_ORDER_INTEL:
		b[0] = (unsigned char) value;
		b[1] = (unsigned char) (value >> 8);
		break;
	}
}

IptcSLong
iptc_get_slong (const unsigned char *b, IptcByteOrder order)
{
	if (!b) return 0;
        switch (order) {
        case IPTC_BYTE_ORDER_MOTOROLA:
                return ((b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3]);
        case IPTC_BYTE_ORDER_INTEL:
                return ((b[3] << 24) | (b[2] << 16) | (b[1] << 8) | b[0]);
        }

	/* Won't be reached */
	return (0);
}

void
iptc_set_slong (unsigned char *b, IptcByteOrder order, IptcSLong value)
{
	if (!b) return;
	switch (order) {
	case IPTC_BYTE_ORDER_MOTOROLA:
		b[0] = (unsigned char) (value >> 24);
		b[1] = (unsigned char) (value >> 16);
		b[2] = (unsigned char) (value >> 8);
		b[3] = (unsigned char) value;
		break;
	case IPTC_BYTE_ORDER_INTEL:
		b[3] = (unsigned char) (value >> 24);
		b[2] = (unsigned char) (value >> 16);
		b[1] = (unsigned char) (value >> 8);
		b[0] = (unsigned char) value;
		break;
	}
}

IptcLong
iptc_get_long (const unsigned char *buf, IptcByteOrder order)
{
        return (iptc_get_slong (buf, order) & 0xffffffff);
}

void
iptc_set_long (unsigned char *b, IptcByteOrder order, IptcLong value)
{
	iptc_set_slong (b, order, value);
}

