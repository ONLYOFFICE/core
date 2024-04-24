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

#ifndef _ZLIB_H
#define _ZLIB_H

#ifndef ZLIB_USE_DYNAMIC_LIBRARY
#define ZLIB_DECL_EXPORT
#else
#include "../../../../DesktopEditor/common/base_export.h"
#define ZLIB_DECL_EXPORT Q_DECL_EXPORT
#endif

#include <malloc.h>
#include <string>

#include "../../../src/ZipBuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

ZLIB_DECL_EXPORT void* Zlib_Malloc(unsigned int size);
ZLIB_DECL_EXPORT void  Zlib_Free(void* p);

ZLIB_DECL_EXPORT CZipBuffer* Zlib_Create();
ZLIB_DECL_EXPORT CZipBuffer* Zlib_Open(unsigned char* buffer, unsigned long size);
ZLIB_DECL_EXPORT void   Zlib_Close(CZipBuffer* p);

ZLIB_DECL_EXPORT void Zlib_AddFile   (CZipBuffer* p, const char* path, unsigned char* data, unsigned long length);
ZLIB_DECL_EXPORT void Zlib_RemoveFile(CZipBuffer* p, const char* path);

ZLIB_DECL_EXPORT unsigned char* Zlib_GetPaths(CZipBuffer* p);
ZLIB_DECL_EXPORT unsigned char* Zlib_GetFile (CZipBuffer* p, const char* path);
ZLIB_DECL_EXPORT unsigned char* Zlib_Save    (CZipBuffer* p);

#ifdef __cplusplus
}
#endif

#endif // _ZLIB_H
