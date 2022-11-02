/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
