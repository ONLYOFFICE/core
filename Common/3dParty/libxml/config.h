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

#ifndef __LIBXML_CONFIG_H__
#define __LIBXML_CONFIG_H__

/* Define to 1 if you have the declaration of 'getentropy', and to 0 if you
   don't. */
#define HAVE_DECL_GETENTROPY 0

/* Define to 1 if you have the declaration of 'glob', and to 0 if you don't.
   */
#ifdef _WIN32
#define HAVE_DECL_GLOB 0
#else
#define HAVE_DECL_GLOB 1
#endif

/* Define to 1 if you have the declaration of 'mmap', and to 0 if you don't.
   */
#ifdef _WIN32
#define HAVE_DECL_MMAP 0
#else
#define HAVE_DECL_MMAP 1
#endif

/* Define if __attribute__((destructor)) is accepted */
#ifndef _WIN32
#define HAVE_FUNC_ATTRIBUTE_DESTRUCTOR 1
#endif

/* Have dlopen based dso */
#ifndef _WIN32
#define HAVE_DLOPEN 1
#endif

/* Define if history library is there (-lhistory) */
/* #undef HAVE_LIBHISTORY */

/* Define if readline library is there (-lreadline) */
/* #undef HAVE_LIBREADLINE */

/* Have shl_load based dso */
/* #undef HAVE_SHLLOAD */

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* System configuration directory (/etc) */
#define XML_SYSCONFDIR "/etc"

/* TLS specifier */
#ifdef _WIN32
#define XML_THREAD_LOCAL __declspec(thread)
#else
#define XML_THREAD_LOCAL __thread
#endif

#endif /* __LIBXML_CONFIG_H__ */
