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
#ifndef _PDF_WRITER_SRC_CONSTS_H
#define _PDF_WRITER_SRC_CONSTS_H

#define  OK                    0
#define  NOERROR               0

// Default buffer size for memory-stream-object
#define STREAM_BUF_SIZ         4096

#define MAX_OPENED_FT_FACES    30

// Required buffer size for converting to character string.
#define SHORT_BUFFER_SIZE      32
#define REAL_LEN               31//11
#define INT_LEN                11
#define TEXT_DEFAULT_LEN       256
#define DATE_TIME_STR_LEN      23

// Corresponding lengths in cross-reference-table
#define BYTE_OFFSET_LEN        10
#define OBJ_ID_LEN             7
#define GEN_NO_LEN             5

// Default page size (A4)
#define DEF_PAGE_WIDTH         595.276
#define DEF_PAGE_HEIGHT        841.89

// Compression types

#define  COMP_NONE            0x00
#define  COMP_TEXT            0x01
#define  COMP_IMAGE           0x02
#define  COMP_METADATA        0x04
#define  COMP_ALL             0x0F
// #define  COMP_BEST_COMPRESS   0x10
// #define  COMP_BEST_SPEED      0x20
#define  COMP_MASK            0xFF


// Permission flags (only Revision 2 is supported)

#define ENABLE_READ         0
#define ENABLE_PRINT        4
#define ENABLE_EDIT_ALL     8
#define ENABLE_COPY         16
#define ENABLE_EDIT         32

// Document view options

#define HIDE_TOOLBAR    1
#define HIDE_MENUBAR    2
#define HIDE_WINDOW_UI  4
#define FIT_WINDOW      8
#define CENTER_WINDOW   16


// Limits according to specification
#define LIMIT_MAX_INT             2147483647
#define LIMIT_MIN_INT             -2147483647

#define LIMIT_MAX_REAL            3.402823466e+38F
#define LIMIT_MIN_REAL            -3.402823466e+38F

#define LIMIT_MAX_STRING_LEN      65535
#define LIMIT_MAX_NAME_LEN        127

#define LIMIT_MAX_ARRAY           65535
#define LIMIT_MAX_DICT_ELEMENT    4095
#define LIMIT_MAX_XREF_ELEMENT    8388607
#define MAX_GENERATION_NUM        65535

#endif // _PDF_WRITER_SRC_CONSTS_H
