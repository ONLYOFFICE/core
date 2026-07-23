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
#ifndef _ASC_WIN_API
#define _ASC_WIN_API

#ifndef _WIN32

// from winuser.h ( http://msdn.microsoft.com/en-us/library/windows/desktop/ms724371(v=vs.85).aspx )
#ifndef COLOR_3DDKSHADOW
#define COLOR_3DDKSHADOW            21
#endif

#ifndef COLOR_3DFACE
#define COLOR_3DFACE                15
#endif

#ifndef COLOR_3DHIGHLIGHT
#define COLOR_3DHIGHLIGHT           20
#endif

#ifndef COLOR_3DHILIGHT
#define COLOR_3DHILIGHT             20
#endif

#ifndef COLOR_3DLIGHT
#define COLOR_3DLIGHT               22
#endif

#ifndef COLOR_3DSHADOW
#define COLOR_3DSHADOW              16
#endif

#ifndef COLOR_ACTIVEBORDER
#define COLOR_ACTIVEBORDER          10
#endif

#ifndef COLOR_ACTIVECAPTION
#define COLOR_ACTIVECAPTION         2
#endif

#ifndef COLOR_APPWORKSPACE
#define COLOR_APPWORKSPACE          12
#endif

#ifndef COLOR_BACKGROUND
#define COLOR_BACKGROUND            1
#endif

#ifndef COLOR_BTNFACE
#define COLOR_BTNFACE               15
#endif

#ifndef COLOR_BTNHIGHLIGHT
#define COLOR_BTNHIGHLIGHT          20
#endif

#ifndef COLOR_BTNHILIGHT
#define COLOR_BTNHILIGHT            20
#endif

#ifndef COLOR_BTNSHADOW
#define COLOR_BTNSHADOW             16
#endif

#ifndef COLOR_BTNTEXT
#define COLOR_BTNTEXT               18
#endif

#ifndef COLOR_CAPTIONTEXT
#define COLOR_CAPTIONTEXT           9
#endif

#ifndef COLOR_DESKTOP
#define COLOR_DESKTOP               1
#endif

#ifndef COLOR_GRAYTEXT
#define COLOR_GRAYTEXT              17
#endif

#ifndef COLOR_HIGHLIGHT
#define COLOR_HIGHLIGHT             13
#endif

#ifndef COLOR_HIGHLIGHTTEXT
#define COLOR_HIGHLIGHTTEXT        14
#endif

#ifndef COLOR_HOTLIGHT
#define COLOR_HOTLIGHT              26
#endif

#ifndef COLOR_INACTIVEBORDER
#define COLOR_INACTIVEBORDER        11
#endif

#ifndef COLOR_INACTIVECAPTION
#define COLOR_INACTIVECAPTION        3
#endif


#ifndef COLOR_INACTIVECAPTIONTEXT
#define COLOR_INACTIVECAPTIONTEXT    19
#endif

#ifndef COLOR_INFOBK
#define COLOR_INFOBK                24
#endif

#ifndef COLOR_INFOTEXT
#define COLOR_INFOTEXT              23
#endif

#ifndef COLOR_MENU
#define COLOR_MENU                  4
#endif

#ifndef COLOR_GRADIENTACTIVECAPTION
#define COLOR_GRADIENTACTIVECAPTION 27
#endif

#ifndef COLOR_GRADIENTINACTIVECAPTION
#define COLOR_GRADIENTINACTIVECAPTION 28
#endif

#ifndef COLOR_MENUHILIGHT
#define COLOR_MENUHILIGHT       29
#endif

#ifndef COLOR_MENUBAR
#define COLOR_MENUBAR           30
#endif


#ifndef COLOR_MENUTEXT
#define COLOR_MENUTEXT          7
#endif

#ifndef COLOR_SCROLLBAR
#define COLOR_SCROLLBAR         0
#endif

#ifndef COLOR_WINDOW
#define COLOR_WINDOW            5
#endif

#ifndef COLOR_WINDOWFRAME
#define COLOR_WINDOWFRAME       6
#endif

#ifndef COLOR_WINDOWTEXT
#define COLOR_WINDOWTEXT        8
#endif

unsigned int GetSysColor(const int nIndex);

#else
#include <windows.h>
#include <winuser.h>

#ifndef COLOR_HOTLIGHT
#define COLOR_HOTLIGHT          26
#endif

#ifndef COLOR_GRADIENTACTIVECAPTION
#define COLOR_GRADIENTACTIVECAPTION 27
#endif

#ifndef COLOR_GRADIENTINACTIVECAPTION
#define COLOR_GRADIENTINACTIVECAPTION 28
#endif

#ifndef COLOR_MENUHILIGHT
#define COLOR_MENUHILIGHT       29
#endif

#ifndef COLOR_MENUBAR
#define COLOR_MENUBAR           30
#endif

#endif //#ifndef _WIN32
#endif // #ifndef _ASC_WIN_API
