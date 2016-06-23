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
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1

#ifndef STRICT
#define STRICT
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0501		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0500	// Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS

#ifdef _WIN32
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>
#include <atlcoll.h>
#include "../Common/atldefine.h"


#include <wingdi.h>



//#define _USE_MATH_DEFINES
#include <math.h>

//#define NODOCX
//#define PPTX_DEF
//
//#define PPT_DEF
//#define ENABLE_PPT_TO_PPTX_CONVERT
//#define _AVS_PPT_SHAPE_INCLUDE_

using namespace ATL;
//#include "../ASCImageStudio3/ASCGraphics/Interfaces/ASCRenderer.h"
#include "../Common/Config.h"

//#import "../Redist/ASCOfficeUtils.dll"			named_guids raw_interfaces_only rename_namespace("OfficeUtils")
//#import "../Redist/ASCOfficeDocxFile2.dll"		named_guids raw_interfaces_only rename_namespace("DocxFile2")
//#import "../Redist/XlsxSerializerCom.dll"		named_guids raw_interfaces_only rename_namespace("XlsxCom"), exclude("_IAVSOfficeFileTemplateEvents"), exclude("_IAVSOfficeFileTemplateEvents2")



#ifdef BUILD_CONFIG_OPENSOURCE_VERSION

#import "../Redist/OfficeCore.dll"				named_guids raw_interfaces_only rename_namespace("OfficeCore")

namespace MediaCore
{
	typedef OfficeCore::IUncompressedFrame IAVSUncompressedVideoFrame;
	const GUID CLSID_CAVSUncompressedVideoFrame = OfficeCore::CLSID_CUncompressedFrame;
	const GUID IID_IAVSUncompressedVideoFrame = OfficeCore::IID_IUncompressedFrame;
}

namespace ASCGraphics
{
	typedef OfficeCore::IWinFonts IASCFontManager;
	const GUID CLSID_CASCFontManager = OfficeCore::CLSID_CWinFonts;
	const GUID IID_IASCFontManager = OfficeCore::IID_IWinFonts;
}

#else

//#define AVS_USE_CONVERT_PPTX_TOCUSTOM_VML

//#import "../Redist/ASCMediaCore3.dll"			named_guids raw_interfaces_only rename_namespace("MediaCore"), exclude("tagRECT")
//#import "../Redist/ASCImageStudio3.dll"			named_guids raw_interfaces_only rename_namespace("ImageStudio"), exclude("IASCRenderer")
//#import "../Redist/ASCHTMLRenderer.dll"			named_guids raw_interfaces_only rename_namespace("HtmlRenderer"), exclude("IASCRenderer")

//#import "../Redist/ASCGraphics.dll"				named_guids raw_interfaces_only rename_namespace("ASCGraphics")
#import "../Redist/ASCFontConverter.dll"		named_guids raw_interfaces_only rename_namespace("Fonts")

#endif // #ifdef _WIN32

#endif
