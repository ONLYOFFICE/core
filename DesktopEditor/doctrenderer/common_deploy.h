/*
 * (c) Copyright Ascensio System SIA 2010-2024
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

﻿#ifndef _DOCBUILDER_COMMON_H
#define _DOCBUILDER_COMMON_H

#ifdef _MSC_VER
#define BUILDER_DECL __declspec(dllimport)
#else
#define BUILDER_DECL
#endif

#define OFFICESTUDIO_FILE_DOCUMENT            0x0040
#define OFFICESTUDIO_FILE_DOCUMENT_DOCX       OFFICESTUDIO_FILE_DOCUMENT + 0x0001
#define OFFICESTUDIO_FILE_DOCUMENT_DOC        OFFICESTUDIO_FILE_DOCUMENT + 0x0002
#define OFFICESTUDIO_FILE_DOCUMENT_ODT        OFFICESTUDIO_FILE_DOCUMENT + 0x0003
#define OFFICESTUDIO_FILE_DOCUMENT_RTF        OFFICESTUDIO_FILE_DOCUMENT + 0x0004
#define OFFICESTUDIO_FILE_DOCUMENT_TXT        OFFICESTUDIO_FILE_DOCUMENT + 0x0005
#define OFFICESTUDIO_FILE_DOCUMENT_DOTX       OFFICESTUDIO_FILE_DOCUMENT + 0x000c
#define OFFICESTUDIO_FILE_DOCUMENT_OTT        OFFICESTUDIO_FILE_DOCUMENT + 0x000f
#define OFFICESTUDIO_FILE_DOCUMENT_HTML       OFFICESTUDIO_FILE_DOCUMENT + 0x0012

#define OFFICESTUDIO_FILE_PRESENTATION        0x0080
#define OFFICESTUDIO_FILE_PRESENTATION_PPTX   OFFICESTUDIO_FILE_PRESENTATION + 0x0001
#define OFFICESTUDIO_FILE_PRESENTATION_PPT    OFFICESTUDIO_FILE_PRESENTATION + 0x0002
#define OFFICESTUDIO_FILE_PRESENTATION_ODP    OFFICESTUDIO_FILE_PRESENTATION + 0x0003
#define OFFICESTUDIO_FILE_PRESENTATION_PPSX   OFFICESTUDIO_FILE_PRESENTATION + 0x0004
#define OFFICESTUDIO_FILE_PRESENTATION_POTX   OFFICESTUDIO_FILE_PRESENTATION + 0x0007
#define OFFICESTUDIO_FILE_PRESENTATION_OTP    OFFICESTUDIO_FILE_PRESENTATION + 0x000a

#define OFFICESTUDIO_FILE_SPREADSHEET         0x0100
#define OFFICESTUDIO_FILE_SPREADSHEET_XLSX    OFFICESTUDIO_FILE_SPREADSHEET + 0x0001
#define OFFICESTUDIO_FILE_SPREADSHEET_XLS     OFFICESTUDIO_FILE_SPREADSHEET + 0x0002
#define OFFICESTUDIO_FILE_SPREADSHEET_ODS     OFFICESTUDIO_FILE_SPREADSHEET + 0x0003
#define OFFICESTUDIO_FILE_SPREADSHEET_CSV     OFFICESTUDIO_FILE_SPREADSHEET + 0x0004
#define OFFICESTUDIO_FILE_SPREADSHEET_XLTX    OFFICESTUDIO_FILE_SPREADSHEET + 0x0006
#define OFFICESTUDIO_FILE_SPREADSHEET_OTS     OFFICESTUDIO_FILE_SPREADSHEET + 0x0009

#define OFFICESTUDIO_FILE_CROSSPLATFORM       0x0200
#define OFFICESTUDIO_FILE_CROSSPLATFORM_PDF   OFFICESTUDIO_FILE_CROSSPLATFORM + 0x0001
#define OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU  OFFICESTUDIO_FILE_CROSSPLATFORM + 0x0003
#define OFFICESTUDIO_FILE_CROSSPLATFORM_XPS   OFFICESTUDIO_FILE_CROSSPLATFORM + 0x0004
#define OFFICESTUDIO_FILE_CROSSPLATFORM_PDFA  OFFICESTUDIO_FILE_CROSSPLATFORM + 0x0009

#define OFFICESTUDIO_FILE_IMAGE               0x0400
#define OFFICESTUDIO_FILE_IMAGE_JPG           OFFICESTUDIO_FILE_IMAGE + 0x0001
#define OFFICESTUDIO_FILE_IMAGE_PNG           OFFICESTUDIO_FILE_IMAGE + 0x0005
#define OFFICESTUDIO_FILE_IMAGE_BMP           OFFICESTUDIO_FILE_IMAGE + 0x0008

#endif // _DOCBUILDER_COMMON_H
