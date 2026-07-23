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

#ifndef _DOCBUILDER_COMMON_H
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
#define OFFICESTUDIO_FILE_DOCUMENT_OFORM_PDF  OFFICESTUDIO_FILE_DOCUMENT + 0x0017

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
