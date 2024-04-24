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

#ifndef _RASTER_H
#define _RASTER_H

#ifndef RASTER_USE_DYNAMIC_LIBRARY
#define RASTER_DECL_EXPORT
#else
#include "../../../../../common/base_export.h"
#define RASTER_DECL_EXPORT Q_DECL_EXPORT
#endif

#include <malloc.h>

#ifdef __cplusplus
extern "C" {
#endif

RASTER_DECL_EXPORT void* Raster_DecodeFile(unsigned char* buffer, int size, bool isRgba = false);
RASTER_DECL_EXPORT void* Raster_GetDecodedBuffer(void* frame);
RASTER_DECL_EXPORT int Raster_GetHeight(void* frame);
RASTER_DECL_EXPORT int Raster_GetWidth(void* frame);
RASTER_DECL_EXPORT int Raster_GetStride(void* frame);
RASTER_DECL_EXPORT void Raster_Destroy(void* frame);

RASTER_DECL_EXPORT void* Raster_EncodeImageData(unsigned char* buffer, int w, int h, int stride, int format, bool isRgba = false);
RASTER_DECL_EXPORT void* Raster_Encode(unsigned char* buffer, int size, int format);
RASTER_DECL_EXPORT void* Raster_GetEncodedBuffer(void* encodedData);
RASTER_DECL_EXPORT int Raster_GetEncodedSize(void* encodedData);
RASTER_DECL_EXPORT void Raster_DestroyEncodedData(void* encodedData);

RASTER_DECL_EXPORT int Image_GetFormat(unsigned char* buffer, int size);

#ifdef __cplusplus
}
#endif

#endif // _RASTER_H
